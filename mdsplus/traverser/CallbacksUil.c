/*
 * README: This file is appended to at file generation time.
 * Edits can be made throughout the file
 */
/*
 * Generated by the ICS Builder Xcessory (BX).
 *
 *
 * Builder Xcessory 3.5.1 VMS.
 *
 */
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <X11/Intrinsic.h>
#include <xmdsshr.h>
#include <Xmds/ListTree.h>
#include <Xmds/XmdsXdBox.h>
#include <Xmds/XmdsXdBoxDialog.h>
#include <Xmds/XmdsCallbacks.h>
/*
 * Standard includes for builtins.
 */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <libroutines.h>
#include <strroutines.h>
#include <usagedef.h>
/*
 * mds includes
 */
#include <treeshr.h>
/*
 * support routines
 */
#include "support.h"
/*
 *  MDS routines not in any includes
 */
  extern int TdiModelOf();
/*
 * Macros to make code look nicer between ANSI and K&R.
 */
#ifndef ARGLIST
#if (NeedFunctionPrototypes == 0)
#define PROTOTYPE(p)	()
#define ARGLIST(p)	p
#define ARG(a, b)	a b;
#define GRA(a, b)	a b;
#define UARG(a, b)      a b;
#define GRAU(a, b)      a b;
#else
#define PROTOTYPE(p)	p
#define ARGLIST(p)	(
#define ARG(a, b)	a b,
#define GRA(a, b)	a b)
#ifdef __cplusplus
#define UARG(a, b)      a,
#define GRAU(a, b)      a)
#else
#define UARG(a, b)      a b,
#define GRAU(a, b)      a b)
#endif
#endif
#endif

Widget		BxFindTopShell PROTOTYPE((Widget));
WidgetList	BxWidgetIdsFromNames PROTOTYPE((Widget, char*, char*));


#include <stdlib.h>
#include <mdsdescrip.h>
#include <mds_stdarg.h>
static Pixmap *icons;
static Pixmap *off_icons;
#define MAX_SELECTIONS 10
static ListTreeItem *menu_item = 0;
static ListTreeItem *default_item = 0;
static ListTreeItem *selections[MAX_SELECTIONS];
static int num_selected;
static Atom XA_TARGETS = 0;

struct node {
  int nid;
  int usage;
  char *name;
  int populated;
};

typedef enum {on_off, rename_node, delete, new, tree, set_def} NodeTouchType; 

static char *get_node_name(int nid)
{
    char *name;
    char *ans;
    int def_nid;
    int status;
    static int c_nid;
    static DESCRIPTOR_NID(nid_dsc, &c_nid);
    static char *getnci = "TRIM((GETNCI($1, 'IS_CHILD') ? '.' : '')//GETNCI($1, 'NODE_NAME'))//(((GETNCI($1, 'NUMBER_OF_CHILDREN')+GETNCI($1, 'NUMBER_OF_MEMBERS')) > 0) ? '...' : '')";
    c_nid = nid;
    name=ReadString(getnci, &nid_dsc MDS_END_ARG);
    return name;
}

static void set_name(ListTreeItem *item, char *name)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    nd->name = name;
  }

static void set_nid(ListTreeItem *item, int nid)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    nd->nid = nid;
  }

static void set_usage(ListTreeItem *item, int usage)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    nd->usage = usage;
  }

static int get_nid(ListTreeItem *item)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    return nd->nid;
  }

static int get_usage(ListTreeItem *item)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    return nd->usage;
  }

static void set_populated(ListTreeItem *item, int populated)
  {
    struct node *nd;
    nd = (struct node *)item->user_data;
    nd->populated = populated;
  }

static int is_populated(ListTreeItem *item) 
{
    struct node *nd;
    nd = (struct node *)item->user_data;
    return nd->populated;
}

static int parent_nid(int nid)
{
  static int c_nid;
  static DESCRIPTOR_NID(nid_dsc,&c_nid);
  static char *getparent = "GETNCI($, 'PARENT')";
  c_nid = nid;
  return ReadInt(getparent, &nid_dsc MDS_END_ARG);
}

static Boolean convert_proc(Widget w, Atom *selection, Atom *target, Atom *type_return,
                            XtPointer *value_return, unsigned long *length_return, int *format_return)
{
  int status = 1;
  if (!XA_TARGETS)
    XA_TARGETS = XInternAtom(XtDisplay(w), "TARGETS", 0);
  if (*target == XA_TARGETS) {
    status = 1;
    *type_return = *target;
    *value_return = XtMalloc(sizeof(Atom));
    ((Atom *) * value_return)[0] = XA_STRING;
    *length_return = 1;
    *format_return = 32;
  }
  else if (*target == XA_STRING) {
    if (num_selected > 0) {
      int i;
      char *ans=0;
      status = 1;
      for (i=0; i<num_selected; i++) {
        static char *getnci = "GETNCI($, 'FULLPATH')";
        static int c_nid;
        static DESCRIPTOR_NID(nid_dsc, &c_nid);
        char *fullpath;
        c_nid = get_nid(selections[i]);
        fullpath = ReadString(getnci, &nid_dsc MDS_END_ARG);
        if (i == 0) {
          ans = malloc(strlen(fullpath)+1);
          strcpy(ans, fullpath); 
        }
        else {
          static char *cr = "\r";
          char *tmp = malloc(strlen(ans)+strlen(fullpath)+1+1); /* room for the previous stuff + new + \r + \0 */
          strcpy(tmp, ans);
          strcat(tmp, cr);
          strcat(tmp, fullpath);
          free(ans);
          ans = tmp;
        }
        free(fullpath);
      }
      *value_return = ans;
      *length_return = strlen(ans);
      *type_return = *target;
      *format_return = 8;
    }
    else
      status = 0;
  }
  else
    status = 0;
  return status;
}

