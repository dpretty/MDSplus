/*	Tdi1SQL.C
	An interface to dynamic SQL within TDI.
	Links are dynamic to keep out Rdb libraries until wanted.
	rows = ISQL("SQL-string")
	Interaction is with terminal.

	rows = DSQL("SQL-string", question-mark-replacements, ... output-variables, ...)
	If inputs are missing they are supplied as indicator asserted.
	Useful only for INSERT and UPDATE statements, probably.

	Output may be a variable (defined or not)
	or text string of variable name but not an expression for name.
	If an output for selection XXX is NULL pointer
	then variable of name _XXX is created.
	Note for calculated select, name will be just "_".
	Note same names from different tables will be folded together.

	The SQL string sElEcT is a blob write. 
	Ken Klare, LANL P-4	(c)1991,1992
*/
#include <mdstypes.h>
#define _MOVC3(a,b,c) memcpy(c,b,a)
#include <STATICdef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mdsdescrip.h>
#include <mdsdescrip.h>
#include <math.h>	/*for pow definition*/
#include "tdirefstandard.h"
#include <strroutines.h>
#include <libroutines.h>
#include <tdimessages.h>
#include <mdsshr.h>
#include <mds_stdarg.h>
#ifndef HAVE_VXWORKS_H
#include <config.h>
#endif
#ifdef HAVE_SYBASE
#ifdef __VMS
#include <dvidef.h>	/*for line width*/
#include <starlet.h>
#endif

#ifdef  __ALPHA			/**---AXP---**/
#undef   DTYPE_D
#define  DTYPE_D DTYPE_G
#endif

#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a): (b))
#endif

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: TdiSql.c,v $ $Revision: 1.37 $ $Date: 2012/05/10 18:55:53 $";

extern int stat;
extern int TdiFindImageSymbol();
extern int TdiGetLong();
extern int TdiGetIdent();
extern int TdiPutIdent();
extern int TdiData();
extern int TdiVector();
extern int TdiText();

/* MUST match declaration in DYNAMIC_SYB.C ( ../idlsql/dyanamic_syb.c ) */
#define MAXPARSE 16384

STATIC_CONSTANT DESCRIPTOR(dunderscore,	"_");
typedef struct {
	unsigned int	l0;
	int		l1;
}	quadw;
typedef struct {
	int			used;
	int			c;
	struct descriptor	**v;
}	ARGLIST;
STATIC_CONSTANT double	HUGE_D = 1.7e+38;
STATIC_CONSTANT float	HUGE_F = (float)1.7e+38;
STATIC_CONSTANT quadw	HUGE_Q = {0xffffffff,0x7fffffff};
STATIC_CONSTANT int	HUGE_L = 0x7fffffff;
STATIC_CONSTANT short	HUGE_W = 0x7fff;
STATIC_CONSTANT char	HUGE_B = 0x7f;

/*
STATIC_CONSTANT  unsigned int nan_f_bits =  0x7fbfffff;
STATIC_CONSTANT unsigned long long nan_d_bits = 0xffffffff7ff7ffff;
*/
STATIC_CONSTANT  unsigned int nan_f_bits =  0x7fc00000;

#if defined(_MSC_VER) && _MSC_VER <= 1300
STATIC_CONSTANT _int64u nan_d_bits = 0x7ff8000000000000Ui64;
#else
STATIC_CONSTANT _int64u nan_d_bits = 0x7ff8000000000000ULL;
#endif

static double d_null = 0;
static float  f_null = 0;

STATIC_CONSTANT const char *default_date = "Jan 01 1970 12:00:00:000AM";
static DESCRIPTOR(ddate, "dd-mmm-yyyy hh:mm:ss.cc");
static int	width = 0, head = 2;
static int	(*USERSQL_ERRORS)() = 0;
static int	(*USERSQL_GETS)() = 0;
static int	(*USERSQL_PUTS)() = 0;
static int	(*USERSQL_SET)() = 0;
static int	(*SQL_DYNAMIC)() = 0;
static char    *(*SQL_GETDBMSGTEXT)() = 0;
 
