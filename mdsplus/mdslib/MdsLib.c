/* TO DO: 

 - multi-dimensional arrays (and Fortran - fill by column) 
   - HOW TO HANDLE >2d arrays???

 - cstring data types

 - parameters passed to native API

*/

#include "MdsLib.h"
extern int TdiExecute();
extern int TdiData();
static short ArgLen(struct descrip *d);
static EMPTYXD(mdsValueAnswer);

static int next = 0;

int descr (int *dtype, void *data, int *dim1, ...)

{

  struct descriptor *dsc = &descrs[next];
  int retval;

  dsc->dtype = *dtype;
  dsc->pointer = (char *)data;

  printf("DTYPE_FLOAT: %d\n",DTYPE_FLOAT);

  if (*dim1 == 0) 
  {
    dsc->class = CLASS_S;
    switch (dsc->dtype)
    {
      case DTYPE_CHAR :  
	dsc->length = sizeof(char);
	break;
      case DTYPE_SHORT :  
	dsc->length = sizeof(short);
	break;
      case DTYPE_LONG :  
	dsc->length = sizeof(long);
	break;
      case DTYPE_FLOAT :  
	dsc->length = sizeof(float);
	break;
      case DTYPE_DOUBLE :  
	dsc->length = sizeof(double);
	break;
      default : 
	dsc->length = 0;
    }
  }
  else 
  {

    va_list incrmtr;

    int ndim=1;
    int *dim = &ndim; /*must initialize dim to nonnull so test below passes */
    
    /* count the number of dimensions beyond the first */
    va_start(incrmtr, dim1);
    for (ndim = 1; *dim !=NULL; ndim++)
    {
      dim = va_arg(incrmtr, int *);
    }
    ndim = ndim - 1;  /* ndim is actually the number of dimensions specified */

    if (ndim > 1) 
    {
      int i;
      array_coeff *adsc = (array_coeff *)dsc;  /* &&& CAN I DO DYNAMIC NUMBER OF DIMENSIONS HERE??? */
      adsc->class = CLASS_A;
  
      if (ndim > MAXDIM) 
      {
	ndim = MAXDIM;
	printf("(descr.c) WARNING: requested ndim>MAXDIM, forcing to MAXDIM\n");
      }
      adsc->dimct = ndim;

      adsc->aflags.coeff = 1;
      adsc->a0 = adsc->pointer;  /* &&& this will need to be adjusted for native API, as (array lower bound=0) will not be required. */
      adsc->m[0] = *dim1;

      va_start(incrmtr, dim1);
      for (i = 1; i<ndim; i++) 
      {
	adsc->m[i] = *(va_arg(incrmtr, int *));
      }
      for (i = ndim; i<MAXDIM; i++)
      {
	adsc->m[i] = 0;
      }
    }
    else 
    {
      struct descriptor_a *adsc = (struct descriptor_a *)dsc;
      adsc->class = CLASS_A;
  
      adsc->dimct = 1;
      if (ndim < 1) printf("(descr.c) WARNING: requested ndim<1, forcing to 1.\n");
    }
      


  }
  retval = next+1;
  next++;
  if (next >= NDESCRIP_CACHE) next = 0;
  return retval;
}






void NewMdsConnect(char *host)
{
  mdsSocket = ConnectToMds(host);  /*** SETS GLOBAL VARIABLE mdsSOCKET ***/
}



struct descrip *MakeIpDescrip(struct descrip *arg, struct descriptor *dsc)
{
  if (dsc->class == CLASS_S) 
  {
    arg = MakeDescrip(arg, dsc->dtype, 0, 0, dsc->pointer);
  } 
  else 
  {
    int i;
    array_coeff *adsc = (array_coeff *)dsc;  /* &&& CAN I DO DYNAMIC NUMBER OF DIMENSIONS HERE??? */
    if (adsc->dimct > 1) 
    {
      int dims[MAXDIM];
      for (i=0; i<adsc->dimct; i++) dims[i] = adsc->m[i];
      for (i=adsc->dimct; i<MAXDIM; i++) dims[i] = 0;
      arg = MakeDescrip(arg, adsc->dtype, adsc->dimct, dims, adsc->pointer);
    }
    else 
    {
      /*** &&& HOW AM I KEEPING TRACK OF N_ELEMENTS() IF THERE IS ONLY ONE DIM?  via LENGTH and ARSIZE??? ***/
    }
  }
  return arg;
}



