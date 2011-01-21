from MDSplus import *
from numpy import *
class CAENDT5720(Device):
    print 'CAENDT5720'
    """CAEN DT5720 4 Channels 12 Bit 250MS/S Digitizer"""
    parts=[{'path':':BOARD_ID', 'type':'numeric', 'value':0},
      {'path':':COMMENT', 'type':'text'},
      {'path':':VME_ADDRESS', 'type':'numeric'},
      {'path':':TRIG_MODE', 'type':'text', 'value':'OVER THRESHOLD'},
      {'path':':TRIG_SOFT', 'type':'text', 'value':'ENABLED'},
      {'path':':TRIG_EXT', 'type':'text', 'value':'ENABLED'},
      {'path':':TRIG_SOURCE', 'type':'numeric'},
      {'path':':CLOCK_MODE', 'type':'text', 'value':'250 MHz'},
      {'path':':CLOCK_SOURCE', 'type':'numeric'},
      {'path':':NUM_SEGMENTS', 'type':'numeric','value':1024},
      {'path':':USE_TIME', 'type':'text', 'value':'YES'},
      {'path':':PTS', 'type':'numeric','value':1024},
      {'path':':START_IDX', 'type':'numeric','value':0},
      {'path':':END_IDX', 'type':'numeric','value':1024},
      {'path':':START_TIME', 'type':'numeric','value':0},
      {'path':':END_TIME', 'type':'numeric','value':1E-6}]
    for i in range(0,4):
        parts.append({'path':'.CHANNEL_%d'%(i+1), 'type':'structure'})
        parts.append({'path':'.CHANNEL_%d:STATE'%(i+1), 'type':'text', 'value':'ENABLED'})
        parts.append({'path':'.CHANNEL_%d:TRIG_STATE'%(i+1), 'type':'text', 'value':'DISABLED'})
        parts.append({'path':'.CHANNEL_%d:TRIG_MODE'%(i+1), 'type':'text', 'value':'SOFTWARE'})
        parts.append({'path':'.CHANNEL_%d:OFFSET'%(i+1), 'type':'numeric', 'value':0})
        parts.append({'path':'.CHANNEL_%d:DAC_OFFSET'%(i+1), 'type':'numeric', 'value':0})
        parts.append({'path':'.CHANNEL_%d:THRESH_LEVEL'%(i+1), 'type':'numeric', 'value':0})
        parts.append({'path':'.CHANNEL_%d:THRESH_SAMPL'%(i+1), 'type':'numeric', 'value':0})
        parts.append({'path':'.CHANNEL_%d:DATA'%(i+1), 'type':'signal'})
    del i
    parts.append({'path':':INIT_ACTION','type':'action',
	  'valueExpr':"Action(Dispatch('CPCI_SERVER','INIT',50,None),Method(None,'init',head))",
	  'options':('no_write_shot',)})
    parts.append({'path':':STORE_ACTION','type':'action',
	  'valueExpr':"Action(Dispatch('CPCI_SERVER','STORE',50,None),Method(None,'store',head))",
	  'options':('no_write_shot',)})
    cvV1718 = 0L                    # CAEN V1718 USB-VME bridge    
    cvV2718 = 1L                    # V2718 PCI-VME bridge with optical link       
    cvA2818 = 2L                    # PCI board with optical link                  
    cvA2719 = 3L                    # Optical link piggy-back                      
    cvA32_S_DATA = 0x0D             # A32 supervisory data access                  */
    cvD32 = 0x04		    # D32
    cvD64 = 0x08


    def init(self,arg):
      from ctypes import CDLL, c_int, c_short, c_long, byref, Structure
      import time
      caenLib = CDLL("libCAENVME.so")
      handle = c_long(0)
      status = caenLib.CAENVME_Init(c_int(self.cvV2718), c_int(0), c_int(0), byref(handle))
      if status != 0:
        print 'Error initializing CAENVME' 
        return 0

      try:
        boardId = self.board_id.data()
        print 'BOARD ID: ', boardId
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid Board ID specification')
        return 0
	
      try:
        vmeAddress = self.vme_address.data()
        print 'VME ADDRESS: ', vmeAddress
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid VME Address')
        return 0
  
#Module Reset
      data = c_int(0)
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0xEF24), byref(data), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error resetting V1740 Device' )
        caenLib.CAENVME_End(handle)
        return 0
	
    #give some time
      time.sleep(0.01)