static int      (*SYB_dbconvert)() = 0;
static int      (*SYB_dbnumcols)() = 0;
static char    *(*SYB_dbcolname)() = 0;
static int      (*SYB_dbdatlen)() = 0;
static char    *(*SYB_dbdata)() = 0;
static int      (*SYB_dbcoltype)() = 0;

#ifndef HAVE_WINDOWS_H
#include "sybfront.h"
#include "sybdb.h"
#else
#define _LPCBYTE_DEFINED
typedef unsigned char BYTE;
typedef       BYTE *LPBYTE;
typedef const LPBYTE         LPCBYTE ;
#include <windows.h>
#include <sqlfront.h>
#include <sqldb.h>
#define SYBCHAR		SQLCHAR 
#define SYBTEXT		SQLTEXT
#define SYBFLT8		SQLFLT8
#define SYBREAL		SQLFLT4
#define SYBINT4		SQLINT4
#define SYBINT2		SQLINT2 
#define SYBINT1		SQLINT1
#define SYBDATETIME	SQLDATETIME
#define SYBDATETIME4	SQLDATETIM4
#define SYBMONEY	SQLMONEY
#endif

/*********************************************************/
STATIC_ROUTINE int	TDISQL_LINK(char *name, int (**routine)()) {
STATIC_CONSTANT DESCRIPTOR(dimage,	"MdsSql");
#ifdef __APPLE__
STATIC_CONSTANT DESCRIPTOR(dimage2,	"sybdb");
#endif	
struct descriptor dname = {0,DTYPE_T,CLASS_S,0};
int	status;
dname.length = (unsigned short)strlen(name);
dname.pointer = name;
status = TdiFindImageSymbol(&dimage, &dname, routine);
#ifdef __APPLE__
if (!(status & 1)) status = TdiFindImageSymbol(&dimage2, &dname, routine);
#endif
return status;
}
/*********************************************************/
STATIC_ROUTINE int	TDISQL_LINKCPTR(char *name, char * (**routine)()) {
STATIC_CONSTANT DESCRIPTOR(dimage,	"MdsSql");
#ifdef __APPLE__
	STATIC_CONSTANT DESCRIPTOR(dimage2,	"sybdb");
#endif	
struct descriptor dname = {0,DTYPE_T,CLASS_S,0};
int	status;
dname.length = (unsigned short)strlen(name);
dname.pointer = name;
status = TdiFindImageSymbol(&dimage, &dname, routine);
#ifdef __APPLE__
if (!(status & 1)) status = TdiFindImageSymbol(&dimage2, &dname, routine);
#endif
return status;
}

static int date=0;
static int NaN=0;

static struct ans_buf {
	void *vptr;
	int size;
	int offset;
	int len;
	int syb_type;
} *bufs=0;
static int num_bufs=0;

static void FreeBuffers() {
	int i;
	if (bufs) {
		for (i=0; i<num_bufs; i++) {
			if (bufs[i].vptr) {
				free(bufs[i].vptr);
			}
		}
		free(bufs);
	}
	num_bufs = 0;
	bufs = 0;
}

#define INITIAL_GUESS 32767

STATIC_ROUTINE void AppendAnswer(int idx, void *buffer, int len, int dtype)
{
	int needed = len;
	/*
	if ((dtype == SYBDATETIME) || (dtype == SYBDATETIME4) && !date) dtype = SYBTEXT;
	*/
	if (dtype == DTYPE_T) needed++;
/*
	if (idx > num_bufs) {
		num_bufs++;
		bufs = realloc(bufs, num_bufs*sizeof(struct ans_buf));
		bufs[idx].size = 0;
		bufs[idx].offset=0;
	}
*/
        if (bufs[idx].size < (bufs[idx].offset+needed)) {
		int new_size = (bufs[idx].size == 0) ? INITIAL_GUESS*len : 3*bufs[idx].size;
		if (new_size < bufs[idx].offset+needed) new_size += needed;  /* pathalogical case of a very big string */
		bufs[idx].vptr = realloc(bufs[idx].vptr, new_size);
		bufs[idx].size = new_size;
	}
	memcpy((void *)((char *)bufs[idx].vptr+bufs[idx].offset), buffer, len);
	bufs[idx].offset += len;
	if (dtype == DTYPE_T) {
		char *cptr = bufs[idx].vptr;
		cptr[bufs[idx].offset++] = 1;
	}
}

