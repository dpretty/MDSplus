
#undef _VI_FUNC
#undef __NISCOPE_HEADER

//#define _VI_FUNC

#include <ansi_c.h>
#include <visatype.h>
#include <stdio.h>
#include <cvirte.h>
#include <math.h>
#include <winsock.h>
#include "toolbox.h"
#include <mdslib.h>
#include <niScope.h>
#include <ivi.h>
#include <iviScope.h>
#include "niScopeObsolete.h" 

// id = ih    ���� ���̴�. ���⼭�� ih�� ��� �ߴ�.
// NiScope_init  ->  NiScope_close
int NiScope_init (char* dev, ViSession* ih)
{
//	printf("%s\n", dev);
//	ViSession ih;
	ViStatus status;
    status=niScope_init (dev, 1, 1, ih);
//	printf("ID %d\n", ih);
    return status;
}

// NiScope_initiateacquisition  -> NiScope_acquisitionstatus
int NiScope_initiateacquisition (ViSession ih)
{
	 ViStatus status;
     status=niScope_InitiateAcquisition(ih);
	 return status;
}

// NiScope_initiateacquisition  -> NiScope_acquisitionstatus
int NiScope_acquisitionstatus(ViSession ih, ViInt32* as)
{ 
     ViStatus status;
     status=niScope_AcquisitionStatus (ih, as);
     return status;
}


/* Device Setup */
int NiScope_configuretriggerimmediate(ViSession ih)
{
     ViStatus status;
     status=niScope_ConfigureTriggerImmediate (ih);
     return status;
}

int NiScope_ConfigureChanCharacteristics(ViSession ih, char* dev, ViReal64 *impe)
{
     ViStatus status;
     status=niScope_ConfigureChanCharacteristics (ih, dev, *impe, 0.00);
//	 printf("ID %d\n", impe);
	 return status;
}

int NiScope_ConfigureVertical (ViSession ih, char* dev, ViReal64 *range, ViReal64 *offset, ViInt32 coupl,ViReal64 *pa)
{
	 //ViBoolean Enable(channel enable true,false)==value -> IVI_ATTR_BASE(1000000),IVI_CLASS_PUBLIC_ATTR_BASE  (IVI_ATTR_BASE + 250000)
	// IVISCOPE_ATTR_CHANNEL_ENABLED  (IVI_CLASS_PUBLIC_ATTR_BASE  + 5L)   ==>total value =1000000+250000+5L = 1250005
     ViStatus status;
	 status=niScope_ConfigureVertical(ih,dev,*range,*offset,coupl,*pa,NISCOPE_VAL_TRUE);
	 return status;
}
/*  
#define IVISCOPE_VAL_AC                                         (0L)
#define IVISCOPE_VAL_DC                                         (1L)
#define IVISCOPE_VAL_GND                                        (2L)
*/
//���⼭  number record �� ������ ������ �ѹ��ϰ� ���� ������ ������ �����͸� �ް� �ѹ��� ������ �Ұ����� ���Ѵ�. �׷����� ���� �ʿ��ϴ�. 
//�츮�� ���� 1ȸ�� �ѹ� �����͸� �޴´�.
int NiScope_ConfigureHorizontalTiming(ViSession ih, ViReal64 *msr, ViInt32 mup)
{
     ViStatus status;
     status=niScope_ConfigureHorizontalTiming(ih, *msr, mup, 50.0, 1, NISCOPE_VAL_TRUE);
	 return status;
}
/* Trigger */
int NiScope_ConfigureTriggerEdge (ViSession ih,char* ts, ViReal64 *level, ViInt32 slope, ViInt32 tc, ViReal64 *delay)
{
     ViStatus status;
	 status=niScope_ConfigureTriggerEdge(ih, ts, *level, slope, tc, 0.0, *delay);
	 return status;
}

/* commits to hardware all the parameter settings associated with the task.*/
int NiScope_Commit(ViSession ih)
{
     ViStatus status;
	 status=niScope_Commit (ih);
	 return status;
}


