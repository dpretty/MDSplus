/*
 * README: Portions of this file are merged at file generation
 * time. Edits can be made *only* in between specified code blocks, look
 * for keywords <Begin user code> and <End user code>.
 */
/*
 *
 * This driver program generated by Builder Xcessory 3.5.1 VMS.
 *
 */

/*
 * Standard ANSI Header File
 */
#include <stdlib.h>

/*
 * Motif and X required Header Files
 */
#include <X11/Xlib.h>
#include <Mrm/MrmAppl.h>
/*
 * Digital widgets
 #include <DXm/DecSpecific.h>
 */


#if (XmVersion >= 1002)
#include <Xm/RepType.h>
#endif
#include <Xm/MwmUtil.h>

#define DECLARE_BX_GLOBALS

/*
 * Global declarations are now stored in "MAIN-UIL.H"
 *
 * If DECLARE_BX_GLOBALS is defined then this header file
 * declares the globals, otherwise it just externs them.
 */

#include "MAIN-UIL.H"


/*
 * Application name and class definition
 */
#define BX_APP_NAME "builderProduct"
#define BX_APP_CLASS "BuilderProduct"
#define BX_UID_FILE_NAME "traverser.uid"

/*
 * External procedure declarations 
 */
extern void CloseTree(Widget, XtPointer, XtPointer);
extern void MDisplayData(Widget, XtPointer, XtPointer);
extern void MDoAction(Widget, XtPointer, XtPointer);
extern void ModifyData(Widget, XtPointer, XtPointer);
extern void MenuUnmap(Widget, XtPointer, XtPointer);
extern void BxManageCB(Widget, XtPointer, XtPointer);
extern void MSetupDevice(Widget, XtPointer, XtPointer);
extern void SetDefault(Widget, XtPointer, XtPointer);
extern void MModifyData(Widget, XtPointer, XtPointer);
extern void DisplayNci(Widget, XtPointer, XtPointer);
extern void BxExitCB(Widget, XtPointer, XtPointer);
extern void MTurnOnOff(Widget, XtPointer, XtPointer);
extern void DisplayData(Widget, XtPointer, XtPointer);
extern void MSetDefault(Widget, XtPointer, XtPointer);
extern void MDisplayNci(Widget, XtPointer, XtPointer);
extern void TurnOnOff(Widget, XtPointer, XtPointer);
extern void DoAction(Widget, XtPointer, XtPointer);
extern void OpenTree(Widget, XtPointer, XtPointer);
extern void AddListTree(Widget, XtPointer, XtPointer);
extern void SetupDevice(Widget, XtPointer, XtPointer);
extern void AddNode(Widget, XtPointer, XtPointer);
extern void AddNodeApply(Widget, XtPointer, XtPointer);
extern void AddNodeDismiss(Widget, XtPointer, XtPointer);
extern void SetUsage(Widget, XtPointer, XtPointer);

/*
 * Function prototypes for routines located in utilities file
 */
extern XtPointer CONVERT(Widget, char*, char*, int, Boolean*);
extern void RegisterBxConverters(XtAppContext);
extern void BxStoreWidgetId(Widget, XtPointer, XtPointer);

/* Begin user code block <globals> */

#include <xmdsshr.h>

static XrmOptionDescRec options[] = {
{"-background",	"*background",		XrmoptionSepArg,	NULL},
{"-bd",		"*borderColor",		XrmoptionSepArg,	NULL},
{"-bg",		"*background",		XrmoptionSepArg,	NULL},
{"-borderwidth","*borderWidth",		XrmoptionSepArg,	NULL},
{"-bordercolor","*borderColor",		XrmoptionSepArg,	NULL},
{"-bw",		"*borderWidth",		XrmoptionSepArg,	NULL},
{"-display",	".display",		XrmoptionSepArg,	NULL},
{"-fg",		"*foreground",		XrmoptionSepArg,	NULL},
{"-fn",		"*font",		XrmoptionSepArg,	NULL},
{"-font",	"*font",		XrmoptionSepArg,	NULL},
{"-foreground",	"*foreground",		XrmoptionSepArg,	NULL},
{"-geometry",	"*geometry",		XrmoptionSepArg,	"485x405+250+300"},
{"-iconic",	"*iconic",		XrmoptionNoArg,		"on"},
{"-name",	".name",		XrmoptionSepArg,	NULL},
{"-reverse",	"*reverseVideo",	XrmoptionNoArg,		"on"},
{"-rv",		"*reverseVideo",	XrmoptionNoArg,		"on"},
{"-synchronous",".synchronous",		XrmoptionNoArg,		"on"},
{"-title",	".title",		XrmoptionSepArg,	NULL},
{"-x",		".x",			XrmoptionSepArg,	NULL},
{"-y",		".y",			XrmoptionSepArg,	NULL},
{"-xrm",	NULL,			XrmoptionResArg,	NULL},
{"-tree",	".tree",		XrmoptionSepArg,	NULL},
{"-shot",	".shot",		XrmoptionSepArg,	NULL},
{"-command",	".showCommand",		XrmoptionNoArg,		"True"},
{"-nocommand",	".showCommand",		XrmoptionNoArg,		"False"},
{"-read_only",	".read_only",		XrmoptionNoArg,		"True"},
{"-edit",	".edit",		XrmoptionNoArg,		"True"}};

