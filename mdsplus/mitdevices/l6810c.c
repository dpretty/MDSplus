#include <mdsdescrip.h>
#include <mds_gendevice.h>
#include <mitdevices_msg.h>
#include <mds_stdarg.h>
#include <stdio.h>
#include "l6810c.h"
extern int CamXandQ();

#define min(a,b) ((a) < (b)) ? (a) : (b)
#define max(a,b) ((a) < (b)) ? (b) : (a)
#define return_on_error(f) if (!((status = f) & 1)) return status;
static int one=1;
static int zero=0;

#define pio(f,a,d, q)  return_on_error(DevCamChk(CamPiow(in_struct->name, a, f, d, 16, 0), &one, 0))
#define stop(f,a,n,d)  return_on_error(DevCamChk(CamStopw(in_struct->name, a, f, n, d, 16, 0), &one, 0))

#define wait                 pio(11,0,0,1); for (tries=0; (!(CamXandQ(0) & 1)) && (tries < 100);tries++) {pio(11,0,0,1);}

struct setup {
  short int t_s_res ;
  short int sensitivity[4];
  short int block_len ;
  short int num_blk_lsb ;
  short int num_blk_msb ;
  short int trigger_holdoff ;
  short int trigger_slope ;
  short int trigger_coupling ;
  short int trig_up ;
  short int trig_lo ;
  short int trig_src ;
  short int near_low ;
  short int near_hi ;
  short int active_chan ;
  short int c1_off ;
  short int c2_off ;
  short int c3_off ;
  short int c4_off ;
  short int c1_src ;
  short int c2_src ;
  short int c3_src ;
  short int c4_src ;
  short int trigger_delay ;
  short int samps_per_seg ;
  short int num_seg_lo ;
  short int num_seg_hi ;
  short int dual_timebase ;
  short int f1_freq ;
  short int f2_freq ;
  short int mem_size ;
};


int l6810c___init(struct descriptor *nid_d_ptr, InInitStruct *in_struct)
{
  int status;
  int dummy;
  int tries;

  struct setup setup;
  setup.t_s_res = 0;
  setup.sensitivity[0] = in_struct->input_01_full_scale_convert;
  setup.sensitivity[1] = in_struct->input_02_full_scale_convert;
  setup.sensitivity[2] = in_struct->input_03_full_scale_convert;
  setup.sensitivity[3] = in_struct->input_04_full_scale_convert;
  setup.block_len = 0;
  setup.num_blk_lsb = 0;
  setup.num_blk_msb = 0;
  setup.trigger_holdoff = 1;
  setup.trigger_slope = 0; /* positive slope */
  setup.trigger_coupling = 0; /* DC coupling */
  setup.trig_up = 128+(2.4*128/5);  /* 2.5 volts */
  setup.trig_lo = 255;
  setup.trig_src = 0; /* external trigger */
  setup.near_low = 254;
  setup.near_hi = 255;
  setup.active_chan = in_struct->active_chans;
  setup.c1_off = in_struct->input_01_offset;
  setup.c2_off = in_struct->input_02_offset;
  setup.c3_off = in_struct->input_03_offset;
  setup.c4_off = in_struct->input_04_offset;
  setup.c1_src = in_struct->input_01_src_cpling;
  setup.c2_src = in_struct->input_02_src_cpling;
  setup.c3_src = in_struct->input_03_src_cpling;
  setup.c4_src = in_struct->input_04_src_cpling;
  setup.trigger_delay = in_struct->trigger_dly;
  setup.samps_per_seg = in_struct->active_mem_convert;
  setup.num_seg_lo = in_struct->segments % 256;
  setup.num_seg_hi = in_struct->segments / 256;;
  setup.dual_timebase = 0;  /* single time base */
  setup.f1_freq = in_struct->freq_convert;
  setup.f2_freq = 12;  /* actually not used */
  setup.mem_size = in_struct->memories;

/* Write setup to module */

  pio(25,1,0, 1);
  pio(2,0,&dummy, 1);
  wait;
  pio(18,0,&zero, 1);
  stop(19,1,33,&setup);

/* enable lams */

  pio(26,0,0, 1);

/* verify setup (wait for completion) */

  pio(18,6,&zero, 1);
  wait;

/* Arm module */

  pio(9,0,0, 1);

  return 1;
}

int l6810c___trigger(struct descriptor *nid_d_ptr, InTriggerStruct *in_struct)
{
  int status;
  pio(25, 0, 0, 1); /* Trigger the module */
  return 1;
}

#define L6810C_K_NODES_PER_CHANNEL 6
#define L6810C_N_CHANNELS L6810C_N_INPUT_01
#define L6810C_N_CHAN_HEAD 0
#define L6810C_N_CHAN_STARTIDX 1
#define L6810C_N_CHAN_ENDIDX 2
#define L6810C_N_CHAN_GAINIDX 3
#define L6810C_N_CHAN_SRC_CPLING 5
#define L6810C_N_CHAN_OFFSET 5

extern unsigned short OpcAdd;
extern unsigned short OpcMultiply;
extern unsigned short OpcValue;


