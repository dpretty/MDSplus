#ifndef HAVE_VXWORKS_H
#include <config.h>
#endif
#ifdef _WIN32
#include <io.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <mdsdescrip.h>
#include <mdsshr.h>
#include <ncidef.h>
#include "treeshrp.h"
#include "treethreadsafe.h"
#include <treeshr.h>
#include <librtl_messages.h>
#include <errno.h>
#include <fcntl.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif
#ifndef O_RANDOM
#define O_RANDOM 0
#endif

#define align(bytes,size) ((((bytes) + (size) - 1)/(size)) * (size))

static char *cvsrev = "@(#)$RCSfile: TreeGetRecord.c,v $ $Revision: 1.52.2.6 $ $Date: 2010/11/19 22:12:42 $";

static _int64 ViewDate = -1;
static int MakeNidsLocal(struct descriptor *dsc_ptr, unsigned char tree);

int MdsSerializeDscIn(char *in, struct descriptor_xd *out_dsc_ptr);


extern void **TreeCtx();



#ifndef HAVE_VXWORKS_H
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif




int TreeGetRecord(int nid_in, struct descriptor_xd *dsc) {return _TreeGetRecord(*TreeCtx(),nid_in,dsc);}



int _TreeGetRecord(void *dbid, int nid_in, struct descriptor_xd *dsc)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid = (NID *)&nid_in;
  struct descriptor *dptr;
  int       status;
  NCI nci;
  TREE_INFO *info;
  int       nidx;
  ssize_t       retsize;
  int       nodenum;
#ifdef BIG_DESC
  EMPTYXD(tmp_dsc);
  struct descriptor_xd *orig_dsc=0;
  MdsFree1Dx(dsc,NULL);
  if (dsc->class == CLASS_XD_SHORT) {
    orig_dsc=dsc;
    dsc=&tmp_dsc;
  }
#else
  MdsFree1Dx(dsc,NULL);
#endif
  if (!(IS_OPEN(dblist)))
    return TreeNOT_OPEN;

  if (dblist->remote)
    return GetRecordRemote(dblist, nid_in, dsc);
  nid_to_tree_nidx(dblist, nid, info, nidx);
  if (info)
  {
    status = TreeCallHook(GetNci,info,nid_in);
    if (status && !(status & 1))
      return 0;
    if (!info->data_file)
      status = TreeOpenDatafileR(info);
    else
      status = 1;
    if (status & 1)
    {
      status = TreeGetNciW(info, nidx, &nci,0);
      if (status & 1)
      {
        descriptor_llength len=(descriptor_llength)nci.length + ((nci.flags2 & NciM_BIG_RECORD) ? (((descriptor_llength)nci.length_hi)<<32) : 0);
	if (len)
	{
	  status = TreeCallHook(GetData,info,nid_in);
	  if (status & !(status & 1))
	    return 0;
	  switch (nci.class)
	  {
	   case CLASS_APD:
	   case CLASS_CA:
	   case CLASS_R:
	   case CLASS_A: if (nci.flags2 & NciM_DATA_IN_ATT_BLOCK)
                         {
	                   status = TreeINVDFFCLASS;
                           break;
                         }
	   case CLASS_S:
	   case CLASS_XS:
	     if (status & 1) {
	       if (nci.flags2 & NciM_EXTENDED_NCI) {
		 EXTENDED_ATTRIBUTES attributes;
		 status = TreeGetExtendedAttributes(info,RfaToSeek(nci.DATA_INFO.DATA_LOCATION.rfa),&attributes);
		 if (status & 1 && attributes.facility_offset[STANDARD_RECORD_FACILITY] != -1) {
		   status = TreeGetDsc(info,attributes.facility_offset[STANDARD_RECORD_FACILITY],
				       attributes.facility_length[STANDARD_RECORD_FACILITY],dsc);
		 } else if ( status & 1 && attributes.facility_offset[SEGMENTED_RECORD_FACILITY] != -1) {
		   status = _TreeGetSegmentedRecord(dbid,nid_in,dsc);
		 } else
		   status = TreeBADRECORD;
	       }
	       else {
		 if (nci.flags2 & NciM_DATA_IN_ATT_BLOCK)
		   {
		     unsigned char dsc_dtype = DTYPE_DSC;
		     descriptor_llength dlen = nci.length - 8;
		     descriptor_llength ddlen = dlen + sizeof(struct descriptor);
		     status = MdsGet1Dx(&ddlen, &dsc_dtype, dsc,0);
		     dptr = dsc->pointer;
		     dptr->length = dlen;
		     dptr->dtype = nci.dtype;
		     dptr->class = CLASS_S;
		     dptr->pointer = (char *) dptr + sizeof(struct descriptor);
		     memcpy(dptr->pointer,nci.DATA_INFO.DATA_IN_RECORD.data,dptr->length);
		     if (dptr->dtype != DTYPE_T)
		       {
			 switch (dptr->length)
			   {
			   case 2: *(short *)dptr->pointer = swapshort(dptr->pointer); break;
			   case 4: *(int *)dptr->pointer = swapint(dptr->pointer); break;
			   case 8: *(_int64 *)dptr->pointer = swapquad(dptr->pointer); break;
			   }
		       }
		   }
		 else
		   {
		     size_t length = (size_t)nci.DATA_INFO.DATA_LOCATION.record_length + ((nci.flags2 & NciM_BIG_RECORD) ? (((size_t)nci.DATA_INFO.DATA_LOCATION.record_length_hi)<<32) : 0);
		     if (length > 0)
		       {
			 char *data = malloc(length);
			 status = TreeGetDatafile(info, nci.DATA_INFO.DATA_LOCATION.rfa,&length,data,&retsize,&nodenum,nci.flags2);
			 if (!(status & 1))
			   status = TreeBADRECORD;
			 else if (!(nci.flags2 & NciM_NON_VMS) && ((retsize != length) || (nodenum != nidx)))
			   status = TreeBADRECORD;
			 else
			   status = (MdsSerializeDscIn(data,dsc) & 1) ? TreeNORMAL : TreeBADRECORD;
			 free(data);
		       }
		     else
		       status = TreeBADRECORD;
		   }
	       }
	     }
	       break;
	     default:
	       status = TreeINVDFFCLASS;
	       break;
	  }
	}
	else
	  status = TreeNODATA;
      }
    }
  }
  else
    status = TreeINVTREE;
  if (status & 1)
    status = MakeNidsLocal((struct descriptor *)dsc, (unsigned char)nid->tree);
