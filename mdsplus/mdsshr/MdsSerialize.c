#include <mdsdescrip.h>
#include <mdsshr.h>
#include <librtl_messages.h>
#include <string.h>

#define align(bytes,size) ((((bytes) + (size) - 1)/(size)) * (size))
#define length_of(ptr)    ((unsigned short)swapshort(&ptr[0]))
#define dtype_of(ptr)     (*(unsigned char  *)&ptr[2])
#define class_of(ptr)     (*(unsigned char  *)&ptr[3])
#define pointer_of(ptr)   ((unsigned int)swapint(&ptr[4]))
#define l_length_of(ptr)  ((unsigned int)swapint(&ptr[8]))
#define ndesc_of(ptr)     (*(unsigned char  *)&ptr[8])
#define dscptrs_of(ptr,j) ((unsigned int)swapint(&ptr[12+j*4]))
#define scale_of(ptr)     (*(unsigned char  *)&ptr[8])
#define digits_of(ptr)	  (*(unsigned char  *)&ptr[9])
#define binscale_of(ptr)  ((*((unsigned char  *)&ptr[10]) & 0x08) != 0)
#define redim_of(ptr)     ((*((unsigned char  *)&ptr[10]) & 0x10) != 0)
#define column_of(ptr)    ((*((unsigned char  *)&ptr[10]) & 0x20) != 0)
#define coeff_of(ptr)     ((*((unsigned char  *)&ptr[10]) & 0x40) != 0)
#define bounds_of(ptr)    ((*((unsigned char  *)&ptr[10]) & 0x80) != 0)
#define dimct_of(ptr)     (*(unsigned char  *)&ptr[11])
#define arsize_of(ptr)    ((unsigned int)swapint(&ptr[12]))
#define a0_of(ptr)        ((unsigned int)swapint(&ptr[16]))
#define m_of(ptr)         (&ptr[20])
#define LoadChar(in,outp)  (outp)[0] = ((char *)&in)[0]
#ifdef _big_endian
#define LoadShort(in,outp) (outp)[0] = ((char *)&in)[1]; (outp)[1] = ((char *)&in)[0]
#define LoadInt(in,outp)   (outp)[0] = ((char *)&in)[3]; (outp)[1] = ((char *)&in)[2]; \
                           (outp)[2] = ((char *)&in)[1]; (outp)[3] = ((char *)&in)[0]
#else
#define LoadShort(in,outp) (outp)[0] = ((char *)&in)[0]; (outp)[1] = ((char *)&in)[1]
#define LoadInt(in,outp)   (outp)[0] = ((char *)&in)[0]; (outp)[1] = ((char *)&in)[1]; \
                           (outp)[2] = ((char *)&in)[2]; (outp)[3] = ((char *)&in)[3]
#endif
#define set_aflags(ptr,in)  ptr[10] = (inp->aflags.binscale << 3)  | (inp->aflags.redim << 4) | (inp->aflags.column << 5) \
                                     | (inp->aflags.coeff << 6) | (inp->aflags.bounds << 7)
#define offset(ptr)       *(unsigned int *)&ptr

#if defined(_big_endian)

static int swapint(char *in_c)
{
  int out;
  char *out_c = (char *)&out;
  int i;
  for (i=0;i<4;i++) out_c[3-i] = in_c[i];
  return out;
}

static int swapshort(char *in_c)
{
  short out;
  char *out_c = (char *)&out;
  int i;
  for (i=0;i<2;i++) out_c[1-i] = in_c[i];
  return out;
}

#else

static int swapint(char *in_c)
{
  int out;
  char *out_c = (char *)&out;
  int i;
  for (i=0;i<4;i++) out_c[i] = in_c[i];
  return out;
}

static int swapshort(char *in_c)
{
  short out;
  char *out_c = (char *)&out;
  int i;
  for (i=0;i<2;i++) out_c[i] = in_c[i];
  return out;
}

#endif