#number of segments
      segmentDict = {1:0, 2:1, 4:2, 8:3, 16:4, 32:5, 64:6, 128:7, 256:8, 512:9, 1024:10}
      try:
        nSegments=self.num_segments.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid Number of Segments')
        return 0
      segmentCode = segmentDict[nSegments]
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x800c), byref(c_int(segmentCode)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing number of segments' )
        caenLib.CAENVME_End(handle)
        return 0
#Global Channel Configuration
      trigModeDict = {'OVER THRESHOLD':0, 'UNDER THRESHOLD':1}
      try:
        trigMode = self.trig_mode.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid Trigger mode')
        return 0
      trigModeCode = trigModeDict[trigMode]
      conf = trigModeCode << 6
      conf = conf | 0x00000010
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8000), byref(c_int(conf)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing group configuration')
        caenLib.CAENVME_End(handle)
        return 0

#Channel configurations
      trigEnableCode = 0L
      chanEnableCode = 0L
      enabledDict = {'ENABLED':1, 'DISABLED':0}
      for chan in range(0,4):
#threshold level
        threshold = getattr(self, 'channel_%d_thresh_level'%(chan+1)).data()
        status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x1080 + chan * 0x100), byref(c_int(threshold)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
        if status != 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing threshold level')
          caenLib.CAENVME_End(handle)
          return 0
#threshold samples
        threshSamples = getattr(self, 'channel_%d_thresh_sampl'%(chan+1)).data()
        status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x1084 + chan * 0x100), byref(c_int(threshSamples)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
        if status != 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing threshold samples')
          caenLib.CAENVME_End(handle)
          return 0
#offset
        offset = getattr(self, 'channel_%d_offset'%(chan+1)).data()
        if(offset > 1):
          offset = 1.
        if(offset < -1):
          offset = -1
        offset = (offset / 1.) * 32767
        status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x1098 + chan * 0x100), byref(c_int(int(offset + 0x08000))), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
        if status != 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing DAC offset')
          caenLib.CAENVME_End(handle)
          return 0
#states
        state = getattr(self, 'channel_%d_state'%(chan+1)).data()
        chanEnableCode = chanEnableCode | (enabledDict[state] << chan)
        trigState = getattr(self, 'channel_%d_trig_state'%(chan+1)).data()
        trigEnableCode = trigEnableCode | (enabledDict[trigState] << chan)

#endfor chan in range(0,4)
      
#Set channel enabled mask
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8120), byref(c_int(chanEnableCode)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing Channel enable register')
        caenLib.CAENVME_End(handle)
        return 0
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x810C), byref(c_int(trigEnableCode)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing Channel trigger enable register')
        caenLib.CAENVME_End(handle)
        return 0

#Set trigger enabling
      trigExt = self.trig_ext.data()
      trigEnableCode = trigEnableCode | (enabledDict[trigExt] << 30)
      trigSoft = self.trig_soft.data()
      trigEnableCode = trigEnableCode | (enabledDict[trigSoft] << 31)
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x810C), byref(c_int(trigEnableCode)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error writing trigger configuration')
        caenLib.CAENVME_End(handle)
        return 0

#Front Panel trigger out setting set TRIG/CLK to TTL
      data = 1
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x811C), byref(c_int(data)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))

#trigger source
      try:
        trigSource = self.trig_source.data()
#if trigger is expressed as an array, consider only the first element
        if len(self.trig_source.getShape()) > 0:
          trigSource = trigSource[0]
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot resolve Trigger source')
        caenLib.CAENVME_End(handle)
        return 0

#Clock source	
      clockMode = self.clock_mode.data()
      if clockMode == 'EXTERNAL':
        try:
          clockSource = self.clock_source()
        except:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot resolve Clock source')
          caenLib.CAENVME_End(handle)
          return 0
      else:
        clockSource = Range(None, None, Float64(1/250E6))
        self.clock_source.putData(clockSource)

#Post Trigger Samples
      try:
        pts = self.pts.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot resolve PTS Samples')
        caenLib.CAENVME_End(handle)
        return 0
      segmentSize = 1048576/nSegments
      if pts > segmentSize:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'PTS Larger than segmentSize')
        caenLib.CAENVME_End(handle)
        return 0
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8114), byref(c_int(pts>>2)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))