#ifdef BIG_DESC
  if (orig_dsc) {
    MdsCopyDxXd((struct descriptor *)dsc,orig_dsc);
    MdsFree1Dx(dsc,0);
  }
#endif
  return status;
}

int TreeOpenDatafileR(TREE_INFO *info)
{
  int       status;
  DATA_FILE *df_ptr = info->data_file;
  if (df_ptr)
    return 1;
  df_ptr = TreeGetVmDatafile(info);
  if (df_ptr != NULL)
  {

    size_t len = strlen(info->filespec)-4;
    char *filename = strncpy(malloc(len+9),info->filespec,len);
    filename[len]='\0';
    strcat(filename,"datafile");
    df_ptr->get = MDS_IO_OPEN(filename,O_RDONLY | O_BINARY | O_RANDOM, 0);
    free(filename);
    status = (df_ptr->get == -1) ? TreeFAILURE : TreeNORMAL;
    if (df_ptr->get == -1)
      df_ptr->get = 0;
  }
  info->data_file = df_ptr;
  return status;
}

typedef RECORD(1) record_one;
typedef ARRAY(struct descriptor *) array_dsc;


static int MakeNidsLocal(struct descriptor *dsc_ptr, unsigned char tree)
{
  int       status = 1;
  if (dsc_ptr == NULL)
    status = 1;
  else
    switch (dsc_ptr->class)
    {

     case CLASS_D:
     case CLASS_S:
     case CLASS_XD:
     case CLASS_XS:
      if ((dsc_ptr->dtype != DTYPE_DSC) && (dsc_ptr->dtype != DTYPE_NID))
	status = 1;
      else if (dsc_ptr->dtype == DTYPE_DSC)
	status = MakeNidsLocal((struct descriptor *) dsc_ptr->pointer, tree);
      else if (dsc_ptr->dtype == DTYPE_NID)
      {
	NID      *nid_ptr = (NID *) dsc_ptr->pointer;
	if ((nid_ptr->node == 0) && (nid_ptr->tree == 0))
	  status = 1;
	else
	{
          nid_ptr->node = *(int *)nid_ptr & 0xffffff;
	  nid_ptr->tree = tree;
        }
      }
      break;

     case CLASS_A:
      if ((dsc_ptr->dtype != DTYPE_DSC) && (dsc_ptr->dtype != DTYPE_NID))
	status = 1;
      else
	status = TreeUNSUPARRDTYPE;
      break;

     case CLASS_R:
      {
	record_one *rptr = (record_one *) dsc_ptr;
	int       i;
	for (i = 0, status = 1; status && (i < rptr->ndesc); i++)
	  status = MakeNidsLocal(rptr->dscptrs[i], tree);
      }
      break;

     case CLASS_CA:
      status = 1;
      break;

     case CLASS_APD:
      {
	array_dsc * aptr = (array_dsc *) dsc_ptr;
	int       n_elts = aptr->arsize / aptr->length;
	int       i;
	for (status = 1, i = 0; ((status & 1) && (i < n_elts)); i++)
	  status = MakeNidsLocal((struct descriptor *) * (aptr->pointer + i), tree);
      }
      break;

     default:
      status = LibINVSTRDES;
    }

  return status;
}

