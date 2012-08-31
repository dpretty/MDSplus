from MDSplus import *
from numpy import *
from threading import *
from ctypes import *
import datetime
import time

class ZELOS2150GV(Device):
    print 'Zelos2150GV'
    Int32(1).setTdiVar('_PyReleaseThreadLock')
    """Zelos 2150GV Camera"""
    parts=[
      {'path':':NAME', 'type':'text'},
      {'path':':COMMENT', 'type':'text'},
      {'path':':IP', 'type':'text'},
      {'path':':EXP_TIME', 'type':'numeric', 'value':20E-3},
      {'path':':AUTO_EXP', 'type':'text', 'value':'NO'},
      {'path':':EXP_LEV', 'type':'numeric', 'value':0},
      {'path':':AUTO_GAIN', 'type':'text', 'value':'NO'},
      {'path':':GAIN_LEV', 'type':'numeric', 'value':100},
      {'path':':SLOW_SCAN', 'type':'text', 'value':'NO'},
      {'path':':FRAME_X', 'type':'numeric', 'value':11},
      {'path':':FRAME_Y', 'type':'numeric', 'value':11},
      {'path':':FRAME_WIDTH', 'type':'numeric', 'value':1920},
      {'path':':FRAME_HEIGHT', 'type':'numeric', 'value':1080},
      {'path':':MEAS_X', 'type':'numeric', 'value':11},
      {'path':':MEAS_Y', 'type':'numeric', 'value':11},
      {'path':':MEAS_WIDTH', 'type':'numeric', 'value':1920},
      {'path':':MEAS_HEIGHT', 'type':'numeric', 'value':1080},
      {'path':':VER_BINNING', 'type':'numeric', 'value':1},
      {'path':':HOR_BINNING', 'type':'numeric', 'value':1},
      {'path':':FRAME_SYNC', 'type':'text', 'value':'INTERNAL'},
      {'path':':FRAME_PERIOD', 'type':'numeric', 'value':100E-3},
      {'path':':FRAME_CLOCK', 'type':'numeric'},
      {'path':':FRAMES', 'type':'signal','options':('no_write_model', 'no_compress_on_put')},
      {'path':':STREAMING', 'type':'text', 'value':'Stream and Store'},
      {'path':':STREAM_PORT', 'type':'numeric', 'value':8888},]
    parts.append({'path':':INIT_ACT','type':'action',
	  'valueExpr':"Action(Dispatch('CAMERA_SERVER','PULSE_PREP',50,None),Method(None,'init',head))",
	  'options':('no_write_shot',)})
    parts.append({'path':':START_ACT','type':'action',
	  'valueExpr':"Action(Dispatch('CPCI_SERVER','INIT',50,None),Method(None,'start_store',head))",
	  'options':('no_write_shot',)})
    parts.append({'path':':STOP_ACT','type':'action',
	  'valueExpr':"Action(Dispatch('CPCI_SERVER','STORE',50,None),Method(None,'stop_store',head))",
	  'options':('no_write_shot',)})
    print 'Zelos2150GV added'
    
    
    handle = 0