static int ReadChannel(char *name, 
                       int chan, 
                       int segs, 
                       int seg_size, 
                       int drop, 
                       int *samples_ptr, 
                       short *data_ptr);

int l6810c___store(struct descriptor *niddsc_ptr, InStoreStruct *in_struct)
{
#undef return_on_error
#define return_on_error(f) if (!((status = f) & 1)) return status;
#undef pio
#define pio(f,a,d,q)  return_on_error(DevCamChk(CamPiow(in_struct->name, a, f, d, 16, 0), &one, &q))
#define CHAN_NID(chan, field) c_nids[L6810C_N_CHANNELS+chan*L6810C_K_NODES_PER_CHANNEL+field]

  static unsigned long one = 1;
  static int c_nids[L6810C_K_CONG_NODES];

  static int end;
  static DESCRIPTOR_A_BOUNDS(raw,sizeof(short),DTYPE_W,0,1,0);
  static DESCRIPTOR(counts_str,"counts");
  static DESCRIPTOR_WITH_UNITS(counts,&raw,&counts_str);
  static DESCRIPTOR_LONG(one_d,&one);
  static DESCRIPTOR_LONG(start_d,&raw.bounds[0].l);
  static DESCRIPTOR_FUNCTION_2(start_expr_d,(unsigned char *)&OpcAdd,&start_d,&one_d);
  static DESCRIPTOR_LONG(end_d,&end);
  static DESCRIPTOR_FUNCTION_2(end_expr_d,(unsigned char *)&OpcAdd,&end_d,&one_d);
  static DESCRIPTOR_NID(trigger_d,&c_nids[L6810C_N_STOP_TRIG]);
  static float frequency = 4E-6;
  static DESCRIPTOR_FLOAT(frequency_d,&frequency);
  static DESCRIPTOR_RANGE(int_clock_d,0,0,&frequency_d);
  static DESCRIPTOR_NID(ext_clock_d,&c_nids[L6810C_N_EXT_CLOCK_IN]);
  static float coeffs[] = {100E-6, 250E-6, 500E-6, 1E-3, 2.5E-3, 6.26E-3, 12.5E-3, 25E-3};
  static float coefficient;
  static DESCRIPTOR_FLOAT(coef_d,&coefficient);
  static short offset = -2048;
  static struct descriptor_s  offset_d    = {2,DTYPE_W,CLASS_S,(char *)&offset};
  static DESCRIPTOR_FUNCTION_1(value,(unsigned char *)&OpcValue,0);
  static DESCRIPTOR_FUNCTION_2(add_exp,(unsigned char *)&OpcAdd,&offset_d,&value);
  static DESCRIPTOR_FUNCTION_2(mult_exp,(unsigned char *)&OpcMultiply,&coef_d,&add_exp);
  static DESCRIPTOR(volts_str,"volts");
  static DESCRIPTOR_WITH_UNITS(volts,&mult_exp,&volts_str);
  static DESCRIPTOR_WINDOW(window,&start_expr_d,&end_expr_d,&trigger_d);
  static EMPTYXD(dimension_xd);
  static DESCRIPTOR(time_str,"seconds");
  static DESCRIPTOR_WITH_UNITS(time,&dimension_xd,&time_str);
  static DESCRIPTOR_SIGNAL_1(signal,&volts,&counts,&time);
  short *channel_data;
  int status;
  int chan;
  int samples_to_read;
  int i;
  static int drop;
  static DESCRIPTOR_LONG(drop_d, &drop);  
  static int begin;
  static DESCRIPTOR_LONG(begin_d, &begin);
  int min_idx;
  int max_idx;
  int delay_samples;
  int samples_per_segment;
  int samples_per_channel;
  int segs;
  struct setup setup;

  return_on_error(DevNids(niddsc_ptr,sizeof(c_nids),c_nids));
  value.ndesc = 0;
  return_on_error(DevCamChk(CamPiow(in_struct->name, 0, 8, 0, 16, 0), &one, 0));
  if ((CamXandQ(0)&1) == 0) return DEV$_NOT_TRIGGERED;

  pio(18, 0, &zero, one); /* prepare to read setup information */
  stop(2,1,33,&setup); /* read the setup information */
  if (setup.f1_freq != 0)
  {
    static float freqs[] = {0.0, 1/20., 1/50., 1/100., 1/200., 1/500., 1/1000., 
                            1/2000., 1/5000., 1/10000., 1/20000., 1/50000.,
                            1/100000., 1/200000., 1/500000., 1/1000000., 1/2000000., 1/5000000.};
    frequency = freqs[setup.f1_freq];
  }
  setup.trigger_delay = (setup.trigger_delay <=247) ? setup.trigger_delay : setup.trigger_delay - 248 - 8; 
  segs = setup.num_seg_lo+setup.num_seg_hi*256;
  drop = (setup.active_chan==4) ? 2 : ((setup.active_chan==2) ? 5 : 10);
  samples_per_segment = (1<<setup.samps_per_seg)*1024 - drop;
  samples_per_channel = samples_per_segment*segs;
  delay_samples = samples_per_segment/8*setup.trigger_delay;
  min_idx = 0;
  max_idx = samples_per_channel - 2;
/*
  min_idx = delay_samples;
  max_idx = min_idx+samples_per_segment;
*/
  channel_data = (short *)malloc(samples_per_channel*sizeof(short));

#undef return_on_error
#define return_on_error(f) if (!((status = f) & 1)) {free(channel_data); return status;}

  for (chan=0;((chan < setup.active_chan) && (status & 1));chan++)
  {
    if (TreeIsOn(CHAN_NID(chan,L6810C_N_CHAN_HEAD)) & 1)
    {
      status = DevLong(&CHAN_NID(chan,L6810C_N_CHAN_STARTIDX),(int *)&raw.bounds[0].l);
      if (status&1) raw.bounds[0].l = min(max_idx,max(min_idx,raw.bounds[0].l));
      else raw.bounds[0].l = min_idx;

      status = DevLong(&CHAN_NID(chan,L6810C_N_CHAN_ENDIDX), (int *)&raw.bounds[0].u);
      if (status&1) raw.bounds[0].u = min(max_idx,max(raw.bounds[0].l,raw.bounds[0].u));
      else raw.bounds[0].u = max_idx;
 
      raw.m[0] = raw.bounds[0].u - raw.bounds[0].l + 1;
      if (raw.m[0] > 0)
      {
        samples_to_read = raw.bounds[0].u - min_idx + 1;
        status = ReadChannel(in_struct->name,chan, segs, samples_per_segment, drop, &samples_to_read, channel_data);
        if (status & 1)     
        {
          short *optr;
	  coefficient = coeffs[setup.sensitivity[chan]];
          raw.pointer = (char *)(channel_data + (raw.bounds[0].l - min_idx));
          raw.a0 = raw.pointer - (raw.bounds[0].l * sizeof(channel_data[0]));
          raw.arsize = raw.m[0] * 2;
          optr = &setup.c1_off+chan;
          offset = *optr * -16;
          if (segs != 1) {
            static char dim_str[1024*50];
            static struct descriptor dim_expr={0, DTYPE_T, CLASS_S, (char *)dim_str};
            static int len;
            int smps;
            static struct descriptor len_dsc = {sizeof(int), DTYPE_L, CLASS_S, (char *)&len};
            smps = (1<<setup.samps_per_seg)*1024-1;
            end = smps + smps/8*setup.trigger_delay;
            begin = end-smps+drop;
            len = raw.m[0];
            strcpy(dim_str,"SET_RANGE(0 : $1, [");
            for (i=0; i<segs; i++) {
              char chunk[1000];
              sprintf(chunk, "build_dim(build_window($2, $3, $4[%d]), $5) %s", i, (i != (segs-1)) ? "," : "])");
              strcat(dim_str, chunk);
            }
            dim_expr.length = strlen(dim_str);
            status = TdiCompile(&dim_expr, &len_dsc, &begin_d, &end_d, &trigger_d, 
                                 (setup.f1_freq == 0) ? (struct descriptor *)&ext_clock_d : 
                                                        (struct descriptor *)&int_clock_d, 
                                 &dimension_xd MDS_END_ARG);
          }
          else {
            static DESCRIPTOR(dim_expr, "BUILD_DIM(BUILD_WINDOW($1, $2, $3), $4)");
            end = max_idx;
            begin = min_idx;
            status = TdiCompile(&dim_expr, &begin_d, &end_d, &trigger_d, 
                                 (setup.f1_freq == 0) ? (struct descriptor *)&ext_clock_d : 
                                                        (struct descriptor *)&int_clock_d, 
                                 &dimension_xd MDS_END_ARG);
          }
          if (status&1)
            status = TreePutRecord(CHAN_NID(chan,L6810C_N_CHAN_HEAD),(struct descriptor *)&signal,0);
        }
      }
    }
  }
  free(channel_data);
  return status;
}