static int copy_rec_dx( char *in_ptr, struct descriptor_xd *out_dsc_ptr, 
                        unsigned int *b_out, unsigned int *b_in)
{
  unsigned int status = 1,
              bytes_out = 0,
              bytes_in = 0,
              i,j,
              size_out,
              size_in;
  if (in_ptr)
    switch (class_of(in_ptr))
    {
     case CLASS_S:
     case CLASS_D:
     {
	struct descriptor in;
	struct descriptor *po = (struct descriptor *) out_dsc_ptr;
        in.length = length_of(in_ptr);
        in.dtype = dtype_of(in_ptr);
	in.class = CLASS_S;
	if (po)
	{
	  *po = in;
	  po->pointer = (char *) (po + 1);
          memcpy(po->pointer,in_ptr+8,in.length);
          if (po->length > 1 && po->dtype != DTYPE_T)
	  {
	    switch (po->length)
	    {
	    case 2: *(short *)po->pointer = swapshort(po->pointer); break;
            case 4: *(int *)po->pointer = swapint(po->pointer); break;
            case 8: *(int *)po->pointer = swapint(po->pointer); 
	            ((int *)po->pointer)[1] = swapint(po->pointer + sizeof(int)); break;
	    }
	  }
	}
	bytes_out = align(sizeof(struct descriptor) + in.length,sizeof(void *));
        bytes_in = 8 + in.length;
     }
     break;

     case CLASS_XS:
     case CLASS_XD:
     {
	struct descriptor_xs in;
	struct descriptor_xs *po = (struct descriptor_xs *) out_dsc_ptr;
        in.length = 0;
        in.dtype = dtype_of(in_ptr);
        in.class = CLASS_XS;
        in.l_length = l_length_of(in_ptr);
	if (po)
	{
	  *po = in;
	  po->pointer = (struct descriptor *) (po + 1);
          memcpy(po->pointer,in_ptr+12,in.length);
	}
	bytes_out = align(sizeof(struct descriptor_xs) + in.l_length,sizeof(void *));
	bytes_in = 12 + in.l_length;
      }
      break;

     case CLASS_R:
     {
        struct descriptor_r pi_tmp;
	struct descriptor_r *pi = &pi_tmp;
	struct descriptor_r *po = (struct descriptor_r *) out_dsc_ptr;
        pi_tmp.length = length_of(in_ptr);
        pi_tmp.dtype = dtype_of(in_ptr);
        pi_tmp.class = CLASS_R;
        pi_tmp.ndesc = ndesc_of(in_ptr);
	pi_tmp.dscptrs[0] = 0;
	bytes_out = sizeof(struct descriptor_r) + (int)(pi->ndesc - 1) * sizeof(struct descriptor *);
        bytes_in = 12 + (int)(pi->ndesc) * 4;
	if (po)
	{

	  memset(po,0,bytes_out);
          *po = *pi;
	  if (pi->length > 0)
	  {
	    po->pointer = (unsigned char *) po + bytes_out;
            memcpy(po->pointer,&in_ptr[12+pi->ndesc*4],pi->length);
            if (po->dtype == DTYPE_FUNCTION && po->length == 2)
	      {
                *(short *)po->pointer = swapshort((char *)po->pointer);
              }
	  }
	}
	bytes_out = align(bytes_out + pi->length,sizeof(void *));
	bytes_in += pi->length;

      /******************************
      Each descriptor must be copied.
      ******************************/
	for (j = 0; j < pi->ndesc && status & 1; ++j)
          if (dscptrs_of(in_ptr,j))
	  {
	    status = copy_rec_dx(in_ptr + bytes_in, po ? (struct descriptor_xd *) ((char *) po + bytes_out) : 0, &size_out, &size_in);
	    if (po)
	      po->dscptrs[j] = size_out ? (struct descriptor *) ((char *) po + bytes_out) : 0;
	    bytes_out = align(bytes_out + size_out, sizeof(void *));
            bytes_in += size_in;
	  }

     }
     break;
     case CLASS_A:
     {
        int dsc_size;
        int align_size;
        array_coeff a_tmp;
	array_coeff *pi = &a_tmp;
	array_coeff *po = (array_coeff *) out_dsc_ptr;
        a_tmp.length = length_of(in_ptr);
        a_tmp.dtype = dtype_of(in_ptr);
        a_tmp.class = class_of(in_ptr);
        a_tmp.scale = scale_of(in_ptr);
        a_tmp.digits = digits_of(in_ptr);
	a_tmp.aflags.binscale = binscale_of(in_ptr);
	a_tmp.aflags.redim = redim_of(in_ptr);
	a_tmp.aflags.column = column_of(in_ptr);
	a_tmp.aflags.coeff = coeff_of(in_ptr);
	a_tmp.aflags.bounds = bounds_of(in_ptr);
        a_tmp.dimct = dimct_of(in_ptr);
        a_tmp.arsize = arsize_of(in_ptr);
	bytes_in = 16
		+ (pi->aflags.coeff ? sizeof(int) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0);
	dsc_size = sizeof(struct descriptor_a)
		+ (pi->aflags.coeff ? sizeof(char *) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0);
        align_size = (pi->dtype == DTYPE_T) ? 1 : pi->length;
	bytes_out = dsc_size + pi->arsize + align_size;
	if (po)
	{
          memcpy(po,pi,sizeof(struct descriptor_a));
          for (i=0;i<pi->dimct;i++)
          {
            if (pi->aflags.coeff)
            {
              po->m[i] = swapint(m_of(in_ptr)+sizeof(int) * i);
              if (pi->aflags.bounds)
              {
                po->m[pi->dimct + 2 * i] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i) * sizeof(int));
                po->m[pi->dimct + 2 * i + 1] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i + 1) * sizeof(int));
              }
            }
          }
	  po->pointer = (char *)po + (align(((unsigned int)((char *)po + dsc_size)),align_size) - (unsigned int)po);
          memcpy(po->pointer,&in_ptr[bytes_in],pi->arsize);
	  if (pi->aflags.coeff)
	    po->a0 = po->pointer + (a0_of(in_ptr) - pointer_of(in_ptr));
          if (po->dtype != DTYPE_T)
	  {
            int i;
	    switch (po->length)
	    {
	    case 2: 
	      { short *ptr;
		for (i=0,ptr=(short *)po->pointer;i< (int)po->arsize;i += sizeof(*ptr),ptr++) *ptr = swapshort((char *)ptr);
	      }
	      break;
	    case 4:
	    case 8:
	      { int *ptr;
		for (i=0,ptr=(int *)po->pointer;i<(int)po->arsize;i += sizeof(*ptr),ptr++) *ptr = swapint((char *)ptr);
	      }
	      break;
	    }
	  }
	}
	bytes_out += pi->arsize;
        bytes_in += pi->arsize;
     }
     break;

    /**************************************
    For CA and APD, a0 is the offset.
    **************************************/
     case CLASS_APD:
     {
        array_coeff a_tmp;
	array_coeff *pi = &a_tmp;
	array_coeff *po = (array_coeff *) out_dsc_ptr;
	struct descriptor **pdi = (struct descriptor **) pi->pointer;
	struct descriptor **pdo = 0;
	unsigned int       num_dsc;
        a_tmp.length = length_of(in_ptr);
        a_tmp.dtype = dtype_of(in_ptr);
        a_tmp.class = class_of(in_ptr);
        a_tmp.scale = scale_of(in_ptr);
        a_tmp.digits = digits_of(in_ptr);
	a_tmp.aflags.binscale = binscale_of(in_ptr);
	a_tmp.aflags.redim = redim_of(in_ptr);
	a_tmp.aflags.column = column_of(in_ptr);
	a_tmp.aflags.coeff = coeff_of(in_ptr);
	a_tmp.aflags.bounds = bounds_of(in_ptr);
        a_tmp.dimct = dimct_of(in_ptr);
        a_tmp.arsize = arsize_of(in_ptr);
	bytes_in = 16
		+ (pi->aflags.coeff ? sizeof(int) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0);
	bytes_out = sizeof(struct descriptor_a)
		+ (pi->aflags.coeff ? sizeof(char *) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0);
	num_dsc = pi->arsize / pi->length;
	if (po)
	{
          memcpy(po,pi,sizeof(struct descriptor_a));
          for (i=0;i<pi->dimct;i++)
          {
            if (pi->aflags.coeff)
            {
			  po->m[i] = swapint(m_of(in_ptr) + i * sizeof(int));
              if (pi->aflags.bounds)
              {
                po->m[pi->dimct + 2 * i] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i) * sizeof(int));
                po->m[pi->dimct + 2 * i + 1] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i + 1) * sizeof(int));
              }
            }
          }
	  po->pointer = (char *) po + bytes_out;
          for (i=0,pdo=(struct descriptor **)po->pointer;i<(int)num_dsc;i++)
            pdo[i] = (struct descriptor *)*(int *)&in_ptr[bytes_in];
	  if (pi->aflags.coeff)
	    po->a0 = po->pointer + (a0_of(in_ptr) - pointer_of(in_ptr));
	  pdo = (struct descriptor **) po->pointer;
          po->length = sizeof(struct descriptor *);
          po->arsize = num_dsc * sizeof(struct descriptor *);
	}
	bytes_out += po->arsize;
        bytes_in += pi->arsize;
      /******************************
      Each descriptor must be copied.
      ******************************/
	for (j = 0; j < num_dsc && status & 1; ++j)
	{
          status = copy_rec_dx(in_ptr + bytes_in, po ? (struct descriptor_xd *) ((char *) po + bytes_out) : 0, &size_out, &size_in);
	  if (po)
	    *pdo++ = size_out ? (struct descriptor *) ((char *) po + bytes_out) : 0;
	  bytes_out += size_out;
          bytes_in += size_in;
	}
      }
      break;

     case CLASS_CA:
     {
        array_coeff a_tmp;
	array_coeff *pi = &a_tmp;
	array_coeff *po = (array_coeff *) out_dsc_ptr;
	struct descriptor **pdi = (struct descriptor **) pi->pointer;
	struct descriptor **pdo = 0;
        a_tmp.length = length_of(in_ptr);
        a_tmp.dtype = dtype_of(in_ptr);
        a_tmp.class = class_of(in_ptr);
        a_tmp.scale = scale_of(in_ptr);
        a_tmp.digits = digits_of(in_ptr);
	a_tmp.aflags.binscale = binscale_of(in_ptr);
	a_tmp.aflags.redim = redim_of(in_ptr);
	a_tmp.aflags.column = column_of(in_ptr);
	a_tmp.aflags.coeff = coeff_of(in_ptr);
	a_tmp.aflags.bounds = bounds_of(in_ptr);
        a_tmp.dimct = dimct_of(in_ptr);
        a_tmp.arsize = arsize_of(in_ptr);
	bytes_in = 16
		+ (pi->aflags.coeff ? sizeof(int) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0);
	bytes_out = align(sizeof(struct descriptor_a)
		+ (pi->aflags.coeff ? sizeof(char *) + sizeof(int) * pi->dimct : 0)
		+ (pi->aflags.bounds ? sizeof(int) * (pi->dimct * 2) : 0),sizeof(void *));
	if (po)
	{
          memcpy(po,pi,sizeof(struct descriptor_a));
          for (i=0;i<pi->dimct;i++)
          {
            if (pi->aflags.coeff)
            {
              po->m[i] = swapint(m_of(in_ptr) + i * sizeof(int));
              if (pi->aflags.bounds)
              {
                po->m[pi->dimct + 2 * i] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i) * sizeof(int));
                po->m[pi->dimct + 2 * i + 1] = swapint(m_of(in_ptr) + (pi->dimct + 2 * i + 1) * sizeof(int));
              }
            }
          }
          po->pointer = (pointer_of(in_ptr)) ? 
              (char *)po + (align(((unsigned int)((char *)po + bytes_out)),sizeof(void *)) - (unsigned int)po) : 0;
	}

      /***************************
      Null pointer for shape only.
      ***************************/
	if (pointer_of(in_ptr))
	{
          status = copy_rec_dx(in_ptr + bytes_in, po ? (struct descriptor_xd *) ((char *) po + bytes_out) : 0, &size_out, &size_in);
	  bytes_out += size_out;
          bytes_in += size_in;
	}
      }
      break;

     default:
      status = LibINVSTRDES;
      break;
    }
  *b_out = bytes_out;
  *b_in  = bytes_in;
  return status;
}