STATIC_ROUTINE void StoreAnswer(int idx, struct descriptor *dst, int type)
{
	int status = 1;
	DESCRIPTOR_A(src, 0, 0, 0, 0);
	struct descriptor_xd xs = {0, DTYPE_DSC, CLASS_XS, 0, sizeof(src)}; 
	EMPTYXD(xd);
	struct descriptor t_dsc = {0, DTYPE_T, CLASS_S, 0 };
        xs.pointer = (struct descriptor *)&src;
	if (((type==SYBDATETIME) || (type==SYBDATETIME4)) && !date) type = SYBTEXT;
        src.pointer = bufs[idx].vptr;
        src.arsize = bufs[idx].offset;
	switch(type) {
	case SYBFLT8 : src.length = 8; src.dtype = DTYPE_FT; break;
	case SYBREAL : src.length = 4; src.dtype = DTYPE_FS; break;
	case SYBINT4 : src.length = 4; src.dtype = DTYPE_L; break;
	case SYBINT2 : src.length = 2; src.dtype = DTYPE_W; break;
	case SYBINT1 : src.length = 1; src.dtype = DTYPE_BU; break;
	case SYBDATETIME :
	case SYBDATETIME4 : src.length = 8; src.dtype = DTYPE_FT; break;
	case SYBCHAR :
	case SYBTEXT :
	case SYBMONEY : {
		int num = bufs[idx].offset / 32767 + 1;
		if (num > 1) {
			src.length = 32767;
		    src.dtype = DTYPE_T;
			if (bufs[idx].size < 32767*num) {
			  src.pointer=bufs[idx].vptr = realloc(bufs[idx].vptr, 32767*num);
				bufs[idx].size = 32767*num;
			}
			if (bufs[idx].offset < 32767*num) {
			  memset(((char *)src.pointer)+bufs[idx].offset,0,32767*num-bufs[idx].offset);
			}
			src.arsize = 32767*num;
		} else {
			t_dsc.length = bufs[idx].offset;
			t_dsc.pointer = bufs[idx].vptr;
			xs.pointer = &t_dsc;
		}
					}
		break;
	default: status=0;
	}
        if (status & 1)
	  status = TdiPutIdent(dst, &xs);
	else
	  status = TdiPutIdent(dst, &xd);
	free(bufs[idx].vptr);
	bufs[idx].vptr=0;
	bufs[idx].size=0;
}



