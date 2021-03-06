/*	Tdi1Var.C
	Find a variable by name and other goodies.
	Function definition and execution:
		FUN [PRIVATE|PUBLIC] ident ([OPTIONAL][AS_IS|IN|INOUT|OUT] ident...) stmt
		var (var...)			invocation
	Inquires:
		ALLOCATED (var)			test defined
		PRESENT (var)			test in argument list or defined
	Fetches (these are the "var"):
		[PRIVATE|PUBLIC] ident		by data type / special function
		VAR ("text expression")		fetch by expression
	Stores:
		var = expression		assignment
		var += expression		binary operator and assignment
		VAR ("text", expr)		fetch/store by text expression
	Changes:
		++ [PRIVATE|PUBLIC] var		pre-increment
		-- [PRIVATE|PUBLIC] var		pre-decrement
		[PRIVATE|PUBLIC] var ++		post-increment
		[PRIVATE|PUBLIC] var --		post-decrement
		DEALLOCATE (var...)		free data field
	Utilities:
		TdiFindIdent(search, var, [key], [node_ptr], [block_ptr])
		TdiGetIdent(var, data)	fetch variable
		TdiPutIdent(var, data)	store variable
		RESET_PRIVATE()	RESET_PUBLIC()	release variables
		SHOW_PRIVATE([text],...)
		SHOW_PUBLIC([text],...)		display variables

	FUN arguments declared:
		AS_IS overrides evaluation.
		not OPTIONAL must be present and not null.
		IN are evaluated and must not be changed.
		INOUT or OUT must be variables.
		OUT are initially undefined and may not be evaluated until defined.
	Ken Klare, LANL P-4	(c)1989,1990,1991,1992
	NEED subscripted assignment.
*/
#include "STATICdef.h"
#include "tdithreadsafe.h"
#include "tdirefstandard.h"
#include <libroutines.h>
#include <strroutines.h>
#include <stdio.h>

#define LibVM_EXTEND_AREA 32
#define LibVM_TAIL_LARGE 128
#include <tdimessages.h>
#include <stdlib.h>
#include <mdsshr.h>
#include <string.h>
#include <librtl_messages.h>

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: TdiVar.c,v $ $Revision: 1.20 $ $Date: 2010/01/20 20:52:41 $";

extern unsigned short OpcEquals, OpcEqualsFirst;
extern unsigned short OpcFun;
extern unsigned short OpcIn, OpcInOut, OpcOptional, OpcOut, OpcAsIs;
extern unsigned short OpcPostDec, OpcPostInc, OpcPreDec, OpcPreInc;
extern unsigned short OpcPrivate, OpcPublic, OpcVar;

extern int TdiFaultHandler();
extern int TdiData();
extern int TdiDeallocate();
extern int TdiDecompile();
extern int TdiEvaluate();
extern int TdiPutLong();
extern int TdiEquals();
extern int TdiSubtract();
extern int TdiAdd();

	/*******************************************
	This uses the balanced binary tree routines.
	LOOKUP_TREE INSERT_TREE TRAVERSE_TREE
	Full descriptors cost memory, save time.
	*******************************************/
typedef struct link {
	struct link			*left, *right;	/* binary tree links */
	unsigned short			reserved;	/* tree flags */
	struct descriptor_xd	xd;		/* data descriptor */
	struct descriptor		name_dsc;	/* name descriptor */
	unsigned char			name[1];	/* unchanging name */
} node_type;
typedef struct {
	node_type		*head;		/* changing binary-tree head */
	void			*head_zone;	/* created on first call */
	void			*data_zone;	/* created on first call */
	int                     _public;		/* is public */
} block_type;
typedef struct {
	struct descriptor	match;
	block_type		*block_ptr;
	int			count;
} user_type;

STATIC_CONSTANT struct descriptor EMPTY_D = {0,DTYPE_T,CLASS_D,0};
STATIC_CONSTANT struct descriptor EMPTDY_S = {0,DTYPE_T,CLASS_S,0};
STATIC_CONSTANT struct descriptor_xd	NULL_XD = {0,0,0,0,0};
STATIC_CONSTANT unsigned char true = 1;
STATIC_CONSTANT struct descriptor true_dsc = {sizeof(true),DTYPE_BU,CLASS_S,(char *)&true};
STATIC_THREADSAFE pthread_mutex_t lock;
STATIC_THREADSAFE int lock_initialized=0;
STATIC_THREADSAFE block_type _public	= {0,0,0,1};
STATIC_CONSTANT DESCRIPTOR(star, "*");
STATIC_CONSTANT DESCRIPTOR(percent, "%");
struct descriptor *Tdi3Narg()
{
  return &(TdiThreadStatic())->TdiVar_new_narg_d;
}