/* End user code block <globals> */

/*
 * Widget variable declarations
 */
Widget AppShell; /* The Main Application Shell */
Widget TopLevelShell;
Widget MainWindow;
Widget WriteDialog;
Widget AddDialog;
Widget OpenDialog;

/*
 * Main Program
 */
int main(int argc, char **argv)
{
    Display *display;
    Arg args[256];
    Cardinal ac;
    /*SUPPRESS 591*/
    Boolean argok;
    XtAppContext context;
    Widget w;
    Widget parent;
    
    /* Begin user code block <declarations> */
    /* End user code block <declarations> */
    
    /*
     * Hierarchy variable declarations
     * Set "*newMrmStyle:False" in ~/.bxrc for bx25 style declarations
     */
    
    MrmHierarchy HierarchyID;
    MrmType MrmWClass;
    static char *UidFile [] = { BX_UID_FILE_NAME };
    MRMRegisterArg NamesList[24];
    Cardinal Nlac;
    Nlac = 0;
    NamesList[Nlac].name = "CloseTree";
    NamesList[Nlac++].value = (XtPointer) CloseTree;
    NamesList[Nlac].name = "MDisplayData";
    NamesList[Nlac++].value = (XtPointer) MDisplayData;
    NamesList[Nlac].name = "MDoAction";
    NamesList[Nlac++].value = (XtPointer) MDoAction;
    NamesList[Nlac].name = "ModifyData";
    NamesList[Nlac++].value = (XtPointer) ModifyData;
    NamesList[Nlac].name = "MenuUnmap";
    NamesList[Nlac++].value = (XtPointer) MenuUnmap;
    NamesList[Nlac].name = "BxManageCB";
    NamesList[Nlac++].value = (XtPointer) BxManageCB;
    NamesList[Nlac].name = "MSetupDevice";
    NamesList[Nlac++].value = (XtPointer) MSetupDevice;
    NamesList[Nlac].name = "SetDefault";
    NamesList[Nlac++].value = (XtPointer) SetDefault;
    NamesList[Nlac].name = "MModifyData";
    NamesList[Nlac++].value = (XtPointer) MModifyData;
    NamesList[Nlac].name = "DisplayNci";
    NamesList[Nlac++].value = (XtPointer) DisplayNci;
    NamesList[Nlac].name = "BxExitCB";
    NamesList[Nlac++].value = (XtPointer) BxExitCB;
    NamesList[Nlac].name = "MTurnOnOff";
    NamesList[Nlac++].value = (XtPointer) MTurnOnOff;
    NamesList[Nlac].name = "DisplayData";
    NamesList[Nlac++].value = (XtPointer) DisplayData;
    NamesList[Nlac].name = "MSetDefault";
    NamesList[Nlac++].value = (XtPointer) MSetDefault;
    NamesList[Nlac].name = "MDisplayNci";
    NamesList[Nlac++].value = (XtPointer) MDisplayNci;
    NamesList[Nlac].name = "TurnOnOff";
    NamesList[Nlac++].value = (XtPointer) TurnOnOff;
    NamesList[Nlac].name = "DoAction";
    NamesList[Nlac++].value = (XtPointer) DoAction;
    NamesList[Nlac].name = "OpenTree";
    NamesList[Nlac++].value = (XtPointer) OpenTree;
    NamesList[Nlac].name = "AddListTree";
    NamesList[Nlac++].value = (XtPointer) AddListTree;
    NamesList[Nlac].name = "SetupDevice";
    NamesList[Nlac++].value = (XtPointer) SetupDevice;
    NamesList[Nlac].name = "AddNode";
    NamesList[Nlac++].value = (XtPointer) AddNode;
    NamesList[Nlac].name = "AddNodeApply";
    NamesList[Nlac++].value = (XtPointer) AddNodeApply;
    NamesList[Nlac].name = "AddNodeDismiss";
    NamesList[Nlac++].value = (XtPointer) AddNodeDismiss;
    NamesList[Nlac].name = "SetUsage";
    NamesList[Nlac++].value = (XtPointer) SetUsage;
    NamesList[Nlac].name = NULL;
    NamesList[Nlac].value = NULL;
    
    
    MrmInitialize();
    XtToolkitInitialize();
    context = XtCreateApplicationContext();
    display = XtOpenDisplay(context, 0, BX_APP_NAME, BX_APP_CLASS,
#ifndef XtSpecificationRelease
        options, XtNumber(options), (Cardinal*)&argc, argv);
#else
#if XtSpecificationRelease == 4
        options, XtNumber(options), (Cardinal*)&argc, argv);