STATIC_ROUTINE int Puts(dbproc, prows, arg, rblob)
DBPROCESS	*dbproc;
int 	*prows;
ARGLIST 	*arg;
int 	rblob;
{
	int 		  status=1;
	int 	rows = *prows, used = arg->used;
	int isnotseg = 1, ncol = SYB_dbnumcols(dbproc);
	int   ind,j,type=0,len, dtype;
	char	ddate[28];
	double d_ans;
	char space=' ';
	
	
	struct descriptor_xd	**argv = (struct descriptor_xd **)arg->v;
	struct descriptor		*dst;
	struct descriptor_d 	madeup = {0,DTYPE_T,CLASS_D,0};
	char				*buf;
	static struct descriptor name = {0,DTYPE_T,CLASS_S,0};
	
	if (rows == 0) {
		if (bufs) {
			FreeBuffers();
		}
                if (ncol > 0)
		  bufs = (struct ans_buf *)calloc(ncol,sizeof(struct ans_buf));
		num_bufs = ncol;
		
		/*
		//	  if (used + ncol < arg->c) status = TdiEXTRA_ARG;
		//	  if (used + ncol > arg->c) status = TdiMISS_ARG;
		*/
	}
	else 
		for (j = 0; j < MAX(ncol,num_bufs); ++j, ++used) { 
			if (rows < 0) {
 	/*		dst = (struct descriptor *)(used + j > arg->c ? 0 : *(argv+used)); // should'nt it be used > arg-> c ? */
				dst = (struct descriptor *)(used  > arg->c ? 0 : *(argv+used)); /* // should'nt it be used > arg-> c ? */
				while (dst && dst->dtype == DTYPE_DSC) dst = (struct descriptor *)dst->pointer;
				if (dst == 0) { /* // && (rblob || (pda->SQLTYPE & ~1) != SQL_TYPE_SEGMENT_ID)) { */
					name.pointer = SYB_dbcolname(dbproc,j+1) ;
					name.length = strlen(name.pointer);
					status = StrConcat(dst = (struct descriptor *)&madeup, (struct descriptor *)&dunderscore, &name MDS_END_ARG);
					if (!(status & 1)) break;
					dst->dtype = DTYPE_IDENT;
				}
				StoreAnswer(j, dst, bufs[j].syb_type);
			} else {
				if (j > num_bufs) {
					num_bufs++;
					bufs = realloc(bufs, num_bufs*sizeof(struct ans_buf));
					bufs[j].size = 0;
					bufs[j].offset=0;
					bufs[j].syb_type=0;
				}
				buf = SYB_dbdata(dbproc, j+1);
				ind = buf == (void *)NULL;
				if ((bufs[j].syb_type == 0) || (bufs[j].len == 0)) {
					bufs[j].syb_type = SYB_dbcoltype(dbproc, j+1);
					bufs[j].len = SYB_dbdatlen(dbproc, j+1);
				}
				/*
				   if (rows == -1) status = TdiPutIdent(dst, &tmp);
				*/
/*
				len = SYB_dbdatlen(dbproc, j+1);
				buf = SYB_dbdata(dbproc, j+1);
				ind = len == 0 && buf == (void *)NULL || rows < 0;
				type = SYB_dbcoltype(dbproc, j+1);
*/
				type = bufs[j].syb_type;
				switch (type) {
				case SYBCHAR :
				case SYBTEXT :
					if (ind) {
						buf = &space;
						len = 1;
					}
					else {
						len = SYB_dbdatlen(dbproc, j+1);
					}
					dtype = DTYPE_T;
					AppendAnswer(j, buf, len, dtype);
					break;
				case SYBFLT8:
					if (ind) {buf = (NaN)?(char *)&nan_d_bits:(char *)&HUGE_D;bufs[j].len = 8;}
					dtype = DTYPE_FT;
					AppendAnswer(j, buf, bufs[j].len, dtype);
					break;
				case SYBREAL:
					if (ind) {buf = (NaN)?(char *)&nan_f_bits:(char *)&HUGE_F;bufs[j].len = 4;}
					dtype = DTYPE_FS;
					AppendAnswer(j, buf, bufs[j].len, dtype);
					break;
				case SYBINT4 :
					if (ind) {buf = (char *)&HUGE_L;bufs[j].len = 4;}
					dtype = DTYPE_L;
					AppendAnswer(j, buf, bufs[j].len, dtype);
					break;
				case SYBINT2 :
					if (ind) {buf = (char *)&HUGE_W;bufs[j].len = 2;}
					dtype = DTYPE_W;
					AppendAnswer(j, buf, bufs[j].len, dtype);
					break;
				case SYBINT1 :
					if (ind)  {buf = (char *)&HUGE_B; bufs[j].len = 1;}
					dtype = DTYPE_BU;
					AppendAnswer(j, buf, bufs[j].len, dtype);
					break;
					
					
				case SYBDATETIME4 : 	/***convert date to double***/
				case SYBDATETIME :		/***convert date to double***/
					
					
					if (date) 
					{		/*Julian day 3 million?? is 17-Nov-1858*/
#ifdef OLD						
						int yr, mo, da, hr, mi, se, th, leap;
						int status = SYB_dbconvert(dbproc,type,buf,bufs[j].len,SYBCHAR,(unsigned char*)ddate,sizeof(ddate)-1);
						
#ifdef VMS
						quadword big_time;
						struct dsc$descriptor_s date_dsc = {22, DSC$K_DTYPE_T, DSC$K_CLASS_S, (char *)ddate};
						len=sizeof(d_null);
						dtype = DTYPE_FT;
						ddate[20] = '.';
						ddate[3] = ddate[0];
						ddate[0] = ddate[4];
						ddate[4] = __toupper(ddate[1]);
						ddate[1] = ddate[5];
						ddate[5] = __toupper(ddate[2]);
						ddate[2] = '-';
						ddate[6] = '-';
						if (status < 0) {buf = (char *)&d_null; break;}
						status = sys$bintim(&date_dsc, &big_time);
						if(status&1) {
							double t1;
							double t2;
							t1 = (double)big_time.l1 * pow(2e0, 32e0);
							t2 = (double)big_time.l0; 
							d_ans = (t1+t2)/ 86400e7;
							buf = (char *)&d_ans;
						}
						else
							buf = (char *)&d_null;
						break;
#else
						STATIC_CONSTANT char *moname = "JanFebMarAprMayJunJulAugSepOctNovDec";
						STATIC_CONSTANT int day[] = {0,31,59,90,120,151,181,212,243,273,304,334};
						char	mon[4], *moptr, ampm[3];

						len=sizeof(d_null);
						dtype = DTYPE_FT;
						if (status < 0) {buf = (char *)&d_null; break;}
						ddate[status] = '\0';
						if (type == SYBDATETIME)
							sscanf(ddate, "%3s %2d %4d %2d:%2d:%2d:%3d%2s",
							mon, &da, &yr, &hr, &mi, &se, &th, ampm);
						else {
							sscanf(ddate, "%3s %2d %4d %2d:%2d%2s",
								mon, &da, &yr, &hr, &mi, ampm);
							se = 0;
							th = 0;
						}
						if (hr == 12) hr = 0;
						if (ampm[0] == 'P') hr += 12;
						moptr = strstr(moname, mon);
						if (moptr) mo = (moptr - moname)/3; else mo = 0;
						leap = yr/4 - yr/100 + yr/400;
						leap -= mo < 2 && yr%4 == 0 && (yr%100 != 0 || yr%400 == 0);
						d_ans = (double)(yr * 365 + day[mo] + da + leap - 678941);
						d_ans += (double)(th + 1000*(se + 60*(mi + 60*hr)))/86400000.;
						buf = (char *)&d_ans;
#endif
#else
						d_ans=d_null;
                                                if (bufs[j].len == 8) {
						  d_ans=(double)(((int *)buf)[0]+15020)+((double)((unsigned int *)buf)[1])/300./60./60./24.;
						} else if (bufs[j].len == 4) {
						  d_ans=(double)(((unsigned short *)buf)[0]+15020)+((double)((unsigned short *)buf)[1])/60./24.;
						}
						buf = (char *)&d_ans;
						len = sizeof(d_ans);
#endif
						AppendAnswer(j, buf, len, dtype);
					}
					else {	
						/***convert to text***/
						if (!ind) 
						{
							int status;
#ifdef WORDS_BIGENDIAN
							unsigned long hi = *(unsigned long *)buf;
							*(unsigned long *)buf = *((unsigned long *)buf+1);
							*((unsigned long *)buf+1) = hi;
#endif
                                                        if (bufs[j].len != 0) {
							  status = SYB_dbconvert(dbproc,type,buf,bufs[j].len,SYBCHAR,(unsigned char *)ddate,sizeof(ddate)-1);
							  if (status >= 0) {
							    ddate[status] = '\0'; 
							    len = status;
							  } else {
							    strcpy(ddate, "FAILED");
							    len = strlen(ddate);
							  }
							  buf = ddate;
							  dtype = DTYPE_T; 
							}
							else {
							  buf = (char *)default_date;
							  dtype = DTYPE_T;
							  len = strlen(default_date);
							}
						
							AppendAnswer(j, buf, len, dtype);
						}
                                                else {
						  buf = (char *)default_date;
						  dtype = DTYPE_T;
						  len = strlen(default_date);
						  AppendAnswer(j, buf, len, dtype);
						}
					}
                                        break;
				case SYBMONEY:
				default :
					status = TdiINVDTYDSC;
					break;
					}
			}
  }
  return status;
}

