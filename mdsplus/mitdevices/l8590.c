#include <stdlib.h>
#include <mdsdescrip.h>
#include <mds_gendevice.h>
#include <mitdevices_msg.h>
#include <mds_stdarg.h>
#include <treeshr.h>
#include <stdio.h>
#include "l8590_gen.h"

extern unsigned short OpcValue;

static int one=1;
static int zero=0;

#define return_on_error(f,retstatus) if (!((status = f) & 1)) return retstatus;
#define pio(name,f,a,d) return_on_error(DevCamChk(CamPiow(name,a,f,d,16,0),&one,0),status);
#define stop(name,f,a,cnt,d) return_on_error(DevCamChk(CamStopw(name,a,f,cnt,d,16,0),&one,0),status);
#define return_on_error(f,retstatus) if (!((status = f) & 1)) return retstatus;
#define min(a,b) ((a) <= (b)) ? (a) : (b)
#define max(a,b) ((a) >= (b)) ? (a) : (b)

int l8590___init(struct descriptor_s *niddsc_ptr, InInitStruct *setup)
{
  int status;
  pio(setup->name,9,0,0);
  pio(setup->name,10,0,0);
  pio(setup->name,18,0,&setup->num_active_convert);
  pio(setup->name,26,0,0);
  pio(setup->mem_cts_name,9,0,0);
  pio(setup->mem_cts_name,25,0,0);
  pio(setup->mem_cts_name,11,0,0);
  return status;
}

static int ReadSetup(struct descriptor *key_ptr,
                     int                   *mem_ptr,
                     char		   *head_ptr,
                     int		   *samples_ptr,
                     int                   *min_idx_ptr,
                     int		   *max_idx_ptr,
                     float		   *freq_ptr,
                     int		   *noc_ptr);
static int ReadChannel(struct descriptor_s *key_d_ptr,
                int			*max_samps_ptr,
                int                     *chan_ptr,
                int			*samples_ptr,
                short			*data_ptr);


int l8590___store(struct descriptor_s *niddsc_ptr, InStoreStruct *setup)
{
  int samples = 0;
  int status;
  static DESCRIPTOR_A_BOUNDS(raw,sizeof(unsigned short),DTYPE_WU,0,1,0);
  static DESCRIPTOR(counts_str,"counts");
  static DESCRIPTOR_WITH_UNITS(counts,&raw,&counts_str);
  static int latch_nid;
  static DESCRIPTOR_NID(latch,&latch_nid);
  static FUNCTION(1) value = {2,DTYPE_FUNCTION,CLASS_R,(unsigned char *)&OpcValue,0,0};
  static DESCRIPTOR_SIGNAL_1(signal,&value,&counts,&latch);
  latch_nid = setup->head_nid + L8590_N_LATCH;
  pio(setup->name,2,0,&samples);
  if (samples > 32766)
  {
    printf("Got large sample count from L8590: %s --- %d\n",setup->name,samples);
    samples=32766;
  }
  if (samples)
  {
    int samps_per_chan = samples/setup->num_active;
    unsigned short *values = calloc(samples * 2,sizeof(*values));
    unsigned short *cdata = values + samples;
    int chan;
    int i;
    int min_idx = 0;
    int max_idx = samps_per_chan - 1;
    pio(setup->mem_cts_name,19,0,&zero);
    pio(setup->mem_cts_name,18,0,&zero);
    pio(setup->mem_cts_name,25,0,0);
    pio(setup->mem_cts_name,2,0,&values[0]);
    stop(setup->mem_cts_name,2,0,samples,values);
    for (i=0;i<samples;i++) cdata[i/setup->num_active + (i % setup->num_active) * samps_per_chan] = values[i];
    for (chan=0;chan<setup->num_active;chan++)
    {
      int data_nid = setup->head_nid+L8590_N_INPUT_1+(L8590_N_INPUT_2 - L8590_N_INPUT_1)*chan;
      int start_nid = data_nid + L8590_N_INPUT_1_STARTIDX - L8590_N_INPUT_1;
      int end_nid = data_nid + L8590_N_INPUT_1_ENDIDX - L8590_N_INPUT_1;
      if (TreeIsOn(data_nid) & 1) {
        status = DevLong(&start_nid,(int *)&raw.bounds[0].l);
        if (status&1) raw.bounds[0].l = min(max_idx,max(min_idx,raw.bounds[0].l));
        else raw.bounds[0].l = min_idx;

        status = DevLong(&end_nid, (int *)&raw.bounds[0].u);
        if (status&1) raw.bounds[0].u = min(max_idx,max(min_idx,raw.bounds[0].u));
        else raw.bounds[0].u = max_idx;
        raw.m[0] = raw.bounds[0].u - raw.bounds[0].l + 1;
        if (raw.m[0] > 0)
        {
          raw.pointer = (char *)(cdata + chan * samps_per_chan + raw.bounds[0].l);
          raw.a0 = raw.pointer - raw.bounds[0].l * sizeof(*cdata);
          raw.arsize = raw.m[0] * 2;
          status = TreePutRecord(data_nid,(struct descriptor *)&signal,0);
        }
      }
    }
    free(values);
  }
  return status;
}