static void loose_selection_proc(Widget w, Atom *selection)
{
  int i;
  for (i=0; i<num_selected; i++)
    selections[i]->highlighted = 0;
  ListTreeRefresh(w);
}

void add_to_selected(ListTreeItem *item, Boolean first)
{
  if (first) num_selected = 0;
  if (num_selected < MAX_SELECTIONS) {
    selections[num_selected++] = item;
  }
  else
    item->highlighted = 0;
}

static void FixPixMap(Widget w, ListTreeItem *itm)
{
    static char *get_on = "GETNCI($, 'ON')";
    int usage = get_usage(itm);
    int on;
    static int c_nid;
    static DESCRIPTOR_NID(nid_dsc, &c_nid);
    c_nid = get_nid(itm);
    on = ReadInt(get_on, &nid_dsc MDS_END_ARG);
    if (on)
      ListTreeSetItemPixmaps (w, itm, icons[usage], icons[usage]);
    else
      ListTreeSetItemPixmaps (w, itm, off_icons[usage], off_icons[usage]);
}    

static void FixPixMaps(Widget w, ListTreeItem *itm)
{
  ListTreeItem *tmp;
  FixPixMap(w, itm);
  for (tmp=itm->firstchild; tmp; tmp=tmp->nextsibling)
    FixPixMaps(w, tmp);
}

static ListTreeItem *add_item(Widget tree, ListTreeItem *parent,int nid)
  {
    static char *get_usage = "GETNCI($, 'USAGE')";
    static int c_nid;
    static DESCRIPTOR_NID(nid_dsc, &c_nid);
    int usage;
    ListTreeItem *item;
    char *name;
    c_nid = nid;
    name = get_node_name(nid);
    item = ListTreeAdd(tree, parent, name);
    item->user_data = (XtPointer)malloc(sizeof(struct node));
    set_nid(item, nid);
    set_name(item, name);
    if (name[strlen(name)-1] == '.')
      set_populated(item, 0);
    else
      set_populated(item, 1);
    usage = ReadInt(get_usage, &nid_dsc MDS_END_ARG);
    set_usage(item, usage);
    FixPixMap(tree, item);
    return item;
  }

void add_descendents(Widget tree, ListTreeItem *item, int nid)
{
  static int c_nid;
  static DESCRIPTOR_NID(nid_dsc, &c_nid);
  static char *get_num_members = "GETNCI($, 'NUMBER_OF_MEMBERS')";
  static char *get_num_children = "GETNCI($, 'NUMBER_OF_CHILDREN')";
  static char *get_mem_nid = "GETNCI($, 'MEMBER')";
  static char *get_bro_nid = "GETNCI($, 'BROTHER')";
  static char *get_child_nid = "GETNCI($, 'CHILD')";
  int num;
  int i;
  int mem_nid;
  int child_nid;

  c_nid = nid;

  item->open = 1;
  num = ReadInt(get_num_members, &nid_dsc MDS_END_ARG);
  for (i=0; i<num; i++) {
    mem_nid = ReadInt((i==0)?get_mem_nid:get_bro_nid, &nid_dsc MDS_END_ARG);
    add_item(tree, item, mem_nid);
    c_nid = mem_nid;
  }

  c_nid = nid;
  num = ReadInt(get_num_children, &nid_dsc MDS_END_ARG);
  for (i=0; i<num; i++) {
    child_nid = ReadInt((i==0)?get_child_nid:get_bro_nid, &nid_dsc MDS_END_ARG);
    add_item(tree, item, child_nid);
    c_nid = child_nid;
  }
  set_populated(item, 1);
}

static ListTreeItem *insert_item(Widget tree, ListTreeItem *parent,int nid)
  {
    ListTreeItem *itm;
    char *node_name=get_node_name(nid);
    itm = ListTreeFindChildName(tree, parent, node_name);
    if (itm == NULL) {
      if (!is_populated(parent)) {
        ListTreeRefreshOff(tree);
	add_descendents(tree, parent, get_nid(parent));
	ListTreeRefreshOn(tree);
	itm = ListTreeFindChildName(tree, parent, node_name);
      }else{
	itm = add_item(tree, parent, nid);
	ListTreeOrderChildren(tree, parent);
      }
    }
    {
      int status;
      int def_nid;
      int parent_nid = get_nid(parent);
      char *name = get_node_name(parent_nid);
      status = TreeGetDefaultNid(&def_nid);
      if ((status&1) &&(parent_nid==def_nid)) {
        char *tmp = malloc(strlen(name)+3+3+1);
        strcpy(tmp, "<<<");
        strcat(tmp, name);
        strcat(tmp,">>>");
        ListTreeRenameItem(tree, parent, tmp); 
        free(tmp);
      } else
        ListTreeRenameItem(tree, parent, name);
    } 
      return itm;
  }
      