/*--------------------------------------------------------------
	Put a byte to output.
*/
int			TdiPutLogical(
unsigned char		data,
struct descriptor_xd *out_ptr) {
int	status;
STATIC_CONSTANT unsigned short len = (unsigned short)sizeof(unsigned char);
STATIC_CONSTANT unsigned char dtype = (unsigned char)DTYPE_BU;
	if (out_ptr == 0) return 0;
	status = MdsGet1DxS(&len, &dtype, out_ptr);
	if (status & 1) *(unsigned char *) out_ptr->pointer->pointer = data;
	return status;
}
/*--------------------------------------------------------------
	Comparison routine.
*/
STATIC_ROUTINE int		compare(
struct descriptor	*key_ptr,
node_type		*node_ptr,
block_type		*block_ptr)
{
	return StrCaseBlindCompare(key_ptr, &node_ptr->name_dsc);
}
/*--------------------------------------------------------------
	Allocation routine, does not set data field.
	ASSUMED called only once per node.
*/
STATIC_ROUTINE int		allocate(
struct descriptor	*key_ptr,
node_type		**node_ptr_ptr,
block_type		*block_ptr)
{
int			status = 1, len = sizeof(struct link) - 1 + key_ptr->length;

	/*******************************************************************
	Must clear memory unless allocate is called only once for each node.
	*******************************************************************/
	if (block_ptr->data_zone == 0) {
		LibCreateVmZone(&block_ptr->head_zone);
		LibCreateVmZone(&block_ptr->data_zone);
	}
	status = LibGetVm(&len, node_ptr_ptr, &block_ptr->head_zone);
	if (status & 1) {
	node_type *node_ptr = *node_ptr_ptr;
		node_ptr->xd = EMPTY_XD;
		node_ptr->name_dsc = *key_ptr;
		node_ptr->name_dsc.pointer = (char *)&node_ptr->name[0];
                memcpy(node_ptr->name, key_ptr->pointer, key_ptr->length);
	}
	return status;
}
/*--------------------------------------------------------------
	Find active variable.
	Look in private and public lists.
	search: 1=public 2=private 4=check that it exists.
*/
STATIC_ROUTINE int		TdiFindIdent(
int				search,
struct descriptor_r		*ident_ptr,
struct descriptor		*key_ptr,
node_type			**node_ptr_ptr,
block_type			**block_ptr_ptr)
{
  struct descriptor		key_dsc, name_dsc;
  node_type			*node_ptr;
  block_type			*block_ptr;
  int				code, status = 1;
  block_type *private = (block_type *)&(TdiThreadStatic())->TdiVar_private;

	if (ident_ptr == 0) status = TdiNULL_PTR;
	else switch (ident_ptr->class) {
	case CLASS_S :
	case CLASS_D :
		switch (ident_ptr->dtype) {
		/********************************************
		The choice of default public/private is here.
		********************************************/
		case DTYPE_T :
		case DTYPE_IDENT :
			block_ptr = private;
			key_dsc = *(struct descriptor *)ident_ptr;
			key_dsc.dtype = DTYPE_T;
			key_dsc.class = CLASS_S;
			if (search & 4) {
				if (search & 1) {
					status = LibLookupTree(&private->head, &key_dsc, compare, &node_ptr);
					if (status & 1)
					if (node_ptr->xd.class != 0) break;
					else status = TdiUNKNOWN_VAR;
				}
				if (search & 2) {
					status = LibLookupTree(&_public.head, &key_dsc, compare, &node_ptr);
					if (status & 1)
					if (node_ptr->xd.class != 0) block_ptr = &_public;
					else status = TdiUNKNOWN_VAR;
				}
			}
			else if (search & 1) ;
			else if (search & 2) block_ptr = &_public;
			break;
		default :
			status = TdiINVDTYDSC;
			break;
		}
		break;
	case CLASS_R :
		if (ident_ptr->dtype != DTYPE_FUNCTION) {status = TdiINVCLADTY; break;}
		code = *(unsigned short *)ident_ptr->pointer;
		if (ident_ptr->ndesc < 1) {status = TdiMISS_ARG; break;}
		/***************************************
		Could have (public _a+=6)++ as argument.
		So strip ++ and += and use public.
		***************************************/
		if (code == OpcPublic)
			status = TdiFindIdent(search & ~1, (struct descriptor_r *)ident_ptr->dscptrs[0], 
                                                &key_dsc, &node_ptr, &block_ptr);
		else if (code == OpcPrivate)
			status = TdiFindIdent(search & ~2, (struct descriptor_r *)ident_ptr->dscptrs[0],
                                                &key_dsc, &node_ptr, &block_ptr);
		else if (code == OpcEquals || code == OpcEqualsFirst
		|| code == OpcPostDec || code == OpcPreDec
		|| code == OpcPostInc || code == OpcPreInc) {
		struct descriptor_xd tmp = EMPTY_XD;
			status = TdiEvaluate(ident_ptr, &tmp MDS_END_ARG);
			if (status & 1) status = TdiFindIdent(search, (struct descriptor_r *)ident_ptr->dscptrs[0],
				&key_dsc, &node_ptr, &block_ptr);
		}
		else if (code == OpcVar) {
			name_dsc = EMPTY_D;
			status = TdiData(ident_ptr->dscptrs[0], &name_dsc MDS_END_ARG);
			if (status & 1) status = TdiFindIdent(search, (struct descriptor_r *)&name_dsc, 
                                &key_dsc, &node_ptr, &block_ptr);
			StrFree1Dx(&name_dsc);
		}
		else	status = TdiINV_OPC;
		break;
	default :
		status = TdiINVCLADSC;
		break;
	}
	if (status == LibKEYNOTFOU) status = TdiUNKNOWN_VAR;
	if (status & 1 || status == TdiUNKNOWN_VAR) {
		if (key_ptr) *key_ptr = key_dsc;
		if (node_ptr_ptr) *node_ptr_ptr = node_ptr;
		if (block_ptr_ptr) *block_ptr_ptr = block_ptr;
	}
	return status;
}
/*--------------------------------------------------------------
	Get active variable for VAR and EVALUATE. Copies to MDSSHR space.
*/
int				TdiGetIdent(
struct descriptor		*ident_ptr,
struct descriptor_xd	*data_ptr)
{
  node_type			*node_ptr;
  int				status;