#define bufchk \
if (pout == pout_end) {\
  return 0;\
}

STATIC_ROUTINE int Gets(pin, pout, nmarks, arg)
char		*pin, *pout;
int		*nmarks;
ARGLIST 	*arg;
{
  struct descriptor_xd	**argv = (struct descriptor_xd **)arg->v, tmp = EMPTY_XD;
  int		used = arg->used, status, i;
  char		*pout_end = pout+MAXPARSE;
  while (*pin)          {	
    bufchk
    switch (*pin) {	
    default :			/* copy text */
      *pout++ = *pin++;
      break;
    case '\'' :			/* quoted string */
      while (*pout++ = *pin) {bufchk if (*++pin == '\'') break; }
      break;
    case '"' :			/* quoted string */
      while (*pout++ = *pin) {bufchk if (*++pin == '"') break; }
      break;
    case '?' : ++pin;		/* parameter marker */
      ++*nmarks;
      if (used >= arg->c) {
	/*	   sprintf(hold, "Expect >= %d parameters, %d given.", arg->used, arg->c);  */
	return TdiMISS_ARG;
      }
      if (argv
	  && ((status = TdiData(*argv, &tmp MDS_END_ARG)) & 1)
	  && (tmp.pointer->length)
	  && ((status = TdiText(&tmp, &tmp MDS_END_ARG)) & 1)) {
	for (i=0; i<tmp.pointer->length; i++) {
	  bufchk
	  *pout++ = *(char *)(tmp.pointer->pointer+i);
	}
	MdsFree1Dx(&tmp, NULL);
	argv++;
      }
      used++;
      break;
    }
  }
  arg->used += used;
  *pout = '\0';
  return 1;
}