#else
        options, XtNumber(options), &argc, argv);
#endif
#endif
    if(display == NULL)
    {
        XtWarning("cannot open display");
        exit(1);
    }
    /*
     * So converters work.
     */

    XtInitializeWidgetClass(applicationShellWidgetClass);

    /*
     * Install the tearOffModel resource converter
     */
#if (XmVersion >= 1002)
    XmRepTypeInstallTearOffModelConverter();
#endif
    
    /*
     * Register special BuilderXcessory converters
     */
    RegisterBxConverters(context);
    
    
    /* Begin user code block <create_shells> */
    XmdsInitialize();
    /* End user code block <create_shells> */
    /*
     * The applicationShell is created as an unrealized
     * parent for multiple topLevelShells.  The topLevelShells
     * are created as popup children of the applicationShell.
     * This is a recommendation of Paul Asente & Ralph Swick in
     * _X_Window_System_Toolkit_ p. 677.
     */
    ac = 0;
    AppShell = parent = XtAppCreateShell( BX_APP_NAME, BX_APP_CLASS,
                  applicationShellWidgetClass, display, args, ac);
    
    MrmRegisterNames(NamesList, Nlac);
    
    HierarchyID = 0;
    MrmOpenHierarchy((MrmCount)(1), UidFile, 0, &HierarchyID);
    XtInitializeWidgetClass(topLevelShellWidgetClass);
    ac = 0;
    XtSetArg(args[ac], XmNtitle, "Traverser"); ac++;
    XtSetArg(args[ac], XmNiconName, "Traverser"); ac++;
    XtSetArg(args[ac], XmNx, 947); ac++;
    XtSetArg(args[ac], XmNy, 349); ac++;
    XtSetArg(args[ac], XmNwidth, 509); ac++;
    XtSetArg(args[ac], XmNheight, 672); ac++;
    TopLevelShell = XtCreatePopupShell( "topLevelShell",
                        topLevelShellWidgetClass, AppShell, args, ac);
    MainWindow = NULL;
    MrmFetchWidget(HierarchyID, "mainWindow", TopLevelShell,
            &MainWindow, &MrmWClass);
    
    WriteDialog = NULL;
    MrmFetchWidget(HierarchyID, "writeDialog", MainWindow,
            &WriteDialog, &MrmWClass);
    
    AddDialog = NULL;
    MrmFetchWidget(HierarchyID, "addDialog", MainWindow,
            &AddDialog, &MrmWClass);
    
    OpenDialog = NULL;
    MrmFetchWidget(HierarchyID, "openDialog", MainWindow,
            &OpenDialog, &MrmWClass);
    
    MrmCloseHierarchy(HierarchyID);
    
    XtManageChild(MainWindow);
    XtPopup(XtParent(MainWindow), XtGrabNone);
    
    /* Begin user code block <app_procedures> */
    /* End user code block <app_procedures> */
    
    /* Begin user code block <main_loop> */
    /* End user code block <main_loop> */
    
    XtAppMainLoop(context);
    
    /*
     * A return value even though XtAppMainLoop never ends. 
     */
     return(0); 
}