void HighlightCallback(Widget w, XtPointer client, XtPointer call)
{
ListTreeMultiReturnStruct *ret;
ListTreeItem *item;
int count,i;

	ret=(ListTreeMultiReturnStruct *)call;
	for (i=0; i<ret->count; i++) {
	  item=ret->items[i];
          add_to_selected(item, i==0);
	  while (item->parent) {
	    item=item->parent;
	  }
	}
/*        XtOwnSelection(w, XA_PRIMARY, XtLastTimestampProcessed(XtDisplay(w)), convert_proc, loose_selection_proc, NULL); */
        XtOwnSelection(w, XA_PRIMARY, CurrentTime, convert_proc, loose_selection_proc, NULL);
        menu_item = 0;
}

void
MenuCallback(w,client,call)
Widget w;
XtPointer client;
XtPointer call;
{
    ListTreeItemReturnStruct *ret = (ListTreeItemReturnStruct *)call;
    Widget popup = XtNameToWidget(BxFindTopShell(w), "*.rightButtonMenu");
    menu_item = ret->item;
    XmMenuPosition(popup, (XButtonPressedEvent *)ret->event);
    XtManageChild(popup);
}

void ActivateCallback(Widget w, XtPointer client, XtPointer call)
{
  ListTreeActivateStruct *ret;
  ListTreeMultiReturnStruct ret2;
  ListTreeItem *item;
  int count;
  struct node *node_ptr;
  int nid;

	ret=(ListTreeActivateStruct *)call;
	count=0;
	while (count<ret->count) {
		count++;
	}
        node_ptr = (struct node *)(ret->path[count-1]->user_data);
        if (!is_populated(ret->path[count-1]))  {
          ListTreeRefreshOff(w);
          add_descendents(w, ret->path[count-1], get_nid(ret->path[count-1]));
          ListTreeRefreshOn(w);
        }
  menu_item = 0;
}


static ListTreeItem *Open(Widget tree, int nid)
{
  ListTreeItem *parent_item, *item;
  int parent;
  if (nid == 0) {
    item = ListTreeFirstItem(tree);
    if (!item->open) 
      add_descendents(tree, item, nid);
    return item;
  }
  parent = parent_nid(nid);
  parent_item = Open(tree, parent);
  for(item=parent_item->firstchild; ((item != NULL) && (get_nid(item) != nid)); item = item->nextsibling);
  if (item)
    if (!item->open)
      add_descendents(tree, item, nid);
  return (item==NULL) ? parent_item : item;    
}

static void set_default(Widget w, ListTreeItem *item)
{
  int status;
  int old_def;
  int nid = get_nid(item);
  Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
  if (default_item != NULL) {
    char *name = get_node_name(get_nid(default_item));
    ListTreeRenameItem(tree, default_item, name);
  }
  status = TreeSetDefaultNid(nid);
  if (status&1) {
    char *name = get_node_name(nid);
    char *new_name = malloc(strlen(name)+3+3+1);
    strcpy(new_name, "<<<");
    strcat(new_name, name);
    strcat(new_name, ">>>");
    ListTreeRenameItem(tree, item, new_name);
    free(new_name);
    default_item = item;
  }
  else
    XmdsComplain(w, "Error setting default Nid");
}

static void Init(Widget tree)
{
  ListTreeItem *item;
  char *getdbi = "GETDBI('NUMBER_OPENED')";
  int num = ReadInt(getdbi MDS_END_ARG);
  if (num > 0) {
    int status;
    int nid;
    ListTreeRefreshOff(tree);
    item = add_item(tree, NULL, 0);  /* add the top with no parent */
    default_item = 0;
    status = TreeGetDefaultNid(&nid);
    if (status&1) {
      item = Open(tree, nid);
      set_default(tree, item);
    }
    ListTreeRefreshOn(tree);
  }
}

static void CommandLineOpen(Display *display, Widget tree)
{
  int status;
  typedef struct {
	int shot;
	Boolean edit;
        Boolean read_only;
        String tree;
  } OptionsRec;

  static OptionsRec options;
  static XtResource resources[] = {
	{"tree", "Tree", XtRString, sizeof(String), XtOffsetOf(OptionsRec, tree), XtRString,  "test"},
	{"shot", "Shot", XtRInt,    sizeof(int),    XtOffsetOf(OptionsRec, shot), XtRImmediate, (XtPointer)-1}, 
	{"edit", "Edit", XtRBoolean,sizeof(Boolean), XtOffsetOf(OptionsRec, edit), XtRImmediate, (XtPointer)FALSE}, 
	{"read_only", "Read_only", XtRBoolean,sizeof(Boolean), XtOffsetOf(OptionsRec, read_only), XtRImmediate, (XtPointer)FALSE}};
 
  XtGetApplicationResources(BxFindTopShell(tree), (XtPointer)&options, resources, XtNumber(resources), (Arg *)NULL, 0);
  {
    int status;
    if (options.edit)
      status = TreeOpenEdit(options.tree, options.shot);
    else
      status = TreeOpen(options.tree, options.shot, options.read_only);
    if (status&1)
      Init(tree);
  }
}