#Time management
      useTime=self.use_time.data()
      if useTime == 'YES':
        try:
          startTime = self.start_time.data()
          endTime = self.end_time.data()
        except:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot Read Start or End time')
          caenLib.CAENVME_End(handle)
          return 0
        if endTime > 0:
          endIdx = Data.execute('x_to_i($1, $2)', Dimension(Window(0, None, trigSource), clockSource), endTime + trigSource)
        else:
          endIdx = -Data.execute('x_to_i($1,$2)', Dimension(Window(0, None, trigSource + endTime), clockSource), trigSource)
        self.end_idx.putData(Int32(endIdx))
        if startTime > 0:
          startIdx = Data.execute('x_to_i($1, $2)', Dimension(Window(0, None, trigSource), clockSource), startTime + trigSource)
        else:
          startIdx = -Data.execute('x_to_i($1,$2)', Dimension(Window(0, None, trigSource + startTime), clockSource), trigSource)
        self.start_idx.putData(Int32(startIdx))
#Internal/External clock


# Run device
      runCommand = 4 
      if clockMode == 'EXTERNAL':
        runCommand = runCommand | 0x00000040
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8100), byref(c_int(4)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      caenLib.CAENVME_End(handle)
      return 1


################################TRIGGER###################################

    def trigger(self,arg):
      from ctypes import CDLL, c_int, c_short, c_long, byref, Structure
      import time
      caenLib = CDLL("libCAENVME.so")
      handle = c_long(0)
      status = caenLib.CAENVME_Init(c_int(self.cvV2718), c_int(0), c_int(0), byref(handle))
      if status != 0:
        print 'Error initializing CAENVME' 
        caenLib.CAENVME_End(handle)
        return 0
      try:
        boardId = self.board_id.data()
        vmeAddress = self.vme_address.data()
  #Module SW trigger
        data = c_int(0)
        status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8108), byref(c_int(0L)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
        if status != 0:
          print 'Error resetting V1740 Device' 
          caenLib.CAENVME_End(handle)
          return 0
        caenLib.CAENVME_End(handle)
        return 1
      except:
        print 'Generic SW trigger Error' 
        caenLib.CAENVME_End(handle)
        return 0

####################################STORE###################################

    def store(self,arg):
      from ctypes import CDLL, c_int, c_short, c_long, byref, Structure
      import time
      caenLib = CDLL("libCAENVME.so")
      handle = c_long(0)
      status = caenLib.CAENVME_Init(c_int(self.cvV2718), c_int(0), c_int(0), byref(handle))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error initializing CAENVME' )
        caenLib.CAENVME_End(handle)
        return 0
      try:
        boardId = self.board_id.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid Board ID specification')
        return 0
	
      try:
        vmeAddress = self.vme_address.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Invalid VME Address')
        return 0
  
      try:
        clock = self.clock_source.evaluate()
        dt = clock.getDelta().data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error evaluating clock source' )
        caenLib.CAENVME_End(handle)
        return 0
      try:
        trig = self.trig_source.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error evaluating trigger source' )
        caenLib.CAENVME_End(handle)
        return 0
      try:
        startIdx = self.start_idx.data()
        endIdx = self.end_idx.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error evaluating start or end idx')
        caenLib.CAENVME_End(handle)
        return 0
      try:
        pts = self.pts.data()
      except:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error evaluating Post Trigger Samples' )
        caenLib.CAENVME_End(handle)
        return 0
    
 # Stop device 
      status = caenLib.CAENVME_WriteCycle(handle, c_int(vmeAddress + 0x8100), byref(c_int(0L)), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error stopping device')
        caenLib.CAENVME_End(handle)
        return 0
    #need to wait a while
      time.sleep(0.1)
    
    
    # Read number of buffers 
      actSegments = c_int(0)
      status = caenLib.CAENVME_ReadCycle(handle, c_int(vmeAddress + 0x812C), byref(actSegments), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      if status != 0:
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error reading number of acquired segments' )
        caenLib.CAENVME_End(handle)
        return 0
    
      if actSegments.value == 0:
        caenLib.CAENVME_End(handle)
        return 1

    
    #Compute Segment Size
      try:
        nSegments = self.num_segments.data()
        segmentSamples = 1048576/nSegments
      except: 
        Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error reading max number of segments' )
        caenLib.CAENVME_End(handle)
        return 0
    	
    
    # Get Active channels
      chanMask = c_int(0)
      status = caenLib.CAENVME_ReadCycle(handle, c_int(vmeAddress + 0x8120), byref(chanMask), c_int(self.cvA32_S_DATA), c_int(self.cvD32))
      nActChans = 0
      chanMask = chanMask.value
      for chan in range(0,4):
        if (chanMask & (1 << chan)) != 0:
          nActChans = nActChans + 1
      if nActChans == 0:
        print 'No active groups' 
        caenLib.CAENVME_End(handle)
        return 1
 
      segmentSize = 16 + 2*segmentSamples * nActChans

 #     class DT5720Data(Structure):
 #       _fields_ = [("eventSize", c_int), ("boardGroup", c_int), ("counter", c_int), ("time", c_int), ("data", c_int * (segmentSize / 4))]
      class DT5720Data(Structure):
        _fields_ = [("eventSize", c_int), ("boardGroup", c_int), ("counter", c_int), ("time", c_int), ("data", c_short * (segmentSize / 2))]
    
      actSegments = actSegments.value
      currStartIdx = segmentSamples - pts + startIdx
      currEndIdx = segmentSamples - pts + endIdx
      currChanSamples = currEndIdx - currStartIdx
      triggers = []
      deltas = []
      channels = [] 
      for chan in range(0,4):
        channels.append([])
      for chan in range(0,4):
        channels[chan] = ndarray(currChanSamples * actSegments)
      for segmentIdx in range(0,actSegments):
        segment= DT5720Data()
        retLen = c_int(0)
        status = caenLib.CAENVME_FIFOBLTReadCycle(handle, c_int(vmeAddress), byref(segment), c_int(segmentSize),  c_int(self.cvA32_S_DATA), c_int(self.cvD64), byref(retLen))
        if status != 0:
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error reading data segment')
          caenLib.CAENVME_End(handle)
          return 0

        actSize = 4 * (segment.eventSize & 0x0fffffff)
        if actSize != segmentSize: 
          Data.execute('DevLogErr($1,$2)', self.getNid(), 'Expected event size different from expected size' )		  
          caenLib.CAENVME_End(handle)
          return 0
        counter = segment.time/2
        triggers.append(counter*dt)
        deltas.append(dt)
        sizeInInts = (segment.eventSize & 0x0fffffff) - 4;
        chanSizeInInts = sizeInInts/nActChans
        chanSizeInShorts = chanSizeInInts * 2
        chanOffset = 0
        for chan in range(0,4):
          if (chanMask & (1 << chan)) != 0:
            channels[chan][segmentIdx * currChanSamples : segmentIdx * currChanSamples + currEndIdx - currStartIdx] = segment.data[chan*chanSizeInShorts+currStartIdx:chan*chanSizeInShorts+currEndIdx]
        #endfor  chan in range(0,4)
      #endfor segmentIdx in range(0,actSegments):
      if len(self.trig_source.getShape()) > 0:
        dim = Dimension(Window(startIdx,endIdx+(actSegments - 1) * (endIdx - startIdx), trig[0]),Range(Float64Array(trig) + Float64(startIdx * dt),  Float64Array(trig) + Float64(endIdx * dt), Float64Array(deltas)))
      else:
        dim = Dimension(Window(startIdx,endIdx+(actSegments - 1) * (endIdx - startIdx), trig),Range(Float64Array(triggers) - Float64(triggers[0]) + Float64(trig) + Float64(startIdx * dt),  Float64Array(triggers) - Float64(triggers[0]) + Float64(trig) + Float64(endIdx * dt), Float64Array(deltas)))
      dim.setUnits("s");
      for chan in range(0,4):
        if (chanMask & (1 << chan)) != 0:
          try:        
            offset = getattr(self, 'channel_%d_offset'%(chan+1))	  
          except:
            Data.execute('DevLogErr($1,$2)', self.getNid(), 'Error reading channel offset')		  
            caenLib.CAENVME_End(handle)
            return 0
          raw = Int16Array(channels[chan])
          raw.setUnits("counts")
          data = Data.compile("2*($VALUE - 2048)/4096.+$1", offset);
          data.setUnits("Volts")
          signal = Signal(data, raw, dim)
          try:
            getattr(self, 'channel_%d_data'%(chan+1)).putData(signal)
          except:
            Data.execute('DevLogErr($1,$2)', self.getNid(), 'Cannot write Signal in tree')		  
            caenLib.CAENVME_End(handle)
            return 0
      #endfor chan in range(0,4)
      caenLib.CAENVME_End(handle)
      return 1

 