DATA_FILE *TreeGetVmDatafile()
{
  DATA_FILE *datafile_ptr;
  static const struct descriptor_xd empty_xd = {0, 0, CLASS_XD, 0, 0};
  int length = align(sizeof(DATA_FILE),sizeof(void *)) + 
               align(sizeof(RECORD_HEADER),sizeof(void *)) + 
               align(sizeof(ASY_NCI),sizeof(void *)) + 
               align(sizeof(NCI),sizeof(void *)) + 
               align(sizeof(struct descriptor_xd),sizeof(void *));
  datafile_ptr = malloc(length * 2);
  if (datafile_ptr != NULL)
  {
    char *ptr = (char *)datafile_ptr;
    memset(datafile_ptr,0,length);
    datafile_ptr->record_header = (RECORD_HEADER *) (ptr += align(sizeof(DATA_FILE),sizeof(void *)));
    datafile_ptr->asy_nci = (ASY_NCI *) (ptr += align(sizeof(RECORD_HEADER),sizeof(void *)));
    datafile_ptr->data = (struct descriptor_xd *) (ptr += align(sizeof(ASY_NCI),sizeof(void *)));
    *datafile_ptr->data = empty_xd;
    datafile_ptr->asy_nci->nci = (NCI *) (ptr += align(sizeof(struct descriptor_xd),sizeof(void *)));
  }
  return datafile_ptr;
}