  LockTdiMutex(&lock,&lock_initialized);
	status = TdiFindIdent(7, (struct descriptor_r *)ident_ptr, 0, &node_ptr, 0);
	if (status & 1) status = MdsCopyDxXd(node_ptr->xd.pointer, data_ptr);
	else MdsFree1Dx(data_ptr, NULL);
	UnlockTdiMutex(&lock);
	return status;
}
/*--------------------------------------------------------------
	Keep an XD-DSC of whatever.
	Assumes we are given XD or class-0.
*/
int				TdiPutIdent(
struct descriptor_r		*ident_ptr,
struct descriptor_xd	*data_ptr)
{
struct descriptor		key_dsc = EMPTDY_S;
node_type			*node_ptr;
block_type			*block_ptr;
 struct descriptor               upstr = {0,DTYPE_T,CLASS_D,0};
int				size = 0, status;
STATIC_CONSTANT int zero = 0;
 if (ident_ptr->dtype == DTYPE_DSC)
   return TdiPutIdent((struct descriptor_r *)ident_ptr->pointer,data_ptr);
  LockTdiMutex(&lock,&lock_initialized);

	/************************************
	Find where we should place the stuff.
	************************************/
	status = TdiFindIdent(3, ident_ptr, &key_dsc, 0, &block_ptr);
        StrUpcase(&upstr,&key_dsc);
	if (status & 1) status = LibInsertTree(&block_ptr->head, &upstr, &zero, compare, allocate, &node_ptr, block_ptr); 
        StrFree1Dx(&upstr);
	if (status & 1) {
		if (node_ptr->xd.class == 0) 
                  node_ptr->xd = EMPTY_XD;
		if (data_ptr) 
                  status = MdsCopyDxXdZ(data_ptr->pointer, &node_ptr->xd, &block_ptr->data_zone, 
						NULL, NULL, NULL, NULL);
		else
		{
                  MdsFree1Dx(&node_ptr->xd,&block_ptr->data_zone);
                  node_ptr->xd = NULL_XD;
                }
	}
	UnlockTdiMutex(&lock);
	return status;
}
/***************************************************************
	Display/free wildcarded variables.
	IDENT names are not evaluated.
	PUBLIC or PRIVATE(ident or text) overrides block_ptr.
*/
STATIC_ROUTINE int		wild(
int			(* doit)(),
int			narg,
struct descriptor	*list[],
block_type		*block_ptr,
struct descriptor_xd *out_ptr)
{
  node_type		*node_ptr;
  user_type		user = {{0,DTYPE_T,CLASS_D,0},0,0};
  int			j, status = 1;
  LockTdiMutex(&lock,&lock_initialized);
  user.block_ptr = block_ptr;
	if (narg == 0 || list[0] == 0) status = LibTraverseTree(&block_ptr->head, doit, &user);
	else for (j = 0; status & 1 && j < narg; ++j) {
		status = TdiFindIdent(3, (struct descriptor_r *)list[j], &user.match, 0, &user.block_ptr);
		if (status & 1) status = StrUpcase(&user.match, &user.match);
		if (status & 1)
		if (StrPosition(&user.match, &star, 0) || StrPosition(&user.match, &percent, 0))
			status = LibTraverseTree(&user.block_ptr->head, doit, &user);
		else {
			status = LibLookupTree(&user.block_ptr->head, &user.match, compare, &node_ptr);
			if (status & 1) status = (* doit)(node_ptr, &user);
			else if (status == LibKEYNOTFOU) status = 1;
		}
	}
	if (user.match.class == CLASS_D) StrFree1Dx(&user.match);
	if (status & 1) status = TdiPutLong(&user.count, out_ptr);
	UnlockTdiMutex(&lock);
	return status;
}
/*--------------------------------------------------------------
	Release variables.
*/
STATIC_ROUTINE int		free_one(
node_type		*node_ptr,
user_type		*user_ptr)
{
block_type		*block_ptr = user_ptr->block_ptr;
int			status = 1;

	/***************************************************
	Deallocated variables are not displayed or re-freed.
	***************************************************/
	if (node_ptr->xd.class == 0) return 1;
	if (user_ptr->match.length == 0) status = 1;
	else status = StrMatchWild(&node_ptr->name_dsc, &user_ptr->match);
	if (status & 1) {
		if (node_ptr->xd.l_length) status = LibFreeVm(
			&node_ptr->xd.l_length,
			&node_ptr->xd.pointer,
			&block_ptr->data_zone);
		node_ptr->xd = NULL_XD;
		user_ptr->count++;
	}
	else 
          status = 1;
	return status;
}
/*--------------------------------------------------------------
	Release all variables under a header and the headers too.
	Used to release the FUN variables.
*/
STATIC_ROUTINE int free_all(node_type **pnode) {
int	status = 1, stat2, len;
block_type *private = (block_type *)&((TdiThreadStatic())->TdiVar_private);

	if ((*pnode)->xd.l_length) status = LibFreeVm(
		&(*pnode)->xd.l_length,
		&(*pnode)->xd.pointer,
		&private->data_zone);
	if ((*pnode)->left) {
		stat2 = free_all(&((*pnode)->left));
		if (status & 1) status = stat2;
	}
	if ((*pnode)->right) {
		stat2 = free_all(&((*pnode)->right));
		if (status & 1) status = stat2;
	}
	len = sizeof(struct link) - 1 + (*pnode)->name_dsc.length;
	stat2 = LibFreeVm(&len, pnode, &private->head_zone);
        *pnode = 0;
	if (status & 1) status = stat2;
	return status;
}
/*--------------------------------------------------------------
	Release variables.
*/
TdiRefStandard(Tdi1Deallocate)
block_type *private = (block_type *)&((TdiThreadStatic())->TdiVar_private);
	if (narg == 0 && private->head) {
           status = free_all(&private->head); 
           private->head = 0;
           return status;
        }
	return wild((int (*)())free_one, narg, list, private, out_ptr);
}
/*--------------------------------------------------------------
	Check for allocated variable, private only by default.
*/
TdiRefStandard(Tdi1Allocated)
struct descriptor	key_dsc = EMPTDY_S;
node_type		*node_ptr;
block_type		*block_ptr;
int			found;

  LockTdiMutex(&lock,&lock_initialized);
	status = TdiFindIdent(3, (struct descriptor_r *)list[0], &key_dsc, 0, &block_ptr);
	if (status & 1) status = LibLookupTree(&block_ptr->head, &key_dsc, compare, &node_ptr);
        found = status & 1;
	if (found) found = node_ptr->xd.class != 0;
	else if (status == LibKEYNOTFOU || status == TdiUNKNOWN_VAR) status = 1;
	if (status & 1) status = TdiPutLogical((unsigned char)found, out_ptr);
	UnlockTdiMutex(&lock);
	return status;
}
/*--------------------------------------------------------------
	Check for argument present.
*/
TdiRefStandard(Tdi1Present)
struct descriptor	key_dsc = EMPTDY_S;
node_type		*node_ptr;
block_type		*block_ptr;
int			found;
  LockTdiMutex(&lock,&lock_initialized);
	status = TdiFindIdent(3, (struct descriptor_r *)list[0], &key_dsc, 0, &block_ptr);
	if (status & 1) status = LibLookupTree(&block_ptr->head, &key_dsc, compare, &node_ptr);
        found = status & 1;
	if (found) ;
	else if (status == LibKEYNOTFOU || status == TdiUNKNOWN_VAR) status = 1;
	if (status & 1) status = TdiPutLogical((unsigned char)found, out_ptr);
	UnlockTdiMutex(&lock);
	return status;
}
/***************************************************************
	Execute a function. Set up the arguments.
*/