/* stored */
//Returns the effective sample rate of the acquired waveform using the current configuration in samples per second.
int NiScope_SampleRate (ViSession ih, ViReal64 *samplerate)
{
   ViStatus status;
   status=niScope_SampleRate (ih, samplerate);
   return status;
}
//Returns the actual number of points the digitizer acquires for each channel.
// After configuring the digitizer for an acquisition, call this function to determine the size of the waveforms that the digitizer acquires. 
//Allocate a ViReal64 array of this size or greater to pass as the waveformArray parameter of niScope_ReadWaveform and niScope_FetchWaveform
int NiScope_ActualRecordLength (ViSession ih, ViPInt32 recordlength)
{
   ViStatus status;
   status=niScope_ActualRecordLength (ih, recordlength);
   return status;
}
//the array length can be determined by calling niScope_ActualNumWfms.
//Allows you to declare appropriately-sized waveforms. NI-SCOPE handles the channel list parsing for you. 
int NiScope_ActualNumWfms (ViSession ih, char* dev, ViPInt32 numwfms)
{
   ViStatus status;
   status=niScope_ActualNumWfms (ih, dev, numwfms);
   return status;
}
//Returns the total available size of an array measurement acquisition.
int NiScope_ActualMeasWfmSize (ViSession ih, ViPInt32 measwaveformsize)
{
   ViStatus status;
   status=niScope_ActualMeasWfmSize (ih, NISCOPE_VAL_MULTIPLY_CHANNELS, measwaveformsize);
   return status;
}


/* ch0 & ch1 �� Gain �� Offset ���� ���� ���� �;� �ϴµ�*/

 int NiScope_Store (ViSession ih, char* dev, ViInt32 *bindata, ViReal64 *gainch, ViReal64 *offsetch) 
// int NiScope_Store (ViSession ih, char* dev, ViReal64 *gainch, ViReal64 *offsetch)
{
	ViInt32 samlen;
	ViStatus status;

	struct niScope_wfmInfo info;

    niScope_ActualRecordLength (ih, &samlen);

//    bindata =(ViInt32 *) malloc (samlen * sizeof (ViInt32));
    status=niScope_FetchBinary32 (ih, dev, 5.0, samlen, bindata, &info);

	*gainch = info.gain;
	*offsetch = info.offset;
	return status;
}