static ListTreeItem *FindParentItemByNid(Widget tree, int nid)
{
  static char *getnci = "GETNCI($, 'PARENT')";
  static int c_nid;
  static DESCRIPTOR_NID(nid_dsc, &c_nid);
  int parent_nid;
  c_nid = nid;  
  parent_nid = ReadInt(getnci, &nid_dsc MDS_END_ARG);
  return Open(tree,(parent_nid>0)? parent_nid : 0);
}

static ListTreeItem *FindChildItemByNid(Widget tree, ListTreeItem *parent, int nid)
{
  ListTreeItem *itm;
  Boolean found;

  if (nid==0) return parent;
  for (itm=parent->firstchild; itm && !found; ) {
    found = get_nid(itm) == nid;
    if (!found) itm=itm->nextsibling;
  }
  return itm;
}

static Widget toplevel;

static Boolean notify_on=TRUE;

static void NodeTouched(int nid, NodeTouchType type)
{
  if (notify_on) {
    Widget tree = XtNameToWidget(BxFindTopShell(toplevel), "*.tree");
    ListTreeItem *this_item;
    ListTreeRefreshOff(tree);
    this_item = FindParentItemByNid(tree, nid);
    if (this_item != NULL) {
      switch(type) {
      case on_off:      FixPixMaps(tree, this_item); break;
      case set_def:     set_default(toplevel, FindChildItemByNid(tree, this_item, nid)); break;
      case new:         this_item = insert_item(tree, this_item, nid); break;
      }
    }
    ListTreeRefreshOn(tree);
  }
}


void MessageDismiss(Widget w)
{
  Widget box_w = XtParent(w);
  XtUnmanageChild(box_w);
}

void MessageClear(Widget w)
{
  Widget text_w = XtNameToWidget(XtParent(w), "*message_text");
  XmTextSetString(text_w, "");
}

static void TOutput(char *text)
{
  char *txt = malloc(strlen(text)+2);
  Widget message_box;
  Widget message_text;
  message_box = XtNameToWidget(toplevel, "*message_box");
  message_text = XtNameToWidget(message_box, "*message_text");
  strcpy(txt, text);
  strcat(txt, "\n");
  XmTextInsert(message_text, XmTextGetLastPosition(message_text), txt);
  XmTextShowPosition(message_text, XmTextGetLastPosition(message_text));
  free(txt);
}

static void TCLOutput(char *text)
{
  Widget message_box = XtNameToWidget(toplevel, "*message_box");
  if (!XtIsManaged(message_box))
    XtManageChild(message_box);
  TOutput(text);
}

static void InitializeCommandInterface(Widget w)
{
  static DESCRIPTOR(const image_name, "tcl_commands");
  static DESCRIPTOR(const routine_name, "TclSetCallbacks");
  static DESCRIPTOR(const set_command, "set command tcl_commands -def_file=*.tcl");
  int status = mdsdcl_do_command(&set_command);
  if (status&1) {
    int (*set_callbacks)();
    status = LibFindImageSymbol(&image_name, &routine_name, &set_callbacks);
    if (status&1) {
      status =set_callbacks(TCLOutput, TCLOutput, NodeTouched);
    }
  }
  toplevel = BxFindTopShell(w);
}


void
AddListTree( Widget w, XtPointer client_data, XtPointer call_data)
{
        Widget tree;
	ListTreeItem *item;
        tree = XtCreateWidget("tree", listtreeWidgetClass, w, NULL, 0);
        XtVaSetValues(tree,
          XtNheight,      	(Dimension)200,
          XtNwidth,		(Dimension)150,
          XtNhorizontalSpacing,   5,
          XtNverticalSpacing,     5,
          XtNhighlightPath,       False,
          XtNdoIncrementalHighlightCallback, True, 
          XtNclickPixmapToOpen,   False,
          NULL);
        XtManageChild(tree);
        XmMainWindowSetAreas(w, NULL, NULL, NULL, NULL, tree);
        XmdsUsageIconsInitialize(w);
        icons = XmdsUsageIcons();
        off_icons = XmdsUsageGrayIcons();
	XtAddCallback(tree,XtNhighlightCallback, HighlightCallback, (XtPointer) NULL);
	XtAddCallback(tree,XtNactivateCallback, ActivateCallback, (XtPointer) NULL);
	XtAddCallback(tree,XtNmenuCallback, MenuCallback, (XtPointer) NULL);
        CommandLineOpen(XtDisplay(tree), tree);
        InitializeCommandInterface(w);
}
/*      Function Name:	BxExitCB
 *
 *      Description:   	This functions expects an integer to be passed in
 *		       	client data.  It calls the exit() system call with
 *			the integer value as the argument to the function.
 *
 *      Arguments:      Widget		w: 	the activating widget.
 *			XtPointer	client:	the integer exit value.
 *			XtPointer	call:	the call data (unused).
 */