####Asynchronous readout internal class       
    class AsynchStore(Thread):
      frameIdx = 0
      stopReq = False
   
    
      def configure(self, device, kappaLib, mdsLib, streamLib, width, height, hBuffers):
        self.device = device
        self.kappaLib = kappaLib
        self.mdsLib = mdsLib
        self.streamLib = streamLib
        self.width = width
        self.height = height
        self.hBuffers = hBuffers


      def run(self):
        frameType = c_short * (self.height.value * self.width.value) #used for acquired frame
        frameBuffer = frameType()

        frameType = c_byte * (self.height.value * self.width.value)  #used for streaming frame
        frame8bit = frameType()

        self.idx = 0

        treePtr = c_void_p(0);
        status = self.mdsLib.camOpenTree(c_char_p(self.device.getTree().name), c_int(self.device.getTree().shot), byref(treePtr))

        if status == -1:
          Data.execute('DevLogErr($1,$2)', self.device.getNid(), 'Cannot open tree')
          return 0

        if self.device.frame_sync.data() == 'EXTERNAL': 
          isExternal = 1
          timebaseNid=self.device.frame_clock.getNid()
        else:
          isExternal = 0
          timebaseNid=c_int(-1)
       
        if self.device.streaming.data() == 'Stream and Store': 
          isStreaming = 1
          isStorage = 1
        if self.device.streaming.data() == 'Only Stream': 
          isStreaming = 1
          isStorage = 0
        if self.device.streaming.data() == 'Only Store': 
          isStreaming = 0
          isStorage = 1


        tcpStreamHandle=c_int(-1)
        frameTime=0
        prevFrameTime=0
        framePeriod = int(self.device.frame_period.data()/1E-3)
        status=c_int(-1)
        streamPort=c_int(self.device.stream_port.data())

        lowLim=c_int(0)
        highLim=c_int(32767)

        while not self.stopReq:

          self.kappaLib.kappaGetFrame(self.device.handle, byref(status), frameBuffer)
          print 'get frame status:', status

          if isExternal==0:  #internal clock source -> S.O. timestamp
            timestamp=datetime.datetime.now()
            prevFrameTime=frameTime
            frameTime=int(time.mktime(timestamp.timetuple())*1000)+int(timestamp.microsecond/1000)
            deltaT=frameTime-prevFrameTime
            #print 'delta T: ', deltaT
            if (deltaT)<framePeriod:
              time.sleep((framePeriod-deltaT)/1E3)
              #print 'sleep di ', (framePeriod-deltaT)
            #print 'timestamp 64bit: ', frameTime
            #ts=datetime.datetime.now()
            #ft=int(time.mktime(ts.timetuple())*1000)+int(ts.microsecond/1000)
            #print 'timestamp 64bit post wait: ', ft

          if( (isStorage==1) and ((status.value==1) or (status.value==2)) ):    #frame complete or incomplete
            self.mdsLib.camSaveFrame(frameBuffer, self.width, self.height, byref(c_int64(frameTime)), c_int(14), treePtr, self.device.frames.getNid(), timebaseNid, c_int(self.idx)) 
            self.idx = self.idx + 1
            print 'saved frame idx:', self.idx

          if ( (isStreaming==1) and (tcpStreamHandle.value==-1) ):   
            self.streamLib.camOpenTcpConnection(streamPort, byref(tcpStreamHandle), self.width, self.height)   
            #print 'Stream Tcp Connection Opened. Handle:',tcpStreamHandle.value

          if ( (isStreaming==1) and (tcpStreamHandle.value!=-1) ): 
            #self.streamLib.camFrameTo8bit(frameBuffer, self.width, self.height, c_int(14), frame8bit);
            self.streamLib.camFrameTo8bit(frameBuffer, self.width, self.height, frame8bit, c_int(0), byref(lowLim), byref(highLim), c_int(0), c_int(32767));
            self.streamLib.camSendFrameOnTcp(byref(tcpStreamHandle), self.width, self.height, frame8bit);
   
        #endwhile
        self.streamLib.camCloseTcpConnection(byref(tcpStreamHandle))
        #print 'Stream Tcp Connection Closed'
    
        status = self.kappaLib.kappaStopAcquisition(self.device.handle, self.hBuffers)
        if status != 0:
          Data.execute('DevLogErr($1,$2)', self.device.getNid(), 'Cannot stop camera acquisition')

        #close device and remove from info
        self.kappaLib.kappaClose(self.device.handle)
        self.device.removeInfo()
        return 0


      def stop(self):
        self.stopReq = True
  #end class AsynchStore




###save worker###  
    def saveWorker(self):
      global zelosWorkers
      global zelosWorkerNids
      try:
        zelosWorkers
      except:
	zelosWorkerNids = []
        zelosWorkers = []
      try:
        idx = zelosWorkerNids.index(self.getNid())
        zelosWorkers[idx] = self.worker
      except:
        print 'SAVE WORKER: NEW WORKER'
        zelosWorkerNids.append(self.getNid())
        zelosWorkers.append(self.worker)
        return
      return

###save Info###   
#saveInfo and restoreInfo allow to manage multiple occurrences of camera devices 
#and to avoid opening and closing devices handles 
    def saveInfo(self):
      global zelosHandles
      global zelosNids
      try:
        zelosHandles
      except:
	zelosHandles = []
	zelosNids = []
      try:
        idx = zelosNids.index(self.getNid())
      except:
        print 'SAVE INFO: SAVING HANDLE'
        zelosHandles.append(self.handle)
        zelosNids.append(self.getNid())
        return
      return

###restore worker###   
    def restoreWorker(self):
      global zelosWorkerNids
      global zelosWorkers    
      try:
        idx = zelosWorkerNids.index(self.getNid())
        self.worker = zelosWorkers[idx]
      except:
        print 'Cannot restore worker!!'

###restore info###   
    def restoreInfo(self):
      global zelosHandles
      global zelosNids
      global kappaLib
      global streamLib
      global mdsLib
      try:
        kappaLib
      except:
        kappaLib = CDLL("libkappazelos.so")
      try:
        mdsLib
      except:
        mdsLib = CDLL("libcammdsutils.so")
      try:
        streamLib
      except:
        streamLib = CDLL("libcamstreamutils.so")
      try:
        idx = zelosNids.index(self.getNid())
        self.handle = zelosHandles[idx]
        print 'RESTORE INFO HANDLE TROVATO'
      except:
        print 'RESTORE INFO HANDLE NON TROVATO'
        try: 
          name = self.name.data()
        except:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Missing device name' )
          return 0

        self.handle = c_void_p(0)
        status = kappaLib.kappaOpen(c_char_p(name), byref(self.handle))
        if status < 0:  
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot open device '+ name)
          return 0
      return

