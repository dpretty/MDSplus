/*	Tdi0Decompile_R
	Routine to decompile a descriptor of class R.
	status = Tdi0Decompile_R(in_ptr, precedence, out_ptr);

	Josh Stillerman, MIT-PFC, 28-DEC-1988
	Ken Klare, LANL P-4	(c)1989,1990,1991,1992
*/
#include <STATICdef.h>
#include <string.h>
#include <mdsdescrip.h>
#include "opcopcodes.h"
#include "tdirefcat.h"
#include "tdireffunction.h"
#include "tdirefzone.h"
#include <strroutines.h>
#include <mds_stdarg.h>

#define	P_STMT		96
#define	P_ARG		88
#define P_SUBS		0

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: TdiDecompileR.c,v $ $Revision: 1.4 $ $Date: 2003/11/17 21:21:21 $";

extern unsigned int LEX_CONST;
unsigned int TdiIndent = 1;
extern int Tdi0Decompile(  );
extern int TdiSingle(  );

STATIC_CONSTANT struct op_rec {
	char		*symbol;
	short		opcode;
	char		prec,lorr;
} binary[] = {
	{", "		,OpcComma,		92,-1},
	{" = "		,OpcEquals,		84,1},
	{0		,OpcEqualsFirst,	84,1},
	{" : "		,OpcDtypeRange,	80,0},/*dtype is generated, not opcode*/
	{" ? "		,OpcConditional,	72,1},
	{" @ "		,OpcPromote,		68,1},
	{" Eqv "	,OpcEqv,		64,-1},
	{" Neqv "	,OpcNeqv,		64,-1},
	{" || "		,OpcOr,		60,-1},
	{" Or_Not "	,OpcOrNot,		60,-1},
	{" Nor "	,OpcNor,		60,-1},
	{" Nor_Not "	,OpcNorNot,		60,-1},
	{" && "		,OpcAnd,		52,-1},
	{" And_Not "	,OpcAndNot,		52,-1},
	{" Nand "	,OpcNand,		52,-1},
	{" Nand_Not "	,OpcNandNot,	52,-1},
	{" == "		,OpcEq,		48,-1},
	{" <> "		,OpcNe,		48,-1},
	{" >= "		,OpcGe,		44,-1},
	{" > "		,OpcGt,		44,-1},
	{" <= "		,OpcLe,		44,-1},
	{" < "		,OpcLt,		44,-1},
	{" Is_In "	,OpcIsIn,		40,-1},
	{" // "		,OpcConcat,		32,-1},
	{" << "		,OpcShiftLeft,	28,-1},
	{" >> "		,OpcShiftRight,	28,-1},
	{" + "		,OpcAdd,		24,-1},
	{" - "		,OpcSubtract,	24,-1},
	{" * "		,OpcMultiply,	20,-1},
	{" / "		,OpcDivide,		20,-1},
	{" ^ "		,OpcPower,		16,1},
	{0,0,0,0}
}, unary[] = {
	{"~"		,OpcInot,		8,1},
	{"!"		,OpcNot,		8,1},
	{"--"		,OpcPreDec,		8,1},
	{"++"		,OpcPreInc,		8,1},
	{"-"		,OpcUnaryMinus,	8,1},
	{"+"		,OpcUnaryPlus,	8,1},
	{"--"		,OpcPostDec,	4,-1},
	{"++"		,OpcPostInc,	4,-1},
	{0,0,0,0}
};
STATIC_CONSTANT DESCRIPTOR(ARROW,		"->");
STATIC_CONSTANT char *bname[] = {
	"Param",	/*194*/
	"Signal",	/*195*/
	"Dim",		/*196 dtype_dimension*/
	"Window",	/*197*/
	"Slope",	/*198*/
	"Function",	/*199*/
	"Conglom",	/*200*/
	"Range",	/*201*/
	"Action",	/*202*/
	"Dispatch",	/*203*/
	"Program",	/*204*/
	"Routine",	/*205*/
	"Procedure",	/*206*/
	"Method",	/*207*/
	"Dependency",	/*208*/
	"Condition",	/*209*/
	"Event",	/*210*/
	"With_Units",	/*211*/
	"Call",		/*212*/
	"With_Error",	/*213*/
};
STATIC_ROUTINE int		Append(
char			*pstr,
struct descriptor_d	*pd)
{
struct descriptor	dstr = {0,DTYPE_T,CLASS_S,0};
dstr.length = (unsigned short)strlen(pstr);
dstr.pointer = pstr;
	return StrAppend(pd, &dstr);
}

