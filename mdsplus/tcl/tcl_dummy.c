#include        <stdio.h>

#define dummy(a) \
    int a(){printf("Routine %s not implemented yet\n",#a);return 0;}

dummy(TclDispatch_abort_server)
dummy(TclCleanDatafile)
dummy(TclCompressDatafile)
dummy(TclDecompile)
dummy(TclDeleteNode)
dummy(TclDeletePulse)
dummy(TclDispatch)
dummy(TclDispatch_build)
dummy(TclDispatch_check)
dummy(TclDispatch_phase)
dummy(TclDispatch_close)
dummy(TclDispatch_command)
dummy(TclDoNode)
dummy(TclEdit)
dummy(TclLink)
dummy(TreeMarkIncludes)
dummy(TclPutExpression)
dummy(TclCommand)
dummy(TclRemoveTag)
dummy(TclRename)
dummy(TclRundownTree)
dummy(TclDispatch_set_server)
dummy(TclSetEvent)
dummy(TclShowCurrent)
dummy(TclShowDB)
dummy(TclDispatch_show_server)
dummy(TclDispatch_start_server)
dummy(TclDispatch_stop_server)
dummy(TclWfevent)
dummy(TclWrite)


	/* Some MDS and TREESHR routines not implemented on unix ...	*/
#ifndef __vms
dummy(ServerCreatePulse)
dummy(TreeSetNoSubtree)
dummy(TreeVerifyTree)
#endif
