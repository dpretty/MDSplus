from MDSplus import Device,Data,Action,Dispatch,Method, makeArray, Range, Signal, Window, Dimension
from Dt200WriteMaster import Dt200WriteMaster
import acq200
import transport

from time import sleep
import os
import numpy

class DT216B(Device):
    """
    D-Tacq ACQ216  16 channel transient recorder
    
    Methods:
    Add() - add a DT216B device to the tree open for edit
    Init(arg) - initialize the DT216B device 
                write setup parameters and waveforms to the device
    Store(arg) - store the data acquired by the device
    Help(arg) - Print this message
    
    Nodes:
    
    :HOSTIP - mdsip address of the host storing the data.  Usually 192.168.0.254:8106
     BOARDIP'- ip addres sof the card as a string something like 192.168.0.40
     COMMENT - user comment string
     DI[0-5] - time(s) of the signal on this internal wire (trig reference or clock reference)
            :wire - string specifying the source of this signal { 'fpga','mezz','rio','pxi','lemo', 'none }
            :bus  - string specifying the destination of this signal { 'fpga','rio','pxi', 'none }
    :ACTIVE_CHANS - number of active channels {2, 4, 8, 16}
     INT_CLOCK - stored by module (representation of internal clock       
     TRIG_SRC - reference to DIn line used for trigger (DI3)
     TRIG_EDGE - string {rising, falling} 
     CLOCK_SRC - reference to line (DIn) used for clock or INT_CLOCK
     CLOCK_DIV - NOT CURRENTLY IMPLIMENTED 
     CLOCK_EDGE -  string {rising, falling}
     CLOCK_FREQ - frequency for internal clock
     PRE_TRIG - pre trigger samples MUST BE ZERO FOR NOW
     POST_TRIG - post trigger samples
     SEGMENTS - number of segments to store data in NOT IMPLIMENTED FOR NOW
     CLOCK - Filled in by store place for module to store clock information
     RANGES - place for module to store calibration information 
     STATUS_CMDS - array of shell commands to send to the module to record firmware version  etc
     BOARD_STATUS - place for module to store answers for STATUS_CMDS as signal
     INPUT_[01-16] - place for module to store data in volts (reference to INPUT_NN:RAW)
                  :RAW - place for module to store raw data in volts for each channel
                  START_IDX - first sample to store for this channel
                  END_IDX - last sample to store for this channel
                  INC - decimation factor for this channel
     INIT_ACTION - dispatching information for INIT
     STORE_ACTION - dispatching information for STORE
    """
    
    parts=[
        {'path':':HOSTIP','type':'text','value':'192.168.0.254','options':('no_write_shot',)},
        {'path':':BOARDIP','type':'text','value':'192.168.0.0','options':('no_write_shot',)},
        {'path':':COMMENT','type':'text'},
        ]
    for i in range(6):
        parts.append({'path':':DI%1.1d'%(i,),'type':'numeric','options':('no_write_shot',)})
        parts.append({'path':':DI%1.1d:BUS'%(i,),'type':'text','options':('no_write_shot',)})
        parts.append({'path':':DI%1.1d:WIRE'%(i,),'type':'text','options':('no_write_shot',)})
    parts2=[
        {'path':':ACTIVE_CHANS','type':'numeric','value':16,'options':('no_write_shot',)},        
        {'path':':INT_CLOCK','type':'axis','options':('no_write_model','write_once')},       
        {'path':':TRIG_SRC','type':'numeric','valueExpr':'head.di3','options':('no_write_shot',)},
        {'path':':TRIG_EDGE','type':'text','value':'rising','options':('no_write_shot',)},
        {'path':':CLOCK_SRC','type':'numeric','valueExpr':'head.int_clock','options':('no_write_shot',)},
        {'path':':CLOCK_DIV','type':'numeric','value':1,'options':('no_write_shot',)},
        {'path':':CLOCK_EDGE','type':'text','value':'rising','options':('no_write_shot',)},
        {'path':':CLOCK_FREQ','type':'numeric','value':1000000,'options':('no_write_shot',)},
        {'path':':PRE_TRIG','type':'numeric','value':0,'options':('no_write_shot',)},
        {'path':':POST_TRIG','type':'numeric','value':128,'options':('no_write_shot',)},
        {'path':':SEGMENTS','type':'numeric','value':1,'options':('no_write_shot',)},
        {'path':':CLOCK','type':'axis','options':('no_write_model','write_once')},
        {'path':':RANGES','type':'numeric','options':('no_write_model','write_once')},
        {'path':':STATUS_CMDS','type':'text','value':makeArray(['cat /proc/cmdline', 'get.d-tacq.release']),'options':('no_write_shot',)},
        {'path':':BOARD_STATUS','type':'SIGNAL','options':('no_write_model','write_once')},
        ]
    parts.extend(parts2)
    del parts2
    for i in range(16):
        parts.append({'path':':INPUT_%2.2d'%(i+1,),'type':'signal','options':('no_write_model','write_once',)})
        parts.append({'path':':INPUT_%2.2d:RAW'%(i+1,),'type':'SIGNAL', 'options':('no_write_model','write_once')})
        parts.append({'path':':INPUT_%2.2d:START_IDX'%(i+1,),'type':'NUMERIC', 'options':('no_write_shot')})
        parts.append({'path':':INPUT_%2.2d:END_IDX'%(i+1,),'type':'NUMERIC', 'options':('no_write_shot')})
        parts.append({'path':':INPUT_%2.2d:INC'%(i+1,),'type':'NUMERIC', 'options':('no_write_shot')})
    parts.append({'path':':INIT_ACTION','type':'action',
                  'valueExpr':"Action(Dispatch('CAMAC_SERVER','INIT',50,None),Method(None,'INIT',head))",
                  'options':('no_write_shot',)})
    parts.append({'path':':STORE_ACTION','type':'action',
                  'valueExpr':"Action(Dispatch('CAMAC_SERVER','STORE',50,None),Method(None,'STORE',head))",
                  'options':('no_write_shot',)})
    
    clock_edges=['rising', 'falling']
    trigger_edges = clock_edges
    trig_sources=[ 'DI0',
                   'DI1',
                   'DI2',
                   'DI3',
                   'DI4',
                   'DI5',
                   ]
    clock_sources = trig_sources
    clock_sources.append('INT_CLOCK')
    
    masks = {8: '11110000000000001111000000000000',
             16:'11111111000000001111111100000000',
             }
    wires = [ 'fpga','mezz','rio','pxi','lemo', 'none', 'fpga pxi']
    
    del i
    
    def check(self, expression, message):
        try:
            ans = eval(expression)
            return ans
        except:
            raise Exception, message
        
    def init(self, arg):
        """
        Initialize the device
        Send parameters
        Arm hardware
        """

        debug=os.getenv("DEBUG_DEVICES")
        try:
            boardip=self.check( 'str(self.boardip.record)', "Must specify a board ipaddress")
            UUT = acq200.Acq200(transport.factory(boardip))
            active_chans = self.check("int(self.active_chans)", "Must specify active chans as int in (2, 4, 8, 16)")
            if active_chans not in (2,4,8,16) :
                print "active chans must be in (2, 4,8, 16 )"
                active_chans = 16
            trig_src=self.check('self.trig_src.record.getOriginalPartName().getString()[1:]', "Trig source must be a string")
            print "trig_src is %s\n" % trig_src
            if not trig_src in self.trig_sources:
                raise Exception, "Trig_src must be in %s" % str(self.trig_sources)
            trig_edge=self.check('self.trig_edge.record.getString()', 'Trig edge must be a string')
            clock_src=self.check('self.clock_src.record.getOriginalPartName().getString()[1:]', "Clock source must be a string")
            if debug:
                print "clock_src is %s\n" % clock_src
            if not clock_src in self.clock_sources:
                raise Exception, "Clock_src must be in %s" % str(self.clock_sources)
            if (clock_src == 'INT_CLOCK'):
                clock_freq = self.check('int(self.clock_freq)', "Must specify a frequency for internal clock")
            else:
                clock_freq = self.check('int(self.clock_freq)', "Must specify a frequency for internal clock")                
                clock_div = self.check('int(self.clock_div)', "Must specify a divisor for external clock")
            pre_trig=self.check('int(self.pre_trig.data()*1024)', "Must specify pre trigger samples")
            post_trig=self.check('int(self.post_trig.data()*1024)', "Must specify post trigger samples")
            UUT.set_abort()
            UUT.clear_routes()
            
            for i in range(6):
                line = 'd%1.1d' % i
                try:
                    wire = eval('str(self.di%1.1d_wire.record)' %i)
                    if wire not in self.wires :
                        print "DI%d:wire must be in %s" % (i, str(self.wires), )
                        wire = 'fpga'
                except:
                    wire = 'fpga'
                try:
                    bus = eval('str(self.di%1.1d_bus.record)' % i)
                    if bus not in self.wires :
                        print "DI%d:bus must be in %s" % (i, str(self.wires),)
                        bus = ''
                except:
                    bus = ''
                UUT.set_route(line, 'in %s out %s' % (wire, bus,))
            UUT.setChannelCount(active_chans)

            if clock_src == 'INT_CLOCK' :
                UUT.uut.acqcmd("setInternalClock %d" % clock_freq)
            else:
		UUT.uut.acqcmd("setExternalClock %s" % clock_src)
            UUT.setPrePostMode(pre_trig, post_trig)
            UUT.set_arm()
            return  1

        except Exception,e:
            print "%s\n" % (str(e),)
            return 0

    INIT=init
        
    def getVins(self, UUT):
        vins = UUT.uut.acq2sh('get.vin 1:16')
        ans = eval('makeArray([%s])' % vins)
        return ans

        
    def getInternalClock(self, UUT):
        clock_str = UUT.uut.acqcmd('getInternalClock').split()[0].split('=')[1]
        print "clock_str is -%s-" % clock_str
        return eval('int(%s)' % clock_str)
        
    def store(self, arg):
        """
        Store the data from the device
        Fetch and store the device status (firmware etc)
        If the device is finished
        For each channel that is on and active in the mask
        read the data
        store the data into the raw nodes
        store the expression into the data nodes
        """


        debug=os.getenv("DEBUG_DEVICES")
        try:
            boardip=self.check( 'str(self.boardip.record)', "Must specify a board ipaddress")
            UUT = acq200.Acq200(transport.factory(boardip))
            try:
                ans = []
                cmds = self.status_cmds.record
                for cmd in cmds:
                    print cmd
                    a = UUT.uut.acq2sh(cmd)
                    ans.append(a)
                self.board_status.record = Data.compile('build_signal($,*, $)', ans, cmds)
            except Exception, e:
                pass

            complete = 0
            tries = 0
            while not complete and tries < 60 :
                if UUT.get_state().split()[-1] == "ST_POSTPROCESS" :
                    tries +=1
                    sleep(1)
                else:
                    complete=1
            if UUT.get_state().split()[-1] != "ST_STOP" :
                raise Exception, "Device not Triggered \n device returned -%s-" % UUT.get_state().split()[-1]
            if debug:
                print "about to get the vins\n"
            vins = self.getVins(UUT)
            self.ranges.record = vins
            (tot, pre, post, run) = UUT.get_numSamples()
            pre = int(pre)
            post = int(post)
            mask = UUT.uut.acqcmd('getChannelMask').split('=')[-1]
            if debug:
                print "pre = %d, post = %d" % (pre, post, )
            clock_src=self.check('self.clock_src.record.getOriginalPartName().getString()[1:]', "Clock source must be a string") 
            if clock_src == 'INT_CLOCK' :
                self.clock.record = Range(delta=1./self.getInternalClock(UUT))
            else:
                self.clock.record = self.clock_src

            clock = self.clock.record
            if debug:
                print "about to ask it to mdsconnect"
            status = UUT.uut.acq2sh("mdsConnect %s" % str(self.hostip.record))
            if debug:
                print "    ...returned %s" % status
            if not status.startswith('MDS_SOCK') :
		raise Exception("DT216B board mdsconnect error", str(self.hostip.record), status) 
            if debug:
                print "about to ask it to mdsopen"
            status = UUT.uut.acq2sh('mdsOpen %s %d'  % (self.boardip.tree.name, self.boardip.tree.shot,))
            if debug:
                print "    ...returned %s" % status
            if not status.startswith('OK') :
                raise Exception("DT216B board mdsopen error %s %d"%(self.boardip.tree.name, self.boardip.tree.shot,), status)
            for chan in range(16):
                if debug:
                    print "working on channel %d" % chan
                chan_node = eval('self.input_%2.2d' % (chan+1,))
                chan_raw_node = eval('self.input_%2.2d_raw' % (chan+1,))
                if chan_node.on :
                    if debug:
                        print "it is on so ..."
                    if mask[chan:chan+1] == '1' :
                        try:
			    start =  eval("int(self.input_%2.2d_start_idx)"%(chan+1))
                        except:
                            start = pre
                        try:
			    end =  eval("int(self.input_%2.2d_end_idx"%(chan+1))
                        except:
                            end = post
                        try:
			    inc =  eval("int(self.input_%2.2d_inc)"%(chan+1))
                        except:
                            inc = 1
                        if debug:
                            print "build the command"
                        command = "mdsPutCh --field %s:raw --expr %%calsig --timebase %d,%d,%d %d" % (chan_node.getFullPath(), int(start), int(end), int(inc), chan+1)
                        command = command.replace('\\','\\\\')
                        if debug:
                            print "about to execute %s" % command
                        status = UUT.uut.acq2sh(command)
                        if debug:
			    print "    ...returned %s" % status