###remove info###    
    def removeInfo(self):
      global zelosHandles
      global zelosNids
      try:
        zelosNids.remove(self.getNid())
        zelosHandles.remove(self.handle)
      except:
        print 'ERROR TRYING TO REMOVE INFO'
      return


##########init############################################################################    
    def init(self,arg):
      global kappaLib
      self.restoreInfo()
      self.frames.setCompressOnPut(False)	

      status = kappaLib.kappaSetColorCoding(self.handle, c_int(6));  #Y14
      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Color Coding')
        return 0

###Exposure	Mode
      if self.frame_sync.data() == 'EXTERNAL': 
        status = kappaLib.kappaSetExposureMode(self.handle, c_int(3)) #3 = ZELOS_ENUM_EXPOSUREMODE_RESETRESTART 
      else:
        status = kappaLib.kappaSetExposureMode(self.handle, c_int(2)) #2 = ZELOS_ENUM_EXPOSUREMODE_FREERUNNINGSEQUENTIAL

      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Exposure Mode')
        return 0

###Exposure	
      autoExp = self.auto_exp.data()
      if autoExp == 'YES':
      	status = kappaLib.kappaSetAET(self.handle, c_int(1))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set AET On')
          return 0
        status = kappaLib.kappaSetAutoExposureLevel(self.handle, c_int(self.exp_lev.data()))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Auto Exposure Level')
          return 0
      else:
      	status = kappaLib.kappaSetAET(self.handle, c_int(0))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set AET Off')
          return 0
        status = kappaLib.kappaSetExposure(self.handle, c_float(self.exp_time.data()))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Exposure Time')
          return 0
          
###Gain
      autoGain = self.auto_gain.data()
      if autoGain == 'YES':
      	status = kappaLib.kappaSetAGC(self.handle, c_int(1))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set AGC On')
          return 0
      else:
      	status = kappaLib.kappaSetAGC(self.handle, c_int(0))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set AGC On')
          return 0
        status = kappaLib.kappaSetGain(self.handle, c_int(self.gain_lev.data()))
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Gain')
          return 0

###Slow Scan
      slowScan = self.slow_scan.data()
      if slowScan == 'YES':
       status = kappaLib.kappaSetSlowScan(self.handle, c_int(1))
      else:
       status = kappaLib.kappaSetSlowScan(self.handle, c_int(0))
      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Slow Scan')
        return 0

###Frame Area
      status = kappaLib.kappaSetReadoutArea(self.handle, c_int(self.frame_x.data()),c_int(self.frame_y.data()),c_int(self.frame_width.data()),c_int(self.frame_height.data()))
      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Readout Area')
        return 0

###Measure Area
      status = kappaLib.kappaSetMeasureWindow(self.handle, c_int(self.meas_x.data()),c_int(self.meas_y.data()),c_int(self.meas_width.data()),c_int(self.meas_height.data()))
      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Set Measure Window')
        return 0

###Binning
      status = kappaLib.kappaSetBinning(self.handle, c_int(self.hor_binning), c_int(self.ver_binning))
      if status < 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot set horizontal or vertical binning')
        return 0

      self.saveInfo()
      return 1

####################trigger###PER ORA NON FUNZIONA
    def trigger(self,arg):
      global kappaLib
      self.restoreInfo()

      synch = self.frame_sync.data();  ###Synchronization
      if synch == 'INTERNAL':
        timeMs = int(self.frame_period.data()/1E-3)
        status = kappaLib.kappaSetTriggerTimer(self.handle, c_int(timeMs))		
        if status < 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot set Frame period in internal sync mode')
          return 0
      else:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot issue software trigger if external synchornization')
        return 0
      self.saveInfo()
      return 1
		
##########start store############################################################################   
    def start_store(self, arg):
      global kappaLib
      global mdsLib
      global streamLib
      self.restoreInfo()
      self.worker = self.AsynchStore()        
      self.worker.daemon = True 
      self.worker.stopReq = False
      hBuffers = c_void_p(0)
      width = c_int(0)
      height = c_int(0)
      payloadSize = c_int(0)
      status = kappaLib.kappaStartAcquisition(self.handle, byref(hBuffers), byref(width), byref(height), byref(payloadSize))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Start Camera Acquisition')
        return 0
      self.worker.configure(self, kappaLib, mdsLib, streamLib, width, height, hBuffers)
      self.saveWorker()
      self.worker.start()
      return 1


##########stop store############################################################################   
    def stop_store(self,arg):
      print 'STOP STORE'
      self.restoreWorker()
      self.worker.stop()
      print 'FLAG SETTATO'
      return 1