int NewMdsValue(char *expression, ...) 
{

  va_list incrmtr;
  int a_count;
  int i;
  unsigned char nargs;
  unsigned char idx;
  int status = 1;
  int *descnum = &status;  /* initialize to point at non zero value */
  struct descriptor *dsc;

  va_start(incrmtr, expression);
  for (a_count = 0; *descnum != NULL; a_count++)
  {
    descnum = va_arg(incrmtr, int *);
  }
  a_count--; /* subtract one for terminator of argument list */


  /**************************** REPLACE client/server with regular access ******************/

  if (mdsSocket > 0)   /* CLIENT/SERVER */
  {
    struct descrip exparg;
    struct descrip *arg = &exparg;

    va_start(incrmtr, expression);
    nargs = a_count - 1 + 1;  /* -1 for answer argument, +1 for expression */

    arg = MakeDescrip(&exparg,DTYPE_CSTRING,0,0,expression);
    for (i=0;i<nargs && (status & 1);i++)
    {
      /*printf("sock: %d, idx: %d, dtype: %d, nargs: %d, len: %d, ndims: %d, dims[0]: %d, ptr: %f\n",
	mdsSocket,i,arg->dtype,nargs,ArgLen(arg),arg->ndims,arg->dims,*(float *)arg->ptr);*/
      status = SendArg(mdsSocket, i, arg->dtype, nargs, ArgLen(arg), arg->ndims, arg->dims, arg->ptr);
      descnum = va_arg(incrmtr, int *);
      dsc = &descrs[*descnum-1];
      arg = MakeIpDescrip(arg, dsc);

    }

      

    if (status & 1)
    {
      short len;
      int numbytes;
      void *dptr;
      struct descrip exparg;
      struct descrip *arg = &exparg;

      arg = MakeIpDescrip(arg, dsc);

      status = GetAnswerInfo(mdsSocket, &arg->dtype, &len, &arg->ndims, arg->dims, &numbytes, &dptr);

      arg->length = len; 
      if (numbytes)
      {
	switch (arg->dtype)
	{

/*	  case DTYPE_UCHAR :     
	    memcpy(dsc->pointer, (uchar *) dptr, numbytes);
	    break;
	    
	  case DTYPE_USHORT : 
	    memcpy(dsc->pointer, (ushort *) dptr, numbytes);
	    break;

  	  case DTYPE_ULONG : 
	    memcpy(dsc->pointer, (ulong *) dptr, numbytes);
	    break;
*/
  	  case DTYPE_CHAR : 
  	  case DTYPE_CSTRING:
	    memcpy(dsc->pointer, (char *) dptr, numbytes);
	    break;

  	  case DTYPE_SHORT : 
	    memcpy(dsc->pointer, (short *) dptr, numbytes);
	    break;

  	  case DTYPE_LONG : 
	    memcpy(dsc->pointer, (long *) dptr, numbytes);
	    break;

  	  case DTYPE_FLOAT : 
	    memcpy(dsc->pointer, (float *) dptr, numbytes);
	    break;

  	  case DTYPE_DOUBLE : 
	    memcpy(dsc->pointer, (double *) dptr, numbytes);
	    break;

	  default : 
	    status = 0;
	    break;
	}
      } 
      else 
      {
	status = 0;
      }
    
    }
  }
  else 
  {
    int status;
    void *arglist[34];
    struct descriptor *dsc;
    struct descriptor dexpression = {0,DTYPE_T,CLASS_S,0};
    EMPTYXD(tmp);
    int argidx = 1;
    int i;
    dexpression.length = strlen((char *)expression);
    dexpression.pointer = (char *)expression;
    arglist[argidx++] = (void *)&dexpression;
    va_start(incrmtr, expression);
    for (i=1;i < a_count; i++)
    {
      descnum = va_arg(incrmtr, int *);
      dsc = &descrs[*descnum-1];
      arglist[argidx++] = (void *)dsc;
    }
    arglist[argidx++] = (void *)&tmp;
    arglist[argidx++] = MdsEND_ARG;
    *(int *)&arglist[0] = argidx; 
    status = LibCallg(arglist,TdiExecute);
    if (status & 1)
    {
      status = TdiData(tmp.pointer,&mdsValueAnswer MDS_END_ARG);
      if (status) 
      {
	int numbytes;
	descnum = va_arg(incrmtr, int *);
	dsc = &descrs[*descnum-1];
        /* TdiCvt(&mdsValueAnswer,&dsc,&mdsValueAnswer MDS_END_ARG); */
	switch ((*mdsValueAnswer.pointer).class)
	{
	  case CLASS_S:
	  case CLASS_D:
	                 numbytes = (*mdsValueAnswer.pointer).length;
			 break;
	  case CLASS_A:
	                 numbytes = (*(struct descriptor_a *)mdsValueAnswer.pointer).arsize;
			 break;
	  default:
	                 status = 0;
	}

	if (status & 1)
	{
	  switch (dsc->dtype)
	  {
	
  	    case DTYPE_CHAR : 
	      memcpy(dsc->pointer, (char *) (*mdsValueAnswer.pointer).pointer, numbytes);
	      break;

  	    case DTYPE_SHORT : 
	      memcpy(dsc->pointer, (short *) (*mdsValueAnswer.pointer).pointer, numbytes);
	      break;

  	    case DTYPE_LONG : 
	      memcpy(dsc->pointer, (long *) (*mdsValueAnswer.pointer).pointer, numbytes);
	      break;

  	    case DTYPE_FLOAT : 
	      memcpy(dsc->pointer, (float *) (*mdsValueAnswer.pointer).pointer, numbytes);
	      break;

  	    case DTYPE_DOUBLE : 
	      memcpy(dsc->pointer, (double *) (*mdsValueAnswer.pointer).pointer, numbytes);
	      break;

	    default : 
	      status = 0;
	      break;
	  }
	}
      }

      MdsFree1Dx(&tmp, NULL);
    }
  }

}