#                        if status.startswith('mdsPutChannel ERROR') :
			if status != command :
			    raise Exception('DT216B channel %d'%(chan+1), status)
                        if inc > 1 :
                            clk=None
                            delta=None
                            begin=None
                            ending=None
                            try :
                                clk = self.clock.evaluate()
                                delta = clk.delta
                                begin = clk.begin
                                ending = clk.ending
                            except:
                                pass
                            if delta :
                                axis = Range(begin, ending, delta*inc)
                                window = Window(start/inc, end/inc, self.trig_src)
                                dim = Dimension(window, axis)
                            else:
                                dim = Data.compile('Map($,$)', Dimension(Window(start/inc, end/inc, trig_src), clock), Range(start, end, inc))
                            raw = Data.compile('data($)', chan_raw_node)
                            chan_node.record = eval('Signal(raw, "", dim)')
                        else:
                            raw = Data.compile('data($)', chan_raw_node)
                            chan_node.record = Signal(raw, "", Dimension(Window(start, end, self.trig_src), clock))
            UUT.uut.acq2sh('mdsClose %s' % (self.boardip.tree.name,))
        except Exception,e :
            print "Error storing DT216B Device\n%s" % ( str(e), )
            return 0
                
        return 1

    STORE=store

    def help(self, arg):
        """ Help method to describe the methods and nodes of the DT216B module type """
        help(DT216B)
        return 1