int TreeGetDatafile(TREE_INFO *info, unsigned char *rfa_in, size_t *buffer_size, char *record, ssize_t *retsize,int *nodenum,unsigned char flags)
{
  int status = 1;
  size_t buffer_space = *buffer_size;
  int       first = 1;
  unsigned char rfa[6];
  char *bptr = (char *)record;
  *retsize = 0;
  memcpy(rfa,rfa_in,sizeof(rfa));
  if (!(flags & NciM_DATA_CONTIGUOUS))
  {
    while ((rfa[0] || rfa[1] || rfa[2] || rfa[3] || rfa[4] || rfa[5]) && buffer_space && (status & 1))
    {
      RECORD_HEADER hdr;
      _int64 rfa_l = RfaToSeek(rfa);
      int deleted=1;
      while (status & 1 && deleted) {
        status = (MDS_IO_READ_X(info->data_file->get,rfa_l,(void *)&hdr,12,&deleted) == 12) ? TreeSUCCESS : TreeFAILURE;
        if (status & 1 && deleted)
	  status = TreeReopenDatafile(info);
      }
      if (status & 1) {
	unsigned int partlen = min(swapshort((char *)&hdr.rlength)-10, buffer_space);
	int nidx = swapint((char *)&hdr.node_number);
	if (first)
	  *nodenum = nidx;
	else if (*nodenum != nidx) {
	  status = 0;
	  break;
	}
        deleted=1;
        while (status & 1 && deleted) {
	  status = ((unsigned int)MDS_IO_READ_X(info->data_file->get,rfa_l+12,(void *)bptr,partlen,&deleted) == partlen) ? TreeSUCCESS : TreeFAILURE;
          if (status & 1 && deleted)
	    status = TreeReopenDatafile(info);
        }
	if (status & 1) {
	  bptr += partlen;
	  buffer_space -= partlen;
	  *retsize = *retsize + partlen;
	  memcpy(rfa,&hdr.rfa,sizeof(rfa));
	}
      }
    }
  }
  else
  {
    if (flags & NciM_NON_VMS)
    {
      _int64 rfa_l = RfaToSeek(rfa);
      int deleted=1;
      while (status & 1 && deleted) {
        status = (MDS_IO_READ_X(info->data_file->get,rfa_l,(void *)record,*buffer_size,&deleted) == *buffer_size) ? TreeSUCCESS : TreeFAILURE;
        if (status & 1 && deleted) 
	  status = TreeReopenDatafile(info);
      }
      *retsize = *buffer_size;
    }
    else
    {
      size_t       blen = *buffer_size + (*buffer_size + DATAF_C_MAX_RECORD_SIZE + 1)/(DATAF_C_MAX_RECORD_SIZE + 2)*sizeof(RECORD_HEADER);
      char     *buffer = (char *)malloc(blen);
      char     *bptr_in;
      size_t      bytes_remaining;
      unsigned int      partlen = (blen % (DATAF_C_MAX_RECORD_SIZE + 2 + sizeof(RECORD_HEADER)));
      _int64 rfa_l = RfaToSeek(rfa);
      int deleted=1;
      rfa_l -= blen - (partlen ? partlen : (DATAF_C_MAX_RECORD_SIZE + 2 + sizeof(RECORD_HEADER)));
      while (status & 1 && deleted) {
        status = (MDS_IO_READ_X(info->data_file->get,rfa_l,(void *)buffer,blen,&deleted) == blen) ? TreeSUCCESS : TreeFAILURE;
        if (status & 1 && deleted)
          status = TreeReopenDatafile(info);
      }
      *nodenum = swapint((char *)&((RECORD_HEADER *)buffer)->node_number);
      for (bptr_in = buffer, bptr = record + *buffer_size, bytes_remaining=*buffer_size; bytes_remaining;)
      {
        int bytes_this_time = min(DATAF_C_MAX_RECORD_SIZE + 2, bytes_remaining);
        bptr_in += sizeof(RECORD_HEADER);
        memcpy(bptr-bytes_this_time,bptr_in,bytes_this_time);
        bptr_in += bytes_this_time;
        bptr -= bytes_this_time;
        bytes_remaining -= bytes_this_time;
      }
      free(buffer);
      *retsize = *buffer_size;
    }
  }
  return status;
}

int TreeSetViewDate(_int64 *date)
{ 
  if (TreeGetThreadStatic()->privateCtx)
    TreeGetThreadStatic()->ViewDate=*date;
  else
    ViewDate=*date;
  return TreeSUCCESS;
}

int TreeGetViewDate(_int64 *date)
{
  if (TreeGetThreadStatic()->privateCtx)
    *date=TreeGetThreadStatic()->ViewDate;
  else
    *date= ViewDate;
  return TreeSUCCESS;
}

int TreeGetVersionNci(TREE_INFO *info, NCI *nci, NCI *v_nci)
{
  char nci_bytes[42];
  int status;
  if (!info->data_file)
    status = TreeOpenDatafileR(info);
  else
    status = 1;
  if (status & 1) {
    int deleted=1;
    _int64 rfa_l = RfaToSeek(nci->DATA_INFO.DATA_LOCATION.rfa);
    rfa_l+=((nci->DATA_INFO.DATA_LOCATION.record_length == (DATAF_C_MAX_RECORD_SIZE + 2)) ? (DATAF_C_MAX_RECORD_SIZE + 2) : 
       (nci->DATA_INFO.DATA_LOCATION.record_length % (DATAF_C_MAX_RECORD_SIZE + 2)));
    rfa_l+=sizeof(RECORD_HEADER);
    while (status & 1 && deleted) {
      status = (MDS_IO_READ_X(info->data_file->get,rfa_l,(void *)nci_bytes,42,&deleted) == 42) ? TreeSUCCESS : TreeFAILURE;
      if (status & 1 && deleted)
        status = TreeReopenDatafile(info);
    }
    if (status & 1) {
      TreeSerializeNciIn(nci_bytes,v_nci);
    }
  }
  return status;
}  
  