/*********************************************************/
TdiRefStandard(Tdi1Dsql)
int		rows = 0;
ARGLIST		user_args = {0};
struct descriptor dtext = {0,DTYPE_T,CLASS_D,0};
struct descriptor dq_text = {0,DTYPE_T,CLASS_D,0};
struct descriptor drows = {sizeof(rows),DTYPE_L,CLASS_S,0};
STATIC_CONSTANT DESCRIPTOR(zero, "\0");
        drows.pointer = (char *)&rows;
	user_args.c = narg-1;
	user_args.v = &list[1];

	if (SQL_DYNAMIC == 0) {
	  status = TDISQL_LINK("SQL_DYNAMIC", &SQL_DYNAMIC);
	  if ((status & 1) && (SQL_GETDBMSGTEXT == 0)) TDISQL_LINKCPTR("GetDBMsgText", &SQL_GETDBMSGTEXT);
	  if ((status & 1) && (SYB_dbconvert == 0)) TDISQL_LINK("dbconvert", &SYB_dbconvert);
	  if ((status & 1) && (SYB_dbnumcols == 0)) TDISQL_LINK("dbnumcols", &SYB_dbnumcols);
	  if ((status & 1) && (SYB_dbcolname == 0)) TDISQL_LINKCPTR("dbcolname", &SYB_dbcolname);
	  if ((status & 1) && (SYB_dbcoltype == 0)) TDISQL_LINK("dbcoltype", &SYB_dbcoltype);
	  if ((status & 1) && (SYB_dbdatlen == 0)) TDISQL_LINK("dbdatlen", &SYB_dbdatlen);
	  if ((status & 1) && (SYB_dbdata == 0)) TDISQL_LINKCPTR("dbdata", &SYB_dbdata);
	}        
        if (status & 1) status = TdiData(list[0], &dtext MDS_END_ARG);
        status = StrConcat((struct descriptor *)&dtext, (struct descriptor *)&dtext, &zero MDS_END_ARG); 

        /* see if the last argument is "/date" */
        if (status & 1) {
          int ss = TdiData(list[narg-1], &dq_text MDS_END_ARG);
          if (ss) {
	    if (dq_text.length == 5)
	      date = (strncmp("/date", dq_text.pointer, 5) == 0);
            else
	      date = 0;
	    StrFree1Dx( &dq_text);
          } else date = 0;
        }

        /* see if the 2nd to last argument is "/NaN" */
        if ((status & 1) && (narg > 2)) {
          int ss = TdiData(list[narg-2], &dq_text MDS_END_ARG);
          if (ss) {
	    if (dq_text.length == 4)
	      NaN = (strncmp("/NaN", dq_text.pointer, 4) == 0);
            else
	      NaN = 0;
	    StrFree1Dx( &dq_text);
          } else NaN = 0;
        } else NaN = 0;

 
	if (status & 1) status = SQL_DYNAMIC(
		Gets,	/*routine to fill markers	*/
		Puts,	/*routine to store selctions	*/
		dtext.pointer,		/*text string descriptor	*/
		&user_args,	/*value passed to GETS and PUTS	*/
		&rows);		/*output, number of rows	*/

	StrFree1Dx(&dtext);
	if (status & 1) {
	  status = MdsCopyDxXd(&drows, out_ptr);
	} else {
          struct descriptor msg = {0, DTYPE_T, CLASS_S, 0};
	  msg.pointer = SQL_GETDBMSGTEXT();
	  msg.length = strlen(msg.pointer); 
	  status = MdsCopyDxXd(&msg, out_ptr);
	}
	return status;
}
/*********************************************************/
TdiRefStandard(Tdi1Isql)
ARGLIST		user_args = {0};
int		rows = 0;
struct descriptor dtext = {0,DTYPE_T,CLASS_D,0};
struct descriptor drows = {sizeof(rows),DTYPE_L,CLASS_S,0};
        drows.pointer = (char *)&rows;
	user_args.c = narg-1;
	user_args.v = &list[1];
	if (USERSQL_SET == 0) {
		if (status & 1) status = TDISQL_LINK("SQL_DYNAMIC", &SQL_DYNAMIC);
		if (status & 1) status = TDISQL_LINK("USERSQL_ERRORS", &USERSQL_ERRORS);
		if (status & 1) status = TDISQL_LINK("USERSQL_GETS", &USERSQL_GETS);
		if (status & 1) status = TDISQL_LINK("USERSQL_PUTS", &USERSQL_PUTS);
		if (status & 1) status = TDISQL_LINK("USERSQL_SET", &USERSQL_SET);
	}