#ifdef VMS
#include <stdlib.h>
#endif

/* ARGSUSED */
void
BxExitCB ARGLIST((w, client, call))
UARG( Widget, w)
ARG( XtPointer, client)
GRAU( XtPointer, call)
{
    int		*exitValue = (int*)client;
    
    exit(*exitValue);
}


/*      Function Name: 	BxManageCB
 *
 *      Description:   	Given a string of the form:
 *		       	"(WL)[widgetName, widgetName, ...]"
 *			BxManageCB attempts to convert the name to a Widget
 *			ID and manage the widget.
 *
 *      Arguments:     	Widget	    w:      the widget activating the callback.
 *		       	XtPointer   client: the list of widget names to attempt
 *					    to find and manage.
 *		       	XtPointer   call:   the call data (unused).
 *
 *      Notes:        *	This function expects that there is an application
 *		       	shell from which all other widgets are descended.
 */

/* ARGSUSED */
void
BxManageCB ARGLIST((w, client, call))
ARG( Widget, w)
ARG( XtPointer, client)
GRAU( XtPointer, call)
{
    WidgetList		widgets;
    int			i;

    /*
     * This function returns a NULL terminated WidgetList.  The memory for
     * the list needs to be freed when it is no longer needed.
     */
    widgets = BxWidgetIdsFromNames(w, "BxManageCB", (String)client);

    i = 0;
    while( widgets && widgets[i] != NULL )
    {
	XtManageChild(widgets[i]);
	i++;
    }
    XtFree((char *)widgets);
}

static void display_data(Widget w, int nid, int count)
{
  static int x, y;
  Arg args[] = {
		 {XmdsNnid, 0},
		 {XmNx, 0},
		 {XmNy, 0},
		 {XmdsNdisplayOnly, 1},
                 {XmNdefaultPosition,1},
                 {XmdsNautoPut, 1},
		 {XmdsNautoDestroy, 1}
	       };
  args[0].value = nid;
  args[1].value = x;
  args[2].value = y;
  if (count == 0) {
      XtManageChild(w = XmdsCreateXdBoxDialog(BxFindTopShell(w), "Display Data", args, XtNumber(args)-2));
      XtVaGetValues(w, XmNx, &x, XmNy, &y, 0);
  }
  else {
    x += 20;
    y += 20;
    args[3].value = 0;
    XtManageChild(w = XmdsCreateXdBoxDialog(BxFindTopShell(w), "Display Data", args, XtNumber(args)));
  }
}

void
DisplayData( Widget w, XtPointer client_data, XtPointer call_data)
{
    int i;
    for (i=0; i<num_selected; i++) {
      int nid = get_nid(selections[i]);
      display_data(w, nid, i);
    }
}

void
TurnOnOff( Widget w, XtPointer client_data, XtPointer call_data)
{
    int i;
    int on = *(int *)client_data;
    Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
    XmAnyCallbackStruct *acs=(XmAnyCallbackStruct*)call_data;
    ListTreeRefreshOff(tree);
    for (i=0; i<num_selected; i++) {
      int nid = get_nid(selections[i]);
      switch (on) {
        case 0 : 
	  TreeTurnOff(nid);  break;
	case 1 : TreeTurnOn(nid); break;
      }
      FixPixMaps(tree, selections[i]); 
    }
    ListTreeRefreshOn(tree);
}

static void modify_data(Widget w, int nid, int count)
{
  static int x, y;
  Arg args[] = {
		 {XmdsNnid, 0},
		 {XmNx, 0},
		 {XmNy, 0},
		 {XmdsNdisplayOnly, 0},
                 {XmNdefaultPosition,1},
                 {XmdsNautoPut, 1},
                 {XmdsNputOnApply, 1},
		 {XmdsNautoDestroy, 1}
	       };
  args[0].value = nid;
  args[1].value = x;
  args[2].value = y;
  if (count == 0) {
      XtManageChild(w = XmdsCreateXdBoxDialog(BxFindTopShell(w), "Modify Data", args, XtNumber(args)));
      XtVaGetValues(w, XmNx, &x, XmNy, &y, 0);
  }
  else {
    x += 20;
    y += 20;
    args[3].value = 0;
    XtManageChild(w = XmdsCreateXdBoxDialog(BxFindTopShell(w), "Modify Data", args, XtNumber(args)));
  }
}

void
ModifyData( Widget w, XtPointer client_data, XtPointer call_data)
{
    int i;
    for (i=0; i<num_selected; i++) {
      int nid = get_nid(selections[i]);
      modify_data(w, nid, i);
    }
}