int				TdiDoFun(
struct descriptor		*ident_ptr,
int				nactual,
struct descriptor_r		*actual_arg_ptr[],
struct descriptor_xd	*out_ptr)
{
node_type			*node_ptr;
struct descriptor_r		*formal_ptr=0, *formal_arg_ptr, *actual_ptr;
struct descriptor_xd	tmp = EMPTY_XD;
node_type			*old_head, *new_head = 0;
int				code, opt, j, nformal=0, status;
int    *new_narg = &(TdiThreadStatic())->TdiVar_new_narg;
int				old_narg = *new_narg;
block_type *private = (block_type *)&((TdiThreadStatic())->TdiVar_private);

	/******************************************
	Get name of function to do. Check its type.
	******************************************/

	status = TdiFindIdent(7, (struct descriptor_r *)ident_ptr, 0, &node_ptr, 0);
	if (!(status & 1)) ;
	else if ((formal_ptr = (struct descriptor_r *)node_ptr->xd.pointer) == 0
	|| formal_ptr->dtype != DTYPE_FUNCTION
	|| *(unsigned short *)formal_ptr->pointer != OpcFun) status = TdiUNKNOWN_VAR;
	else if ((nformal = formal_ptr->ndesc - 2) < nactual) status = TdiEXTRA_ARG;
	/**************************************
	Now copy input arguments into new head.
	Pick up some keywords from prototype.
	Only OPTIONAL arguments may be omitted.
	OUT arguments are not evaluated.
	**************************************/
	for (j = 0; j < nformal && status & 1; ++j) {
		formal_arg_ptr = (struct descriptor_r *)formal_ptr->dscptrs[j+2];
		if (formal_arg_ptr == 0) {status = TdiNULL_PTR; break;}
		if (formal_arg_ptr->dtype == DTYPE_FUNCTION
		&& formal_arg_ptr->ndesc == 1) {
                        code = *(unsigned short *)formal_arg_ptr->pointer;
                        opt = (code == OpcOptional);
			if (opt) {
				formal_arg_ptr = (struct descriptor_r *)formal_arg_ptr->dscptrs[0];
				if (formal_arg_ptr == 0) {status = TdiNULL_PTR; break;}
				if (formal_arg_ptr->dtype != DTYPE_FUNCTION
				|| formal_arg_ptr->ndesc != 1) code = 0;
				else code = *(unsigned short *)formal_arg_ptr->pointer;
			}
		}
		else code = opt = 0;
		/****************************
		[OPTIONAL] AS_IS IN INOUT OUT only.
		All other words are bad here.
		****************************/
		if (code == OpcIn || code == OpcInOut || code == OpcOut) {
			formal_arg_ptr = (struct descriptor_r *)formal_arg_ptr->dscptrs[0];
			if (formal_arg_ptr->dtype != DTYPE_T) {status = TdiINVDTYDSC; break;}
		}
		else if (code == OpcAsIs) formal_arg_ptr = (struct descriptor_r *)formal_arg_ptr->dscptrs[0];
		else if (formal_arg_ptr->dtype != DTYPE_IDENT
		&& (formal_arg_ptr->dtype != DTYPE_T || !opt)) {status = TdiINVDTYDSC; break;}
		/******************************
		OUT is present, but undefined.
		Eval others with old vari list.
		******************************/
		if (j >= nactual || (actual_ptr = actual_arg_ptr[j]) == 0) {
			if (!opt) status = TdiMISS_ARG;
		}
		else if (code == OpcOut) {
			old_head = private->head; private->head = new_head;
			status = TdiPutIdent(formal_arg_ptr, 0);
			new_head = private->head; private->head = old_head;
		}
		else {
			if (code == OpcAsIs) {
				status = MdsCopyDxXd((struct descriptor *)actual_ptr, &tmp);
			}
			else if (actual_ptr->dtype == DTYPE_IDENT) {
				status = TdiGetIdent((struct descriptor *)actual_ptr, &tmp);
				if (opt && status == TdiUNKNOWN_VAR) status = 1;
			}
			else {
				status = TdiEvaluate(actual_ptr, &tmp MDS_END_ARG);
				if (!opt && status & 1 && tmp.pointer == 0) status = TdiMISS_ARG;
			}
			/**************************
			Do this with new variables.
			**************************/
			old_head = private->head; private->head = new_head;
			if (status & 1) status = TdiPutIdent(formal_arg_ptr, &tmp);
			new_head = private->head; private->head = old_head;
		}
	}
	/**************************
	Do this with new variables.
	**************************/
	old_head = private->head; private->head = new_head;
	*new_narg = nactual;
	if (status & 1) {
		status = TdiEvaluate(formal_ptr->dscptrs[1], out_ptr MDS_END_ARG);
		if (status == TdiRETURN) status = 1;
	}
	/*****************************
	Send outputs back up the line.
	*****************************/
	for (j = 0; j < nactual && status & 1; ++j) {
		formal_arg_ptr = (struct descriptor_r *)formal_ptr->dscptrs[j+2];
		if (actual_arg_ptr[j]
		&& formal_arg_ptr->dtype == DTYPE_FUNCTION
		&& formal_arg_ptr->ndesc == 1) {
			if ((code = *(unsigned short *)formal_arg_ptr->pointer) == OpcOptional) {
				formal_arg_ptr = (struct descriptor_r *)formal_arg_ptr->dscptrs[0];
				if (formal_arg_ptr->dtype != DTYPE_FUNCTION
				|| formal_arg_ptr->ndesc != 1) code = 0;
				else code = *(unsigned short *)formal_arg_ptr->pointer;
			}
			if (code == OpcInOut || code == OpcOut) {
				status = TdiFindIdent(7, (struct descriptor_r *)formal_arg_ptr->dscptrs[0], 0, 
                                          &node_ptr, 0);
				/**************************
				Do this with old variables.
				**************************/
				new_head = private->head; private->head = old_head;
				if (status & 1) status = TdiPutIdent(actual_arg_ptr[j], &node_ptr->xd);
				old_head = private->head; private->head = new_head;
			}
		}
	}
	TdiDeallocate(&tmp MDS_END_ARG);
	MdsFree1Dx(&tmp, NULL);
	private->head = old_head;
	*new_narg = old_narg;
	return status;
}
/***************************************************************
	Replace variable. Assumed called by INTRINSIC so that out_ptr is XD-DSC.
*/
TdiRefStandard(Tdi1Equals)

	status = TdiEvaluate(list[1], out_ptr MDS_END_ARG);
	/************************************
	Place the data or clear the variable.
	************************************/
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], out_ptr);
	else TdiPutIdent((struct descriptor_r *)list[0], 0);
	return status;
}
/*--------------------------------------------------------------
	Store in first argument of binary opertor.
*/
TdiRefStandard(Tdi1EqualsFirst)

	if (list[0]->dtype != DTYPE_FUNCTION
	|| ((struct descriptor_function *)list[0])->ndesc != 2) status = TdiINVDTYDSC;
	else status = TdiEquals(((struct descriptor_r *)list[0])->dscptrs[0], list[0], out_ptr MDS_END_ARG);
	return status;
}
/*--------------------------------------------------------------
	Decrement a variable before use.
*/
TdiRefStandard(Tdi1PreDec)

	status = TdiSubtract(list[0], &true_dsc, out_ptr MDS_END_ARG);
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], out_ptr);
	return status;
}
/*--------------------------------------------------------------
	Increment a variable before use.
*/
TdiRefStandard(Tdi1PreInc)

	status = TdiAdd(list[0], &true_dsc, out_ptr MDS_END_ARG);
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], out_ptr);
	return status;
}
/*--------------------------------------------------------------
	Decrement a variable after use.
*/
TdiRefStandard(Tdi1PostDec)
struct descriptor_xd	tmp = EMPTY_XD;

	status = TdiGetIdent(list[0], out_ptr);
	if (status & 1) status = TdiSubtract(out_ptr->pointer, &true_dsc, &tmp MDS_END_ARG);
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], &tmp);
	MdsFree1Dx(&tmp, NULL);
	return status;
}
/*--------------------------------------------------------------
	Increment a variable after use.
*/
TdiRefStandard(Tdi1PostInc)
struct descriptor_xd	tmp = EMPTY_XD;

	status = TdiGetIdent(list[0], out_ptr);
	if (status & 1) status = TdiAdd(out_ptr->pointer, &true_dsc, &tmp MDS_END_ARG);
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], &tmp);
	MdsFree1Dx(&tmp, NULL);
	return status;
}
/***************************************************************
	Find by identifier.
	PRIVATE and PUBLIC must have text argument, not expression.
	They are keywords, not standard functions. NEED we change this?
*/
TdiRefStandard(Tdi1Private)
node_type	*node_ptr;
block_type *private = (block_type *)&((TdiThreadStatic())->TdiVar_private);

	status = LibLookupTree(&private->head, list[0], compare, &node_ptr);
	if (status & 1) status = MdsCopyDxXd(node_ptr->xd.pointer, out_ptr);
	else if (status == LibKEYNOTFOU) status = TdiUNKNOWN_VAR;
	return status;
}
/*--------------------------------------------------------------
	Find by identifier.
	PRIVATE and PUBLIC must have text argument, not expression.
*/
TdiRefStandard(Tdi1Public)
node_type	*node_ptr;
  LockTdiMutex(&lock,&lock_initialized);

	status = LibLookupTree(&_public.head, list[0], compare, &node_ptr);
	if (status & 1) status = MdsCopyDxXd(node_ptr->xd.pointer, out_ptr);
	else if (status == LibKEYNOTFOU) status = TdiUNKNOWN_VAR;
	UnlockTdiMutex(&lock);
	return status;
}
/*--------------------------------------------------------------
	Find by text expression.
*/
TdiRefStandard(Tdi1Var)
struct descriptor_xd	tmp = EMPTY_XD;

	status = TdiData(list[0], &tmp MDS_END_ARG);
	if (status & 1) {
		if (narg < 2 || list[1] == 0) status = TdiGetIdent(tmp.pointer, out_ptr);
		else status = TdiEquals(tmp.pointer, list[1], out_ptr MDS_END_ARG);
	}
	MdsFree1Dx(&tmp, NULL);
	return status;
}
/***************************************************************
	Define a function by reconstruction.
*/
TdiRefStandard(Tdi1Fun)
DESCRIPTOR_FUNCTION(hold,0,255);
int			j;
unsigned short opcode_s = (unsigned short)opcode;
        hold.pointer = (unsigned char *)&opcode_s;
        hold.ndesc = (unsigned char)narg;
	for (j = narg; --j >=0;) if ((hold.arguments[j] = list[j]) == 0) return TdiNULL_PTR;
	status = MdsCopyDxXd((struct descriptor *)&hold, out_ptr);
	if (status & 1) status = TdiPutIdent((struct descriptor_r *)list[0], out_ptr);
	return status;
}
/***************************************************************
	Release the private variables.
	This to be used by functions.
*/
TdiRefStandard(Tdi1ResetPrivate)
block_type *private = (block_type *)&((TdiThreadStatic())->TdiVar_private);

	private->head = 0;
	if (private->data_zone) status = LibResetVmZone(&private->data_zone);
	if (private->head_zone) status = LibResetVmZone(&private->head_zone);
	return status;
}
/*--------------------------------------------------------------
	Release the public variables.
*/
TdiRefStandard(Tdi1ResetPublic)

  LockTdiMutex(&lock,&lock_initialized);
	_public.head = 0;
        TdiResetGetRecord();
	if (_public.data_zone) status = LibResetVmZone(&_public.data_zone);
	if (_public.head_zone) status = LibResetVmZone(&_public.head_zone);
	UnlockTdiMutex(&lock);
	return status;
}
/***************************************************************
	Display a variable.
*/
STATIC_ROUTINE int		show_one(
node_type		*node_ptr,
user_type		*user_ptr)
{
int			status;
struct descriptor	tmp = EMPTY_D;
struct descriptor_r	*rptr = (struct descriptor_r *)node_ptr->xd.pointer;

	if (node_ptr->xd.class == 0) return 1;
	if (user_ptr->match.length == 0) status = 1;
	else status = StrMatchWild(&node_ptr->name_dsc, &user_ptr->match);
	if (status & 1) {
		if (rptr) status = TdiDecompile(rptr, &tmp MDS_END_ARG);
		if (status & 1) {
			if (rptr && rptr->dtype == DTYPE_FUNCTION
			&& *(unsigned short *)rptr->pointer == OpcFun)
				printf("%.*s\n", tmp.length, tmp.pointer);
			else if (tmp.length > 0)
				printf("%s %.*s\t= %.*s\n", user_ptr->block_ptr->_public ? "Public" : "Private",
				node_ptr->name_dsc.length, node_ptr->name_dsc.pointer,
				tmp.length, tmp.pointer);
			else
				printf("%s %.*s\t=\n", user_ptr->block_ptr->_public ? "Public" : "Private",
				node_ptr->name_dsc.length, node_ptr->name_dsc.pointer);
			user_ptr->count++;
		}
		else printf("ERROR on %.*s\n", node_ptr->name_dsc.length, node_ptr->name_dsc.pointer);
		StrFree1Dx(&tmp);
	}
	else 
          status = 1;
	return status;
}
/*--------------------------------------------------------------
	Display private variables.
*/
TdiRefStandard(Tdi1ShowPrivate)
block_type *_private = (block_type *)&((TdiThreadStatic())->TdiVar_private);
	return wild((int (*)())show_one, narg, list, _private, out_ptr);
}
/*--------------------------------------------------------------
	Display public variables.
*/
TdiRefStandard(Tdi1ShowPublic)
  LockTdiMutex(&lock,&lock_initialized);
	status = wild((int (*)())show_one, narg, list, &_public, out_ptr);
	UnlockTdiMutex(&lock);
     return status;
}


