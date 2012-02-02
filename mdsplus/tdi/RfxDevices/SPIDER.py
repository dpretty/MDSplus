# -*- coding: iso-8859-1 -*-
from MDSplus import *
import time

class SPIDER(Device):
    print 'SPIDER'
    """MARTe configuration"""
    parts=[{'path':':COMMENT', 'type':'text'},
      {'path':':CAMERA_FREQ', 'type':'numeric', 'value':10},
      {'path':':CAMERA_START', 'type':'numeric', 'value':0},
      {'path':':CAMERA_DURAT', 'type':'numeric', 'value':60},
      {'path':':CAEN_FREQ', 'type':'numeric', 'value':2},
      {'path':':CAEN_START', 'type':'numeric', 'value':0},
      {'path':':CAEN_DURAT', 'type':'numeric', 'value':30},
      {'path':':NI_FREQ', 'type':'numeric', 'value':10000},
      {'path':':NI_START', 'type':'numeric', 'value':0},
      {'path':':NI_DURAT', 'type':'numeric', 'value':60},
      {'path':':NI_IN_FREQ', 'type':'numeric', 'value':10},
      {'path':':NI_IN_START', 'type':'numeric', 'value':0},
      {'path':':NI_IN_DURAT', 'type':'numeric', 'value':30},
      {'path':':NI_IN_DCYCLE', 'type':'numeric', 'value':10},
      {'path':'.WAVE_1', 'type':'structure'},
      {'path':'.WAVE_1:WAVE', 'type':'signal'},
      {'path':'.WAVE_1:MIN_X', 'type':'numeric'},
      {'path':'.WAVE_1:MAX_X', 'type':'numeric'},
      {'path':'.WAVE_1:MIN_Y', 'type':'numeric'},
      {'path':'.WAVE_1:MAX_Y', 'type':'numeric'},
      {'path':'.WAVE_2', 'type':'structure'},
      {'path':'.WAVE_2:WAVE', 'type':'signal'},
      {'path':'.WAVE_2:MIN_X', 'type':'numeric'},
      {'path':'.WAVE_2:MAX_X', 'type':'numeric'},
      {'path':'.WAVE_2:MIN_Y', 'type':'numeric'},
      {'path':'.WAVE_2:MAX_Y', 'type':'numeric'},
      {'path':'.WAVE_3', 'type':'structure'},
      {'path':'.WAVE_3:WAVE', 'type':'signal'},
      {'path':'.WAVE_3:MIN_X', 'type':'numeric'},
      {'path':'.WAVE_3:MAX_X', 'type':'numeric'},
      {'path':'.WAVE_3:MIN_Y', 'type':'numeric'},
      {'path':'.WAVE_3:MAX_Y', 'type':'numeric'},
      {'path':'.WAVE_4', 'type':'structure'},
      {'path':'.WAVE_4:WAVE', 'type':'signal'},
      {'path':'.WAVE_4:MIN_X', 'type':'numeric'},
      {'path':'.WAVE_4:MAX_X', 'type':'numeric'},
      {'path':'.WAVE_4:MIN_Y', 'type':'numeric'},
      {'path':'.WAVE_4:MAX_Y', 'type':'numeric'}]
 