STATIC_CONSTANT DESCRIPTOR(newline,	"\r\n\t\t\t\t\t\t\t");

void			TdiDecompileDeindent(
struct descriptor_d	*pout)
{
char	*start = pout->pointer, *fin = start + pout->length - 1;

	if (!start) return;
	while (fin >= start && (*fin == '\t' || *fin == '\n' || *fin == '\r')) --fin;
	pout->length = (unsigned short)(fin - start + 1);
	return;
}

STATIC_ROUTINE int		Indent(
int			step,
struct descriptor_d	*pout)
{
	newline.length = (unsigned short)(((TdiIndent += step) < 8 ? TdiIndent : 8) + 1);
	return StrAppend(pout, &newline);
}

STATIC_ROUTINE int		OneStatement(
struct descriptor_r	*pin,
struct descriptor_d	*pout)
{
int	status = 1;

	if (pin) status = Tdi0Decompile(pin, P_STMT, pout);
	if (status&1)	TdiDecompileDeindent(pout);
	if (status&1)	switch (pout->pointer[pout->length-1]) {
	default :
		status = Append(";", pout);
		break;
	case ';' :
	case '}' :
		break;
	}
	if (status&1)	status = Indent(0, pout);
	return status;
}

STATIC_ROUTINE int		MultiStatement(
int			nstmt,
struct descriptor_r	*pin[],
struct descriptor_d	*pout)
{
int	status = 1, j;

	if (nstmt == 0) {
				status = Append(";", pout);
		if (status&1)	status = Indent(0, pout);
	}
	else	for (j = 0; j < nstmt && status&1; j++) status = OneStatement(pin[j], pout);
	return status;
}

STATIC_ROUTINE int		CompoundStatement(
int			nstmt,
struct descriptor_r	*pin[],
struct descriptor_d	*pout)
{
int	status;

	status = Append("{", pout);
	if (nstmt > 0) {
		if (status&1)	status = Indent(1, pout);
		if (status&1)	status = MultiStatement(nstmt, pin, pout);
		if (status&1)	TdiDecompileDeindent(pout);
		if (status&1)	status = Indent(-1, pout);
	}
	if (status&1)	status = Append("}", pout);
	return status;
}

STATIC_ROUTINE int		Arguments(
int			first,
char			*left,
char			*right,
struct descriptor_r	*pin,
struct descriptor_d	*pout)
{
int	j, status, last = pin->ndesc - 1;

	if (left) status = Append(left, pout);
	else status = 1;
	for (j = first; status&1 && j <= last; j++) {
		status = Tdi0Decompile(pin->dscptrs[j], P_ARG, pout);
		if (status&1 && j < last) status = Append(", ", pout);
	}
	if (status&1)	status = Append(right, pout);
	return status;
}
int			Tdi0Decompile_R(
struct descriptor_r	*pin,
int			prec,
struct descriptor_d	*pout)
{
struct descriptor_r	*r_ptr;
char		 	*ptext;
struct TdiFunctionStruct *fun_ptr;
int			narg = pin->ndesc;
int			m, lorr, newone, status = 1;
struct op_rec		*pop;
unsigned int		dtype;
enum OpcOpcodes        opcode;