/*   C++ ���� ��ٷ� Tree�� �ִ� �κ��ε� ���� �ϼ� ���� �κ�. �̻��� ����~~~
//        error C2664: 'descr' : cannot convert parameter 1 from 'long *' to 'int *'
//        Types pointed to are unrelated; conversion requires reinterpret_cast, C-style cast or function-style cast


int NiScope_Store (int ih, char *nodeName, char* dev)
{
   int i;
   ViInt32 stats, dtype, idesc1, idesc2, idesc3, idesc4,idesc5,idesc6, null=0;
   char pathName[256];
   ViReal64 srate, voltrange, voltresolution;
   ViInt32 *bindata, *bindata1, samlen, samcount, numsam;
   struct niScope_wfmInfo in;
   struct niScope_wfmInfo info;

   voltresolution = 8912; //NI5122 14bits		 ���⼭�� ������ ���� ���� ������ �Ǵܵȴ�. ��Ʈ��� ���� ���� ��..
   niScope_SampleRate (ih, &srate);
   niScope_ActualRecordLength (ih, &samlen);
   
   // Data acquisition and Data Put tree   /  
	  samcount = samlen -1;        //
	  srate = 1.0/srate;        // sample rate  --> time

	  dtype = DTYPE_DOUBLE;
      idesc4 = descr(&dtype, &srate, &null);   // time

      dtype = DTYPE_ULONGLONG;
	  idesc3 = descr(&dtype, &samcount, &null);   // samples number

   if (dev="0")
	  bindata = malloc (samlen * sizeof (ViInt32));
	  niScope_FetchBinary32 (ih, "0", 5.0, samlen, bindata, &in);

//	voltage= bindata * gain + in.offset 

	  dtype = DTYPE_DOUBLE;
	  idesc1 = descr(&dtype, &in.gain, &null);  // gain   ch 0
      idesc2 = descr(&dtype, bindata, &samlen, &null);  // binary data ch 0
	  idesc5 = descr(&dtype, &in.offset, &null);  //offset  ch 0
     sprintf (pathName, "%s:AI0:FOO", nodeName);
     stats = MdsPut(pathName,"BUILD_SIGNAL(BUILD_WITH_UNITS(($*$VALUE)+$,'V'),(`$[0:$]),MAKE_DIM(MAKE_WINDOW(0,$,0.0),MAKE_SLOPE(MAKE_WITH_UNITS($,'s'))))", &idesc1, &idesc5, &idesc2, &idesc3, &idesc4, &null);          

   else if (dev="1")
	  bindata1 = malloc (samlen * sizeof (ViInt32));
	  niScope_FetchBinary32 (ih, "1", 5.0, samlen, bindata1, &info);

	  dtype = DTYPE_DOUBLE;
	  idesc1 = descr(&dtype, &info.gain, &null);  // gain   ch 0
      idesc2 = descr(&dtype, bindata, &samlen, &null);  // binary data ch 0
	  idesc5 = descr(&dtype, &info.offset, &null);  //offset  ch 0
	 sprintf (pathName, "%s:AI1:FOO", nodeName);
     stats = MdsPut(pathName,"BUILD_SIGNAL(BUILD_WITH_UNITS(($*$VALUE)+$,'V'),(`$[0:$]),MAKE_DIM(MAKE_WINDOW(0,$,0.0),MAKE_SLOPE(MAKE_WITH_UNITS($,'s'))))", &idesc1, &idesc5, &idesc2, &idesc3, &idesc4, &null);          

   else if (dev="0,1")
	  bindata = malloc (samlen * sizeof (ViInt32));
      bindata1 = malloc (samlen * sizeof (ViInt32));
      niScope_FetchBinary32 (ih, "0", 5.0, samlen, bindata, &in);
	  niScope_FetchBinary32 (ih, "1", 5.0, samlen, bindata1, &info);
	  voltdata = malloc (samlen * sizeof (ViReal64));
	  voltdata1 = malloc (samlen * sizeof (ViReal64));

	  dtype = DTYPE_DOUBLE;
	  idesc1 = descr(&dtype, &in.gain, &null);  // gain   ch 0
      idesc2 = descr(&dtype, bindata, &samlen, &null);  // binary data ch 0
	  idesc5 = descr(&dtype, &in.offset, &null);  //offset  ch 0
     sprintf (pathName, "%s:AI0:FOO", nodeName);
     stats = MdsPut(pathName,"BUILD_SIGNAL(BUILD_WITH_UNITS(($*$VALUE)+$,'V'),(`$[0:$]),MAKE_DIM(MAKE_WINDOW(0,$,0.0),MAKE_SLOPE(MAKE_WITH_UNITS($,'s'))))", &idesc1, &idesc5, &idesc2, &idesc3, &idesc4, &null);          
	 
	  dtype = DTYPE_DOUBLE;
	  idesc1 = descr(&dtype, &info.gain, &null);  // gain   ch 0
      idesc2 = descr(&dtype, bindata, &samlen, &null);  // binary data ch 0
	  idesc5 = descr(&dtype, &info.offset, &null);  //offset  ch 0
	 sprintf (pathName, "%s:AI1:FOO", nodeName);
     stats = MdsPut(pathName,"BUILD_SIGNAL(BUILD_WITH_UNITS(($*$VALUE)+$,'V'),(`$[0:$]),MAKE_DIM(MAKE_WINDOW(0,$,0.0),MAKE_SLOPE(MAKE_WITH_UNITS($,'s'))))", &idesc1, &idesc5, &idesc2, &idesc3, &idesc4, &null);          

    free(voltdata);
	free(voltdata1);
  return 1;
}
*/



int NiScope_close(ViSession ih)
{
     ViStatus status;
     status=niScope_close(ih);
     return status;
}