int MdsSerializeDscIn(char *in, struct descriptor_xd *out)
{
  unsigned int size_out;
  unsigned int size_in;
  int       status;
  static const unsigned char dsc_dtype = DTYPE_DSC;
  status = copy_rec_dx(in, 0, &size_out, &size_in);
  if (status & 1 && size_out)
  {
    status = MdsGet1Dx(&size_out, (unsigned char *) &dsc_dtype, out, 0);
    if (status & 1)
      status = copy_rec_dx(in, (struct descriptor_xd *)out->pointer, &size_out, &size_in);
  }
  else
    MdsFree1Dx(out, 0);
  return status;
}

static int copy_dx_rec( struct descriptor *in_ptr,char *out_ptr,unsigned int *b_out, unsigned int *b_in)
{
  unsigned int status = 1,
              bytes_out = 0,
              bytes_in = 0,
              j,
              size_out,
              size_in,
              num_dsc;
  if (in_ptr)
    switch (in_ptr->class)
    {
     case CLASS_S:
     case CLASS_D:
      {
	if (out_ptr)
	{
          int dscsize = 8;
          LoadShort(in_ptr->length, out_ptr);
          LoadChar(in_ptr->dtype, out_ptr+2);
          LoadChar(in_ptr->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          out_ptr += 8;
          memcpy(out_ptr,((char *)in_ptr) + offset(in_ptr->pointer), in_ptr->length);
          if (in_ptr->dtype != DTYPE_T)
	  {
	    switch (in_ptr->length)
	    {
	    case 2: *(short *)out_ptr = swapshort(out_ptr); break;
            case 4: *(int *)out_ptr = swapint(out_ptr); break;
            case 8: *(int *)out_ptr = swapint(out_ptr); 
	            ((int *)out_ptr)[1] = swapint(out_ptr + sizeof(int)); break;
	    }
	  }
          out_ptr += in_ptr->length;
	}
	bytes_out = 8 + in_ptr->length;
        bytes_in = sizeof(struct descriptor) + in_ptr->length;
      }
      break;

     case CLASS_XS:
     case CLASS_XD:
      {
	struct descriptor_xs *inp = (struct descriptor_xs *)in_ptr;
	if (out_ptr)
	{
          int dscsize = 12;
          LoadShort(in_ptr->length, out_ptr);
          LoadChar(in_ptr->dtype, out_ptr+2);
          LoadChar(in_ptr->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          LoadInt(inp->l_length,out_ptr+8);
          out_ptr += 12; 
          memcpy(out_ptr, ((char *)in_ptr) + offset(in_ptr->pointer), inp->l_length);
          out_ptr += inp->l_length;
	}
	bytes_in = sizeof(struct descriptor_xs) + inp->l_length;
	bytes_out = 12 + inp->l_length;
      }
      break;

     case CLASS_R:
      {
        struct descriptor_r *inp = (struct descriptor_r *)in_ptr;
        char *begin = out_ptr;
        char *dscptrs = NULL;
        if (out_ptr)
	{
          int dscsize = inp->length ? 12 + inp->ndesc * 4 : 0;
          LoadShort(inp->length, out_ptr);
          LoadChar(inp->dtype, out_ptr+2);
          LoadChar(inp->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          LoadChar(inp->ndesc, out_ptr+8);
          dscptrs = out_ptr + 12;
          memset(dscptrs,0,inp->ndesc * 4);
          out_ptr += 12 + inp->ndesc * 4;
          if (inp->length)
            memcpy(out_ptr,((char *)inp)+offset(inp->pointer),inp->length);
          out_ptr += inp->length;
        }
        bytes_out = 12 + (int)(inp->ndesc) * 4 + inp->length;
        bytes_in = sizeof(struct descriptor_r) + (int)(inp->ndesc - 1) * sizeof(struct descriptor *) + inp->length;
        for (j = 0; j < inp->ndesc; j++)
	{
	  if (inp->dscptrs[j])
	  {
	    status = copy_dx_rec((struct descriptor *)(((char *)inp) + offset(inp->dscptrs[j])), out_ptr, &size_out, &size_in);
            if (out_ptr)
	    {
              int poffset = out_ptr - begin;
              LoadInt(poffset, dscptrs + (j * 4));
              out_ptr += size_out;
            }
            bytes_out += size_out;
            bytes_in += size_in;
          }
	}
      }
      break;

     case CLASS_A:
      {
        array_coeff *inp = (array_coeff *)in_ptr;
        if (out_ptr)
	{
          int dscsize = 16 + (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
                    + (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0);
          LoadShort(inp->length, out_ptr);
          LoadChar(inp->dtype, out_ptr+2);
          LoadChar(inp->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          LoadChar(inp->scale,out_ptr+8);
          LoadChar(inp->digits,out_ptr+9);
          set_aflags(out_ptr,in);
          LoadChar(inp->dimct,out_ptr+11);
          LoadInt(inp->arsize,out_ptr+12);
          out_ptr += 16;
          if (inp->aflags.coeff)
	  {
            int a0 = dscsize + (offset(inp->a0) - offset(inp->pointer));
            LoadInt(a0,out_ptr);
            out_ptr += 4;
            for (j=0;j<inp->dimct;j++)
	    {
              LoadInt(inp->m[j],out_ptr);
              out_ptr += 4;
            }
            if (inp->aflags.bounds)
	    {
              for (j=0;j<inp->dimct;j++)
	      {
                LoadInt(inp->m[inp->dimct + 2 * j],out_ptr);
                out_ptr += 4;
                LoadInt(inp->m[inp->dimct + 2 * j + 1],out_ptr);
                out_ptr += 4;
              }
            }
          }
          memcpy(out_ptr,((char *)inp) + offset(inp->pointer),inp->arsize);
          if (in_ptr->dtype != DTYPE_T)
	  {
            unsigned int i;
            switch (in_ptr->length)
	    {
            case 2:
              { short *ptr;
                for (i=0,ptr = (short *)out_ptr; i < inp->arsize; i += sizeof(*ptr),ptr++)
                  *ptr = swapshort((char *)ptr);
              }
              break;
            case 4:
            case 8:
              { int *ptr;
                for (i=0,ptr=(int *)out_ptr; i < inp->arsize; i += sizeof(*ptr),ptr++)
                  *ptr = swapint((char *)ptr);
              }
              break;
            }
          }
          out_ptr += inp->arsize;
        }
	bytes_out = 16
		+ (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0) + inp->arsize;
	bytes_in = sizeof(struct descriptor_a)
		+ (inp->aflags.coeff ? sizeof(char *) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0) + inp->arsize;
      }
      break;

    /**************************************
    For CA and APD, a0 is the offset.
    **************************************/
     case CLASS_APD:
      {
        array_coeff *inp = (array_coeff *)in_ptr;
        struct descriptor **dsc = (struct descriptor **)  (((char *)in_ptr) + offset(inp->pointer));
        char *begin = out_ptr;
        char *dscptr;
	num_dsc = inp->arsize / inp->length;
        if (out_ptr)
	{
          int dscsize = 16 + (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
                    + (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0);
          LoadShort(inp->length, out_ptr);
          LoadChar(inp->dtype, out_ptr+2);
          LoadChar(inp->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          LoadChar(inp->scale,out_ptr+8);
          LoadChar(inp->digits,out_ptr+9);
          set_aflags(out_ptr,in);
          LoadChar(inp->dimct,out_ptr+11);
          LoadInt(inp->arsize,out_ptr+12);
          out_ptr += 16;
          if (inp->aflags.coeff)
	  {
            int a0 = dscsize + (offset(inp->a0) - offset(inp->pointer));
            LoadInt(a0,out_ptr);
            out_ptr += 4;
            for (j=0;j<inp->dimct;j++)
	    {
              LoadInt(inp->m[j],out_ptr);
              out_ptr += 4;
            }
            if (inp->aflags.bounds)
	    {
              for (j=0;j<inp->dimct;j++)
	      {
                LoadInt(inp->m[inp->dimct + 2 * j],out_ptr);
                out_ptr += 4;
                LoadInt(inp->m[inp->dimct + 2 * j + 1],out_ptr);
                out_ptr += 4;
              }
            }
          }
          dscptr = out_ptr;
          out_ptr += num_dsc * 4;
          memset(dscptr, 0, num_dsc * 4);
        }
	bytes_in = 16
		+ (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0) + inp->arsize;
	bytes_out = sizeof(struct descriptor_a)
		+ (inp->aflags.coeff ? sizeof(char *) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0 + num_dsc * 4);
      /******************************
      Each descriptor must be copied.
      ******************************/
	for (j = 0; j < num_dsc; j++, bytes_in += 4)
	{
          if (offset(dsc[j]))
	  {
            status = copy_dx_rec((struct descriptor *)(((char *)in_ptr) + offset(dsc[j])), out_ptr, &size_out, &size_in);
  	    if (out_ptr)
	    {
              int poffset = out_ptr - begin;
              LoadInt(poffset, dscptr + (j * 4));
	      out_ptr += size_out;
            }
	    bytes_out += size_out;
            bytes_in += size_in;
          }
	}
      }
      break;

     case CLASS_CA:
      {
        array_coeff *inp = (array_coeff *)in_ptr;
        if (out_ptr)
	{
          int dscsize = inp->pointer ? 16 + (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
                    + (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0) : 0;
          LoadShort(inp->length, out_ptr);
          LoadChar(inp->dtype, out_ptr+2);
          LoadChar(inp->class,out_ptr+3);
          LoadInt(dscsize,out_ptr+4);
          LoadChar(inp->scale,out_ptr+8);
          LoadChar(inp->digits,out_ptr+9);
          set_aflags(out_ptr,in);
          LoadChar(inp->dimct,out_ptr+11);
          LoadInt(inp->arsize,out_ptr+12);
          out_ptr += 16;
          if (inp->aflags.coeff)
	  {
            int a0 = dscsize + (offset(inp->a0) - offset(inp->pointer));
            LoadInt(a0,out_ptr);
            out_ptr += 4;
            for (j=0;j<inp->dimct;j++)
	    {
              LoadInt(inp->m[j],out_ptr);
              out_ptr += 4;
            }
            if (inp->aflags.bounds)
	    {
              for (j=0;j<inp->dimct;j++)
	      {
                LoadInt(inp->m[inp->dimct + 2 * j],out_ptr);
                out_ptr += 4;
                LoadInt(inp->m[inp->dimct + 2 * j + 1],out_ptr);
                out_ptr += 4;
              }
            }
          }
        }
	bytes_out = 16
		+ (inp->aflags.coeff ? sizeof(int) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0);
	bytes_in = sizeof(struct descriptor_a)
		+ (inp->aflags.coeff ? sizeof(char *) + sizeof(int) * inp->dimct : 0)
		+ (inp->aflags.bounds ? sizeof(int) * (inp->dimct * 2) : 0);
      /***************************
      Null pointer for shape only.
      ***************************/
	if (inp->pointer)
	{
          status = copy_dx_rec((struct descriptor *)(((char *)in_ptr) + offset(inp->pointer)), out_ptr, &size_out, &size_in);
	  bytes_out += size_out;
          bytes_in += size_in;
	}
      }
      break;

     default:
      status = LibINVSTRDES;
      break;
    }
  *b_out = bytes_out;
  *b_in  = bytes_in;
  return status;
}

static int Dsc2Rec(struct descriptor *inp, struct descriptor_xd *out_dsc_ptr, unsigned int *reclen)
{
  unsigned int size_out;
  unsigned int size_in;
  int       status;
  static const unsigned char dsc_dtype = DTYPE_B;
  status = copy_dx_rec((struct descriptor *)inp, 0, &size_out, &size_in);
  if (status & 1 && size_out)
  {
    unsigned short nlen = 1;
    array out_template = {1,DTYPE_B,CLASS_A,0,0,0,{0,1,1,0,0},1,0};
    out_template.arsize = *reclen = size_out;
    status = MdsGet1DxA((struct descriptor_a *)&out_template, &nlen, (unsigned char *) &dsc_dtype, out_dsc_ptr);
    if (status & 1)
    {
	  memset(out_dsc_ptr->pointer->pointer,0,size_out);
      status = copy_dx_rec((struct descriptor *)inp, out_dsc_ptr->pointer->pointer, &size_out, &size_in);
	}
  }
  else
    MdsFree1Dx(out_dsc_ptr, NULL);
  return status;
}

static int PointerToOffset(struct descriptor *dsc_ptr, unsigned int *length)
{
  int       status = 1;
  if ((dsc_ptr->dtype == DTYPE_DSC) && (dsc_ptr->class != CLASS_A))
    status = PointerToOffset((struct descriptor *) dsc_ptr->pointer, length);
  if (status & 1)
  {
    switch (dsc_ptr->class)
    {
     case CLASS_S:
     case CLASS_D:
      *length += sizeof(struct descriptor) + dsc_ptr->length;
      dsc_ptr->pointer = dsc_ptr->pointer - (int) dsc_ptr;
      break;
     case CLASS_XD:
     case CLASS_XS:
      *length += sizeof(struct descriptor_xd) + ((struct descriptor_xd *) dsc_ptr)->l_length;
      dsc_ptr->pointer = dsc_ptr->pointer - (int) dsc_ptr;
      break;
     case CLASS_R:
      {
	struct descriptor_r *r_ptr = (struct descriptor_r *) dsc_ptr;
	int       i;
	*length += sizeof(*r_ptr) + (r_ptr->ndesc - 1) * sizeof(struct descriptor *)
		   + r_ptr->length;
	if (r_ptr->length != 0)
	  r_ptr->pointer = r_ptr->pointer - (int) r_ptr;
	for (i = 0; (status & 1) && (i < r_ptr->ndesc); i++)
	  if (r_ptr->dscptrs[i] != 0)
	  {
	    status = PointerToOffset(r_ptr->dscptrs[i], length);
	    r_ptr->dscptrs[i] = (struct descriptor *) ((char *) r_ptr->dscptrs[i] - (int) r_ptr);
	  }
      }
      break;
     case CLASS_A:
      {
	struct descriptor_a *a_ptr = (struct descriptor_a *) dsc_ptr;
	*length += sizeof(struct descriptor_a)
		+ (a_ptr->aflags.coeff ? sizeof(int) * (a_ptr->dimct + 1) : 0)
		+ (a_ptr->aflags.bounds ? sizeof(int) * (a_ptr->dimct * 2) : 0)
		+ a_ptr->arsize;
	a_ptr->pointer = a_ptr->pointer - (int) a_ptr;
	if (a_ptr->aflags.coeff)
	{
	  int     *a0_ptr = (int *)((char *) a_ptr + sizeof(struct descriptor_a));
	  *a0_ptr = *a0_ptr - (int) a_ptr;
	}
      }
      break;
     case CLASS_APD:
      {
	int       i;
	struct descriptor_a *a_ptr = (struct descriptor_a *) dsc_ptr;
	int       elts = a_ptr->arsize / a_ptr->length;
	*length += sizeof(struct descriptor_a)
		+ (a_ptr->aflags.coeff ? sizeof(int) * (a_ptr->dimct + 1) : 0)
		+ (a_ptr->aflags.bounds ? sizeof(int) * (a_ptr->dimct * 2) : 0)
		+ a_ptr->arsize;
	for (i = 0; (i < elts) && (status & 1); i++)
	{
	  struct descriptor **dsc_ptr = (struct descriptor **) a_ptr->pointer + i;
	  if (dsc_ptr && *dsc_ptr)
	  {
	    status = PointerToOffset(*dsc_ptr, length);
	    *dsc_ptr = (struct descriptor *) ((char *) *dsc_ptr - (int) a_ptr);
	  }
	  else
	    status = 1;
	}
	if (status & 1)
	{
	  a_ptr->pointer = a_ptr->pointer - (int) a_ptr;
	  if (a_ptr->aflags.coeff)
	  {
	    char     *a0_ptr = (char *) a_ptr + sizeof(struct descriptor_a);
	    *a0_ptr = *a0_ptr - (int) a_ptr;
	  }
	}
      }
      break;
     case CLASS_CA:
      if (dsc_ptr->pointer)
      {
	unsigned int dummy_length;
	struct descriptor_a *a_ptr = (struct descriptor_a *) dsc_ptr;
	*length += sizeof(struct descriptor_a)
		+ (a_ptr->aflags.coeff ? sizeof(int) * (a_ptr->dimct + 1) : 0)
		+ (a_ptr->aflags.bounds ? sizeof(int) * (a_ptr->dimct * 2) : 0)
		+ a_ptr->arsize;
	status = PointerToOffset((struct descriptor *) dsc_ptr->pointer, &dummy_length);
	a_ptr->pointer = a_ptr->pointer - (int) a_ptr;
      }
      break;
     default:
      status = LibINVSTRDES;
      break;
    }
  }
  return status;
}

int MdsSerializeDscOutZ(struct descriptor *in,
	     struct descriptor_xd *out,
             int (*fixupNid)(),
	     void *fixupNidArg,
             int (*fixupPath)(),
             void *fixupPathArg,
             int compress,
             int *compressible_out,
             unsigned int *length_out,
             unsigned int *reclen_out,
             unsigned char *dtype_out,
             unsigned char *class_out,
             int  altbuflen,
             void *altbuf,
             int *data_in_altbuf_out)
{
  int       status;
  struct descriptor *out_ptr;
  struct descriptor_xd tempxd;
  int compressible = 0;
  unsigned int length = 0;
  unsigned int reclen = 0;
  unsigned char dtype = 0;
  unsigned char class = 0;
  int data_in_altbuf = 0;
  status = MdsCopyDxXdZ(in, out, 0, fixupNid, fixupNidArg, fixupPath, fixupPathArg);
  if (status == MdsCOMPRESSIBLE)
  {
    if (compress)
    {
      tempxd = *out;
      out->l_length = 0;
      out->pointer = 0;
      status = MdsCompress(0, 0, tempxd.pointer, out);
      MdsFree1Dx(&tempxd, NULL);
      compressible = 0;
    }
    else
      compressible = 1;
  }
  else
    compressible = 0;
  if (status & 1)
  {
    if (out->pointer && out->dtype == DTYPE_DSC)
    {
      out_ptr = out->pointer;
      dtype = out_ptr->dtype;
      if ( (out_ptr->class == CLASS_S || out_ptr->class == CLASS_D) &&
           out_ptr->length < altbuflen)
      {
        data_in_altbuf = 1;
	class = CLASS_S;
	length = out_ptr->length + 8;
        memcpy(altbuf, out_ptr->pointer, out_ptr->length);
      }
      else
      {
	data_in_altbuf = 0;
	class = (out_ptr->class == CLASS_XD) ? CLASS_XS : out_ptr->class;
	length = 0;
	status = PointerToOffset(out->pointer, &length);
        if (status & 1)
	{ 
	  tempxd = *out;
	  out->l_length = 0;
	  out->pointer = 0;
	  Dsc2Rec(tempxd.pointer,out,&reclen);
	  MdsFree1Dx(&tempxd, NULL);
	}
      }
    }
    else
    {
      length = 0;
      dtype = 0;
      class = 0;
      reclen = 0;
      data_in_altbuf = 1;
    }
  }
  if (compressible_out) *compressible_out = compressible;
  if (length_out) *length_out = length;
  if (reclen_out) *reclen_out = reclen;
  if (dtype_out) *dtype_out = dtype;
  if (class_out) *class_out = class;
  if (data_in_altbuf_out) *data_in_altbuf_out = data_in_altbuf;
  return status;
}

int MdsSerializeDscOut(struct descriptor *in,struct descriptor_xd *out)
{
  return MdsSerializeDscOutZ(in,out,0,0,0,0,1,0,0,0,0,0,0,0,0);
}