	switch (pin->dtype) {
	default :
		status = Append("%Unknown%", pout);
		break;

	case DTYPE_PARAM :
	case DTYPE_SIGNAL :
	case DTYPE_DIMENSION :
	case DTYPE_WINDOW :
	case DTYPE_SLOPE :
	case DTYPE_CONGLOM :
	case DTYPE_ACTION :
	case DTYPE_DISPATCH :
	case DTYPE_PROGRAM :
	case DTYPE_ROUTINE :
	case DTYPE_PROCEDURE :
	case DTYPE_METHOD :
	case DTYPE_DEPENDENCY :
	case DTYPE_CONDITION :
	case DTYPE_WITH_UNITS :
	case DTYPE_WITH_ERROR :
build:		status = Append("Build_", pout);
		if (status&1)	status = Append(bname[pin->dtype - DTYPE_PARAM], pout);
		if (status&1)	status = Append("(", pout);
		if (pin->length) {
			if (pin->length == 1) opcode = (enum OpcOpcodes)(*(unsigned char *)pin->pointer);
			else if (pin->length == 2) opcode = (enum OpcOpcodes)(*(unsigned short *)pin->pointer);
			else if (pin->length == 4) opcode = (enum OpcOpcodes)(*(unsigned int *)pin->pointer);
			else opcode = (enum OpcOpcodes)-1;
			if (status&1)	status = TdiSingle(opcode, pout);
			if (status&1)	status = Append(", ", pout);
		}
		if (status&1)	status = Arguments(0, 0, ")", pin, pout);
		break;

	case DTYPE_RANGE :
		opcode = OpcDtypeRange;
		goto range;

	case DTYPE_FUNCTION :
		opcode = (enum OpcOpcodes)(*(unsigned short *)pin->pointer);
		if ((int)opcode >= TdiFUNCTION_MAX) return (int)(opcode = (enum OpcOpcodes)0);

range:		fun_ptr = (struct TdiFunctionStruct *)&TdiRefFunction[opcode];
		if (narg < fun_ptr->m1 || narg > fun_ptr->m2) goto cannot;
		else if (fun_ptr->token == LEX_CONST) status = Append(fun_ptr->name, pout);

		/*****************************************
		Functions, subscripts, and constructors.
		*****************************************/
		else switch (opcode) {
		default :		/*intrinsic(arg, ...)*/
cannot:			status = Append(fun_ptr->name, pout);
			if (status&1)	status = Arguments(0, "(", ")", pin, pout);
			break;
		case OpcFun :	/*fun ident(arg, ...) stmt*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("Fun ", pout);
			if (!(status&1)) break;
			r_ptr = (struct descriptor_r *)pin->dscptrs[0];
			if (r_ptr->dtype == DTYPE_T) status = StrAppend(pout, r_ptr);
			else status = Tdi0Decompile(r_ptr, P_SUBS, pout);
			if (status&1)	status = Arguments(2, " (", ") ", pin, pout);
			if (status&1)	status = CompoundStatement(1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (status&1 && prec < P_STMT)	status = Append(")", pout);
			break;
		case OpcIn :		/*input argument*/
		case OpcInOut :	/*input and output argument*/
		case OpcOptional :	/*optional argument*/
		case OpcOut :	/*output argument*/
		case OpcPrivate :	/*private ident*/
		case OpcPublic :	/*public ident*/
			status = Append(fun_ptr->name, pout);
			if (status & 1) status = Append(" ", pout);
			r_ptr = (struct descriptor_r *)pin->dscptrs[0];
			if (status & 1)
			if (r_ptr->dtype == DTYPE_T) status = StrAppend(pout, r_ptr);
			else	status = Tdi0Decompile(r_ptr, P_SUBS, pout);
			break;
		case OpcExtFunction : /*_label(arg, ...)*/
			if (pin->dscptrs[0] != 0
			|| pin->dscptrs[1] == 0
			|| pin->dscptrs[1]->dtype != DTYPE_T) goto cannot;
			status = StrAppend(pout, pin->dscptrs[1]);
			if (status&1)	status = Arguments(2, "(", ")", pin, pout);
			break;
		case OpcSubscript :	/*postfix[subscript, ...]*/
					status = Tdi0Decompile(pin->dscptrs[0], P_SUBS, pout);
			if (status&1)	status = Arguments(1, "[", "]", pin, pout);
			break;
		case OpcVector :	/*[elem, ...]*/
			status = Arguments(0, "[", "]", pin, pout);
			break;
		/****************
		Unary operations.
		****************/
		case OpcInot :	case OpcNot :
		case OpcPreDec :	case OpcPreInc :
		case OpcUnaryMinus :case OpcUnaryPlus :
		case OpcPostDec :	case OpcPostInc :
			for (pop = unary; pop->opcode; pop++) if ((enum OpcOpcodes)pop->opcode == opcode) break;
			newone = pop->prec;
			lorr = pop->lorr;
			if (		lorr > 0)	status = Append(pop->symbol, pout);
			if (status&1 && prec <= newone)	status = Append("(", pout);
			if (status&1)			status = Tdi0Decompile(pin->dscptrs[0], newone + lorr, pout);
			if (status&1 && prec <= newone)	status = Append(")", pout);
			if (status&1 && lorr < 0)	status = Append(pop->symbol, pout);
			break;

		/***********************
		Binary/n-ary operations.
		***********************/
		case OpcEqualsFirst :
			r_ptr = (struct descriptor_r *)pin->dscptrs[0];
			newone = *(unsigned short *)r_ptr->pointer;
			narg = r_ptr->ndesc;
			goto first;
		case OpcPower :
		case OpcDivide :	case OpcMultiply :
		case OpcAdd :	case OpcSubtract :
		case OpcShiftLeft :	case OpcShiftRight :
		case OpcConcat :
		case OpcIsIn :
		case OpcGe :		case OpcGt :		case OpcLe :		case OpcLt :
		case OpcEq :		case OpcNe :
		case OpcAnd :	case OpcNand :
		case OpcOr :		case OpcNor :
		case OpcEqv :	case OpcNeqv :
		case OpcPromote :
		case OpcEquals :
		case OpcDtypeRange :
		case OpcComma :
		case OpcConditional :
			r_ptr = (struct descriptor_r *)pin;
			newone = (int)opcode;
first:			for (pop = binary; pop->opcode; pop++) if (pop->opcode == newone) break;
			newone = pop->prec;
			lorr = pop->lorr;
			if (opcode == OpcEqualsFirst) {
				newone = binary[2].prec;
				lorr = binary[2].lorr;
			}
			if (prec <= newone) status = Append("(", pout);
			if (opcode == OpcConditional) {
				if (status&1)	status = Tdi0Decompile(r_ptr->dscptrs[2], newone - lorr, pout);
				if (status&1)	status = Append(pop->symbol, pout);
				if (status&1)	status = Tdi0Decompile(r_ptr->dscptrs[0], newone, pout);
				if (status&1)	status = Append(" : ", pout);
				if (status&1)	status = Tdi0Decompile(r_ptr->dscptrs[1], newone + lorr, pout);
			}
			else {
				if (status&1)	status = Tdi0Decompile(r_ptr->dscptrs[0], newone - lorr, pout);
				for (m = 1; m < narg; m++) {
					if (status&1)	status = Append(pop->symbol, pout);
					if (status&1 && pin != r_ptr) status = Append("= ", pout);
					if (status&1)	status = Tdi0Decompile(r_ptr->dscptrs[m], newone + lorr, pout);
				}
			}
			if (status&1 && prec <= newone) status = Append(")", pout);
			break;

		/*****************************************
		C-language constructs followed by newline.
		Plus WHERE.
		*****************************************/
		case OpcBreak :	/*break;*/
		case OpcContinue :	/*continue;*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append(fun_ptr->name, pout);
			if (status&1)	status = OneStatement(0, pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcCase :	/*case (xxx) stmt ...*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("Case (", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[0], P_STMT, pout);
			if (status&1)	status = Append(") ", pout);
			if (status&1)	status = MultiStatement(narg-1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcDefault :	/*case default stmt ...*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("Case Default ", pout);
			if (status&1)	status = MultiStatement(narg, (struct descriptor_r **)&pin->dscptrs[0], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcDo :		/*do {stmt} while (exp); Note argument order is (exp,stmt,...)*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("DO {", pout);
			if (status&1)	status = MultiStatement(narg-1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (status&1)	status = Append("} While ", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[0], P_STMT, pout);
			if (status&1)	status = MultiStatement(0, (struct descriptor_r **)0, pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcFor :	/*for (init;test;step) stmt*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("For (", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[0], P_STMT, pout);
			if (status&1)	status = Append("; ", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[1], P_STMT, pout);
			if (status&1)	status = Append("; ", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[2], P_STMT, pout);
			if (status&1)	status = Append(") ", pout);
			if (status&1)	
                           status = CompoundStatement(narg-3, (struct descriptor_r **)&pin->dscptrs[3], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcGoto :	/*goto xxx;*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("GoTo ", pout);
			if (status&1)	status = StrAppend(pout, pin->dscptrs[0]);
			if (status&1)	status = OneStatement(0, pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcIf :		/*if (exp) stmt else stmt*/
		case OpcWhere :	/*where (exp) stmt elsewhere stmt*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append((opcode == OpcIf) ? "If (" : "Where (", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[0], P_STMT, pout);
			if (status&1)	status = Append(") ", pout);
			if (status&1)	status = CompoundStatement(1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (status&1 && narg >= 3) {
						status = Append((opcode == OpcIf) ? " Else " : " ElseWhere ", pout);
				if (status&1)	
                                   status = CompoundStatement(1, (struct descriptor_r **)&pin->dscptrs[2], pout);
			}
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcLabel :	/*xxx : stmt ...*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("Label ", pout);
			if (status&1)	status = StrAppend(pout, pin->dscptrs[0]);
			if (status&1)	status = Append(" : ", pout);
			if (status&1)	status = MultiStatement(narg-1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcReturn :	/*return (optional-exp);*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append("Return (", pout);
			if (status&1)	status = Tdi0Decompile(pin->ndesc ? pin->dscptrs[0] : 0, P_STMT, pout);
			if (status&1)	status = Append(")", pout);
			if (status&1)	status = OneStatement(0, pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcStatement :	/*{stmt ...}*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = MultiStatement(narg, (struct descriptor_r **)&pin->dscptrs[0], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		case OpcSwitch :	/*switch (exp) stmt*/
		case OpcWhile :	/*while (exp) stmt*/
			if (prec < P_STMT) status = Append("(", pout);
			if (status&1)	status = Append((opcode == OpcSwitch) ? "Switch (" : "While (", pout);
			if (status&1)	status = Tdi0Decompile(pin->dscptrs[0], P_STMT, pout);
			if (status&1)	status = Append(") ", pout);
			if (status&1)	
                           status = CompoundStatement(narg-1, (struct descriptor_r **)&pin->dscptrs[1], pout);
			if (prec < P_STMT && status&1) status = Append(")", pout);
			break;
		}
		/********************
		End of opcode switch.
		********************/
		break;
	case DTYPE_CALL : /*label->label(arg, ...) or label->label:type(arg, ...)*/
		if (pin->dscptrs[0] == 0 || pin->dscptrs[0]->dtype != DTYPE_T) goto build;
		if (pin->dscptrs[1] == 0 || pin->dscptrs[1]->dtype != DTYPE_T) goto build;
		status = StrConcat(pout, pout, pin->dscptrs[0], &ARROW, pin->dscptrs[1] MDS_END_ARG);
		if (status&1 && pin->length && pin->pointer) {
			dtype = *(char *)pin->pointer;
			if (dtype < TdiCAT_MAX)  ptext = TdiREF_CAT[dtype].name;
			else if (dtype == DTYPE_NID) ptext = "Nid";
			else if (dtype == DTYPE_PATH) ptext = "Path";
			else ptext = "%Unknown%";
					status = Append(":", pout);
			if (status&1)	status = Append(ptext , pout);
		}
		if (status&1)	status = Arguments(2, "(", ")", pin, pout);
		break;
	}
	/*******************
	End of dtype switch.
	*******************/
	return status;
}