static int DoMethodNoSignal(struct descriptor *niddsc, struct descriptor *method, Widget parent)
{
  /*  LibEstablish(LibSigToRet); */
  return TreeDoMethod(niddsc, method, parent);
}

static int setup_device(Widget parent, int nid)
{
  static DESCRIPTOR(const method, "DW_SETUP");
  struct descriptor niddsc = {4, DTYPE_NID, CLASS_S, 0};
  volatile int status;
  niddsc.pointer = (char *)&nid;
  /*  status = DoMethodNoSignal(&niddsc, (struct descriptor *)&method, parent); */
  /*  if (status == TreeNOMETHOD) */
  {
    static char *getnci = "GETNCI($, 'CONGLOMERATE_ELT')";
    static int c_nid;
    static DESCRIPTOR_NID(nid_dsc, &c_nid);
    int  conglomerate_elt;
    static struct descriptor model = {0,DTYPE_T, CLASS_D, 0};
    static int head_nid;
    c_nid = nid;
    conglomerate_elt = ReadInt(getnci, &nid_dsc MDS_END_ARG);
    head_nid = nid + 1 - conglomerate_elt;
    c_nid = head_nid;
    if (TdiModelOf(&nid_dsc,&model MDS_END_ARG) & 1)
    {
      static struct descriptor filename = {0, DTYPE_T, CLASS_D, 0};
      /*      static DESCRIPTOR(const prefix, "DECW$SYSTEM_DEFAULTS:"); */
      static DESCRIPTOR(const postfix, ".uid\0");
      static DESCRIPTOR(const zero,"\0");
      StrTrim(&model,&model,0);
      StrConcat(&filename,&model,&postfix MDS_END_ARG);
      StrAppend(&model,&zero MDS_END_ARG);
      status = XmdsDeviceSetup(parent, &c_nid, &filename.pointer, 1, model.pointer, NULL, 0, 0);
      switch (status)
      {
        case MrmSUCCESS:  status = 1; break;
        case MrmNOT_FOUND: status = TreeNOMETHOD; break;
        default: XmdsComplain(parent, "Error in activating setup for this device");
                 status = 1;
      }
    }
  }
  return status;
}

void
SetupDevice( Widget w, XtPointer client_data, XtPointer call_data)
{
  int i;
  for (i=0; i<num_selected; i++) {
      int nid = get_nid(selections[i]);
      setup_device(w, nid);
  }
}

void
MenuUnmap( Widget w, XtPointer client_data, XtPointer call_data)
{
}

void
MSetupDevice( Widget w, XtPointer client_data, XtPointer call_data)
{
  if (menu_item != NULL) {
      int nid = get_nid(menu_item);
      setup_device(w, nid);
      menu_item = NULL;
  }
}

void
MTurnOnOff( Widget w, XtPointer client_data, XtPointer call_data)
{
    int on = *(int *)client_data;
    Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
    ListTreeRefreshOff(tree);
    if (menu_item != NULL) {
      int nid = get_nid(menu_item);
      if (on)
	TreeTurnOn(nid);
      else
        TreeTurnOff(nid);
      FixPixMaps(tree, menu_item); 
      menu_item = NULL;
    }
    ListTreeRefreshOn(tree);
}

void
MDisplayData( Widget w, XtPointer client_data, XtPointer call_data)
{
    if (menu_item != NULL) {
      int nid = get_nid(menu_item);
      display_data(w, nid, 0);
      menu_item = NULL;
    }
}

void
MModifyData( Widget w, XtPointer client_data, XtPointer call_data)
{
    if (menu_item != NULL) {
      int nid = get_nid(menu_item);
      modify_data(w, nid, 0);
    }
}

static void display_nci(Widget bu, int nid, int count)
{
  static int x, y;
  XmString label = GetNciString(nid);
  Widget w;
  static XtCallbackRec ok_callback_list[] = {{(XtCallbackProc)XtDestroyWidget, 0},{0,0}};
  Arg args[] = {
                 {XmNmessageString, 0},
                 {XmNdialogTitle, 0},
                 {XmNokLabelString, 0},
                 {XmNx, 0},
                 {XmNy, 0},
                 {XmNokCallback, (long)ok_callback_list},
                 {XmNmessageAlignment, XmALIGNMENT_BEGINNING},
                 {XmNminimizeButtons, TRUE},
                 {XmNdefaultPosition,1}
               };
  args[0].value = (long)label;
  args[1].value = (long)XmStringCreateLtoR("Display Node Characteristics", XmSTRING_DEFAULT_CHARSET);
  args[2].value = (long)XmStringCreateLtoR("Dismiss", XmSTRING_DEFAULT_CHARSET);
  args[3].value = x;
  args[4].value = y;
  if (count == 0) {
    XtManageChild(w = XmCreateInformationDialog(BxFindTopShell(bu), "Display Nci", args, XtNumber(args)-2));
    XtVaGetValues(w, XmNx, &x, XmNy, &y, 0);
  }
  else {
    x += 20;
    y += 20;
    args[6].value = 0;
    XtManageChild(w = XmCreateInformationDialog(BxFindTopShell(bu), "Display Nci", args, XtNumber(args)));
  }
  XmStringFree(label);
}