#ifdef __VMS
	if (width == 0) 
        {
          STATIC_CONSTANT DESCRIPTOR(output_device,	"SYS$OUTPUT");
          LibGetDvi(&DVI$_DEVBUFSIZ, 0, &output_device, &width, 0, 0);
        }
#endif
        width = width > 0 ? width : 132;
	if (status & 1) status = TdiData(list[0], &dtext MDS_END_ARG);
	if (status & 1) {
		USERSQL_SET(gets, printf, printf, printf, width, head);
		status = SQL_DYNAMIC(
			USERSQL_GETS,	/*routine to fill markers	*/
			USERSQL_PUTS,	/*routine to store selections	*/
			&dtext,		/*text string descriptor	*/
			&width,		/*value passed to GETS and PUTS	*/
			&rows);		/*output, number of rows	*/
		if (!(status & 1)) USERSQL_ERRORS();
	}
	StrFree1Dx(&dtext);
	if (status & 1) status = MdsCopyDxXd(&drows, out_ptr);
	FreeBuffers();

	return status;
}
/*********************************************************/
TdiRefStandard(Tdi1IsqlSet)
	if (narg >= 1 && list[0])		status = TdiGetLong(list[0], &width);
	if (narg >= 2 && list[1] && status & 1) status = TdiGetLong(list[1], &head);
	return status;
}
#else /* no sybase support */
STATIC_CONSTANT DESCRIPTOR(const msg, "Sybase support not compiled into TDI.  Did you want to MDSConnect ?");

TdiRefStandard(Tdi1Dsql)
     status = MdsCopyDxXd((struct descriptor *)&msg, out_ptr);
     return status;
}
TdiRefStandard(Tdi1Isql)
     status = MdsCopyDxXd((struct descriptor *)&msg, out_ptr);
     return status;
}
TdiRefStandard(Tdi1IsqlSet)
     status = MdsCopyDxXd((struct descriptor *)&msg, out_ptr);
     return status;
}
#endif
