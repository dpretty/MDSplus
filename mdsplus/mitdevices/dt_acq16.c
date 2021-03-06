#ifndef _WIN32
#ifdef __VMS
#include <descrip.h>
#else
#include <mdsdescrip.h>
#endif
#include <treeshr.h>
#include <ncidef.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <xmdsshr.h>

static void ManageChild(Widget w, char *name);
static void UnManageChild(Widget w, char *name);

#ifdef __VMS
int DT_ACQ16$DW_SETUP( struct dsc$descriptor *niddsc, struct dsc$descriptor *methoddsc, Widget parent)
{ 
  static String uids[] = {{"DECW$SYSTEM_DEFAULTS:DT_ACQ16.UID"}};
  static int nid;
  static MrmRegisterArg uilnames[] = {
	{"nid",(XtPointer)0},
	{"ManageChild",(XtPointer)ManageChild},
	{"UnManageChild",(XtPointer)UnManageChild},
	};
  static NCI_ITM   nci[] = {{4, NCI$_CONGLOMERATE_NIDS, (unsigned char *)&nid, 0}, {0, NCI$_END_OF_LIST, 0, 0}};
  TREE$GETNCI((int *)niddsc->dsc$a_pointer, nci);
  uilnames[0].value = (XtPointer)nid;
  return XmdsDeviceSetup(parent, (int *)niddsc->dsc$a_pointer, uids, XtNumber(uids), "DT_ACQ16", uilnames, XtNumber(uilnames), 0);
}
#else
int dt_acq16__dw_setup( struct descriptor *niddsc, struct descriptor *methoddsc, Widget parent)
{ 
  static String uids[] = {"DT_ACQ16.uid"};
  static int nid;
  static MrmRegisterArg uilnames[] = {
	{"nid",(XtPointer)0},
	{"ManageChild",(XtPointer)ManageChild},
	{"UnManageChild",(XtPointer)UnManageChild},};
  static NCI_ITM   nci[] = {{4, NciCONGLOMERATE_NIDS, (unsigned char *)&nid, 0}, {0, NciEND_OF_LIST, 0, 0}};
  TreeGetNci(nid, nci);
  uilnames[0].value = (XtPointer)(nid + (char *)0);
  return XmdsDeviceSetup(parent, (int *)niddsc->pointer, uids, XtNumber(uids), "DT_ACQ16", uilnames, XtNumber(uilnames), 0);
}
#endif

static Widget FindTop(Widget w)
{
  for (;w &&  ((w->core.name == 0) || strcmp(w->core.name,"DT_ACQ16")); w=XtParent(w));
  return w;
}

static void ManageChild(Widget w, char *name)
{
  Widget par = FindTop(w);
  if (par) {
    XtManageChild(XtNameToWidget(par, name));
  }
}

static void UnManageChild(Widget w, char *name)
{
  Widget par = FindTop(w);
  if (par) {
    XtUnmanageChild(XtNameToWidget(par, name));
  }
}
#endif