void
DisplayNci( Widget w, XtPointer client_data, XtPointer call_data)
{
    int i;
    for (i=0; i<num_selected; i++) {
      int nid = get_nid(selections[i]);
      display_nci(w, nid, i);
    }
}

void
MDisplayNci( Widget w, XtPointer client_data, XtPointer call_data)
{
    if (menu_item != NULL) {
      int nid = get_nid(menu_item);
      display_nci(w, nid, 0);
      menu_item = NULL;
    }
}

void CloseTree( Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
    ListTreeItem *top = ListTreeFirstItem(tree);
    static char *getediting = "GETDBI('OPEN_FOR_EDIT') &&  GETDBI('MODIFIED')";
    if (ReadInt(getediting MDS_END_ARG)) {
      Widget write_dlog = XtNameToWidget(BxFindTopShell(w), "*.writeDialog");
      XtManageChild(write_dlog);
    }
    else {
      int status = TreeClose(NULL, NULL);
      ListTreeRefreshOff(tree);
      if ((status&1) && (top != NULL)) ListTreeDelete(tree, top);
      Init(tree);
    }
}

void WriteTree( Widget w, XtPointer client_data, XtPointer call_data)
{
    int status;
    int write = *(int *)client_data;
    if (write) {
      status = TreeWriteTree(0, 0);
      if (status)
	status = TreeClose(NULL, NULL);
    }
    else
      status = TreeQuitTree(0, 0);
    if (status&1) {
      Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
      ListTreeItem *top = ListTreeFirstItem(tree);
      ListTreeRefreshOff(tree);
      if ((status&1) && (top != NULL)) ListTreeDelete(tree, top);
      Init(tree);
      ListTreeRefreshOn(tree);
    } 
    else
      XmdsComplain(BxFindTopShell(w), "Error writing or quiting from tree");
}

void open_tree(Widget w, char *tree, int shot)
{
  int status;
  Widget edit = XtNameToWidget(BxFindTopShell(w), "*.edit_toggle");
  if (XmToggleButtonGetState(edit))
    status = TreeOpenEdit(tree, shot);
  else {
    Widget r_o = XtNameToWidget(BxFindTopShell(w), "*.r_o_toggle");
    status = TreeOpen(tree, shot, XmToggleButtonGetState(r_o));
  }
  if (status&1) {
    Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
    ListTreeItem *top = ListTreeFirstItem(tree);
    ListTreeRefreshOff(tree);
    if (top != NULL) ListTreeDelete(tree, top);
    Init(tree);
    ListTreeRefreshOn(tree);
  }
}
  
void OpenTree( Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget parentw=XtParent(w);
    Widget treew = XtNameToWidget(parentw, "tree_name");
    char *tree = XmTextGetString(treew);
    Widget shotw = XtNameToWidget(parentw, "shot_name");
    char *shot_str = XmTextGetString(shotw);;
    if (tree && strlen(tree) && shot_str && strlen(shot_str)) {
      int lshot;
      int status = sscanf(shot_str, "%d", &lshot);
      if (status)
        open_tree(w, tree, lshot);
      else
        XmdsComplain(parentw, "shot specifed as /%s/\nmust be an integer\nPlease reeenter or choose \"Cancel\"", shot_str);
    }
    else
      XmdsComplain(parentw, "Specify both a tree and a shot\nbefore choosing \"Ok\"\nUse \"Cancel\" to abort operation");
}

void
MDoAction( Widget w, XtPointer client_data, XtPointer call_data)
{
    XmAnyCallbackStruct *acs=(XmAnyCallbackStruct*)call_data;
}

void
SetDefault( Widget w, XtPointer client_data, XtPointer call_data)
{
    int i;
    for (i=0; i<num_selected; i++) {
      set_default(w, selections[i]);
    }
}

void
MSetDefault( Widget w, XtPointer client_data, XtPointer call_data)
{
    if (menu_item != NULL) {
      set_default(w, menu_item);
      menu_item = NULL;
    }
}

void
DoAction( Widget w, XtPointer client_data, XtPointer call_data)
{
    XmAnyCallbackStruct *acs=(XmAnyCallbackStruct*)call_data;
}

static unsigned int usage=0;
static char *device_type=0;

void
AddNodeDismiss( Widget w, XtPointer client_data, XtPointer call_data)
{
  XtUnmanageChild(w);
}

extern int GetSupportedDevices();