int TdiSaveContext(void *ptr[6])
{
    block_type *_private;
    LockTdiMutex(&lock,&lock_initialized);
    _private = (block_type *)&((TdiThreadStatic())->TdiVar_private);
    ptr[0] = (void *)_private->head;
    ptr[1] = _private->head_zone;
    ptr[2] = _private->data_zone;
    ptr[3] = (void *)_public.head;
    ptr[4] = _public.head_zone;
    ptr[5] = _public.data_zone;
	  UnlockTdiMutex(&lock);
    return 1;
}

int TdiDeleteContext(void *ptr[6])
{
    if (ptr[1]) LibDeleteVmZone(&ptr[1]);
    if (ptr[2]) LibDeleteVmZone(&ptr[2]);
    if (ptr[4]) LibDeleteVmZone(&ptr[4]);
    if (ptr[5]) LibDeleteVmZone(&ptr[5]);
    return 1;
}

/*-------------------------------------------------------------
        Restore variable context
*/
int TdiRestoreContext(void *ptr[6])
{
    block_type *_private;
    LockTdiMutex(&lock,&lock_initialized);
    _private = (block_type *)&((TdiThreadStatic())->TdiVar_private);
    _private->head = (node_type *)ptr[0];
    _private->head_zone = ptr[1];
    _private->data_zone = ptr[2];
    _public.head = ptr[3];
    _public.head_zone = (node_type *)ptr[4];
    _public.data_zone = ptr[5];
	  UnlockTdiMutex(&lock);
    return 1;
}