#undef return_on_error
#define return_on_error(f) if (!((status = f) & 1)) return status;
#undef pio
#define pio(f,a,d,q)  return_on_error(DevCamChk(CamPiow(name, a, f, d, 16, 0), &one, 0))
static int ReadChannel(char *name, int chan, int segs, int seg_size, int drop, int *samples_ptr, short *data_ptr)
{
  int i;
  int tries;
  int seg;
  int status;
  int samples_read = 0;
  int samples_to_read = *samples_ptr;
  int read_size;
  short int dummy;
  pio(16,5,&zero,1);
  pio(16,6,&zero,1);
  pio(16,7,&zero,1);
  for (seg=0; seg<segs; seg++) {
    short int a = chan+1;
    pio(18,a, &seg, 1);
    wait;
    for (i = 0; i<drop; i++)
      pio(2,0,&dummy,1);       /* throw one out for good measure */
    for(samples_to_read = seg_size; samples_to_read; samples_to_read -= read_size)
    {
      read_size = min(32767, samples_to_read);
      return_on_error(DevCamChk(CamQstopw(name,0,2,read_size,data_ptr+samples_read,16,0),&one,0));
      samples_read += read_size;
    }
  }
  pio(25,1,0,1);      /* abort the read of this channel */
  pio(2,0,&dummy,0);       /* throw one out for good measure */
  return 1;
}