Boolean add_node(Widget w, ListTreeItem *parent, char *name, int usage, ListTreeItem **itm)
{
  int parent_nid = get_nid(parent);
  char *parent_path;
  char *full_path;
  int status;
  static char *getnci = "GETNCI($, 'FULLPATH')";
  static int c_nid;
  static DESCRIPTOR_NID(nid_dsc, &c_nid);
  int new_nid;
  c_nid = parent_nid;
  parent_path = ReadString(getnci, &nid_dsc MDS_END_ARG);
  full_path=realloc(parent_path, strlen(parent_path)+1+strlen(name)+1);
  if (usage == TreeUSAGE_SUBTREE) {
    XmdsComplain(BxFindTopShell(w), "Non Motif traverser does not\nyet support adding subtrees to the tree");
    return 0;
  }
    
  if ((usage == TreeUSAGE_STRUCTURE) || (usage == TreeUSAGE_SUBTREE))
    strcat(full_path, ".");
  else
    strcat(full_path, ":");
  strcat(full_path,name);
  if (usage == TreeUSAGE_DEVICE) {
    status = 0;
    if (device_type) {
      notify_on = FALSE;
      status = TreeAddConglom(full_path, device_type, &new_nid);
      notify_on = TRUE;
      if (!(status&1))
	XmdsComplain(BxFindTopShell(w), "Error adding device");
    }
    else
      XmdsComplain(BxFindTopShell(w), "Select a device type before choosing add");
  } else {
    status = TreeAddNode(full_path, &new_nid, usage);
    if (!(status&1))
      XmdsComplain(BxFindTopShell(w), "Error adding node");
  }
  if (status&1) {
     Widget tree = XtNameToWidget(BxFindTopShell(w), "*.tree");
     *itm = insert_item(tree, parent, new_nid);
  }
  return status&1;
}

void add_tags(ListTreeItem *itm, char *tags)
{
}

Boolean AddNodeApply(Widget w)
{
  Boolean status = 0;
  if (TreeEditing()) {
    if (num_selected == 1) {
      if (usage != 0) {
        Widget name_w = XtNameToWidget(XtParent(w), "*.nodeName");
        char *name_c = XmTextGetString(name_w);
        if (strlen(name_c) > 0) {
          if ((index(name_c, '.') == name_c) || (index(name_c, ':') == name_c))
            XmdsComplain(w, "Remove leading punctuation");
          else {
            if (strlen(name_c) > 12)
              XmdsComplain(w, "Node names must be no more than 12 characters long");
	    else {
              ListTreeItem *ret_itm;
	      status = add_node(w, selections[0], name_c, usage, &ret_itm);
              if (status) {
                Widget tag_w = XtNameToWidget(XtParent(w), "*.nodeTags");
                char *tags_c = XmTextGetString(tag_w);
                if (strlen(tags_c) > 0)
                  add_tags(ret_itm, tags_c);
              }
              status = TRUE;
            }
	  }
        }
        else
          XmdsComplain(w, "Specifiy a name before \"Ok\"");
      }
      else
        XmdsComplain(w, "Please choose a usage before pressing \"Ok\"");
    }
    else
      XmdsComplain(w, "Select exactly one node to be the parent!");
  }
  else
    XmdsComplain(w, "Tree not open for edit");
  return status;
}

void
AddNode( Widget w, XtPointer client_data, XtPointer call_data)
{
  if (AddNodeApply(w))
    XtUnmanageChild(w);
}
void
SetUsage( Widget w, XtPointer client_data, XtPointer call_data)
{
  XmToggleButtonCallbackStruct *cb = (XmToggleButtonCallbackStruct *)call_data;
  if (cb->set)
    usage = *(int *)client_data;
}
void SetDeviceType( Widget w, XtPointer client_data, XtPointer call_data)
{
  XmToggleButtonCallbackStruct *cb = (XmToggleButtonCallbackStruct *)call_data;
  device_type = realloc(device_type, strlen(w->core.name)+1);
  strcpy(device_type, w->core.name);
}  
void
CommandEntered( Widget w, XtPointer client_data, XtPointer call_data)
{
  XmCommandCallbackStruct *cb = (XmCommandCallbackStruct *)call_data;
  char *cmd;
  int status;
  XmStringGetLtoR(cb->value, XmSTRING_DEFAULT_CHARSET, &cmd);
  status = mdsdcl_do_command(cmd);
} 

void
CreateAddDevice( Widget w, XtPointer client_data, XtPointer call_data)
{
  static Boolean devices_loaded = False; 
  char **devnames;
  char **imagenames;
  int num;
  Widget top;
  usage = TreeUSAGE_DEVICE;
  top = XtNameToWidget(BxFindTopShell(w), "*.addDeviceDialog");
  if (!devices_loaded) {
  static Arg args[] = {
                        {XmNarmCallback, 0}
                      };
    int i;
    Widget rb;
    int status = GetSupportedDevices(&devnames, &imagenames, &num);
    static XtCallbackRec device_changed_list[] = {{(XtCallbackProc)SetDeviceType, 0},{0,0}};
    args[0].value = (long)device_changed_list;
    rb = XtNameToWidget(top, "*.ad_radioBox1");
    for (i=0; i<num; i++) {
      
      Widget w = XmCreateToggleButton(rb, devnames[i], args, 1);
      XtManageChild(w);
    }
    devices_loaded = True;
  }
  XtManageChild(top);
}

void
AddDevice( Widget w, XtPointer client_data, XtPointer call_data)
{
  XtUnmanageChild(w);
}
void
AddDeviceDismiss( Widget w, XtPointer client_data, XtPointer call_data)
{
  XtUnmanageChild(w);
}