int  NewMdsOpen(char *tree, int shot)
{

  if (mdsSocket > 0)
  {
    struct descrip treearg;
    struct descrip shotarg;
    struct descrip ansarg;
#ifdef _UNIX_SERVER
    static char *expression = "TreeOpen($,$)";
#else
    static char *expression = "MDSLIB->MDS$OPEN($,$)";
#endif

    int status = MdsValue(mdsSocket, expression, MakeDescrip((struct descrip *)&treearg,DTYPE_CSTRING,0,0,tree), 
			  MakeDescrip((struct descrip *)&shotarg,DTYPE_LONG,0,0,&shot),
			  (struct descrip *)&ansarg, (struct descrip *)NULL);
    
    if ((status & 1) && (ansarg.dtype == DTYPE_LONG)) status = *(int *)ansarg.ptr;
    if (ansarg.ptr) free(ansarg.ptr);
    return status;
  }
  else 
  {
    return TreeOpen(*tree, shot);
  }
}


int  NewMdsClose(char *tree, int shot)
{

  if (mdsSocket > 0)
  {
    struct descrip ansarg;
#ifdef _UNIX_SERVER
    static char *expression = "TreeClose()";
#else
    static char *expression = "MDSLIB->MDS$CLOSE()";
#endif
    int status = MdsValue(mdsSocket, expression, &ansarg, NULL);
    if ((status & 1) && (ansarg.dtype == DTYPE_LONG)) status = *(int *)ansarg.ptr;
    if (ansarg.ptr) free(ansarg.ptr);
    return status;
  }
  else 
  {
    return TreeClose(*tree, shot);
  }

}

