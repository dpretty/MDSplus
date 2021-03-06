/*------------------------------------------------------------------------------

                Name:   MDSEVENTV5

                Type:   C function

                Author: TOM FREDIAN

                Date:   14-DEC-1993

                Purpose:  Implement MDS events in IDL Widget applications

------------------------------------------------------------------------------

        Call sequence:

EventStruct *MDSEVENT(int *base_id, int *stub_id, struct dsc$descriptor *name)

------------------------------------------------------------------------------
   Copyright (c) 1993
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

        Description:

Invoked from MDSEVENT.PRO

------------------------------------------------------------------------------*/
#include <ipdesc.h>

extern int MDSEventAst();
extern int MDSEventCan();

typedef struct _event_struct { int stub_id;
                               int base_id;
                               int event_id;
                               char name[28];
                               char value[12];
                               int loc_event_id;
#ifdef WIN32
							   void *thread_handle;
							   DWORD thread_id;
#endif
                               struct _event_struct *next;
                             } EventStruct;

#include <stdio.h>
#ifdef WIN32
#include <windows.h>
extern unsigned long WINAPI MdsDispatchEvent(SOCKET sock);
#else
#include <X11/Intrinsic.h>
static XtInputId XTINPUTID=0;
extern void MdsDispatchEvent(void *, int *, unsigned long *);
#endif

static EventStruct *EventList = (EventStruct *)0;
static int EventCount = 1;
static void EventAst(EventStruct *e, int eventid, char *data);
#include <export.h>
#if defined(__VMS) || defined(WIN32)
#define BlockSig(arg)
#define UnBlockSig(arg)
#else
static int BlockSig(int sig_number)
{
  sigset_t newsigset;
  sigemptyset(&newsigset);
  sigaddset(&newsigset,sig_number);
  return sigprocmask(SIG_BLOCK, &newsigset, NULL);
}

static int UnBlockSig(int sig_number)
{
  sigset_t newsigset;
  sigemptyset(&newsigset);
  sigaddset(&newsigset,sig_number);
  return sigprocmask(SIG_UNBLOCK, &newsigset, NULL);
}
#endif

int IDLMdsEventCan(int argc, void * *argv)
{
    EventStruct *e,*p;
	SOCKET sock = (SOCKET)((char *)argv[0] - (char *)0);
	int eventid = (unsigned int)((char *)argv[1] - (char *)0);
    int status;
    BlockSig(SIGALRM);
	status = (sock >= 0) ? MdsEventCan(sock, eventid) : MDSEventCan(eventid);
    UnBlockSig(SIGALRM);
    for (e=EventList,p=0;e && e->loc_event_id != eventid; p=e,e=e->next);
    if (e)
	{
#ifdef WIN32
		if (e->thread_handle)
			TerminateThread(e->thread_handle, 0);
#endif
		if (p)
			p->next = e->next;
		else
			EventList = e->next;
		free(e);
	}
	return status;
}

int IDLMdsGetevi(int argc, void **argv)
{
  int eventid = (unsigned int)((char *)argv[0] - (char *)0);
  EventStruct *e;
  for (e=EventList;e && e->loc_event_id != eventid;e=e->next);
  if (e) memcpy(argv[1],e,52);
  return (e!=0);
}

#ifndef WIN32
static int event_pipe[2];

static void DoEventUpdate(XtPointer client_data, int *source, XtInputId *id)
{
  char *stub_rec;
  char *base_rec;
  EventStruct *e;
  IDL_WidgetStubLock(TRUE);
  read(event_pipe[0],&e,sizeof(EventStruct *));
  if ((stub_rec = IDL_WidgetStubLookup(e->stub_id)) && (base_rec = IDL_WidgetStubLookup(e->base_id)))
  {
#ifdef WIN32
    HWND wid1, wid2;
#endif
    IDL_WidgetIssueStubEvent(stub_rec, (IDL_LONG)e);
#ifdef WIN32
    IDL_WidgetGetStubIds(stub_rec, (IDL_LONG *)&wid1, (IDL_LONG *)&wid2);
    PostMessage(wid1, WM_MOUSEMOVE, (WPARAM)NULL, (LPARAM)NULL);
#else
    {
      Widget top;
      Widget w;
      IDL_WidgetGetStubIds(base_rec, (unsigned long *)&top, (unsigned long *)&w);
      if (w)
      {
        XClientMessageEvent event;
        event.type = ClientMessage;
        event.display = XtDisplay(top);
        event.window = XtWindow(top);
        event.format = 8;
        XSendEvent(XtDisplay(top),XtWindow(top),TRUE,0,(XEvent *)&event);
        XFlush(XtDisplay(top));
      }
    }

#endif
  }
}


static void EventAst(EventStruct *e,int len, char *data)
{
  if (len > 0) memcpy(e->value,data,len > 12 ? 12 : len);
  write(event_pipe[1],&e,sizeof(EventStruct *));
}
#endif
int IDLMdsEvent(int argc, void * *argv)
{
  SOCKET sock = (SOCKET)((char *)argv[0] - (char *)0);
  int *base_id = (int *)argv[1];
  int *stub_id = (int *)argv[2];
  char *name = (char *)argv[3];
  EventStruct *e = (EventStruct *)malloc(sizeof(EventStruct));
  BlockSig(SIGALRM);
  {
    char *parent_rec;
    char *stub_rec;
    IDL_WidgetStubLock(TRUE);
    if ((parent_rec = IDL_WidgetStubLookup(*base_id))
        && (stub_rec = IDL_WidgetStubLookup(*stub_id)))
    {
      /* IDL_WidgetSetStubIds(stub_rec, parent_rec, parent_rec);   */
#ifdef WIN32
		if (sock > 0) {
			    e->thread_handle = CreateThread(
					(LPSECURITY_ATTRIBUTES)NULL, 
					0, 
					(LPTHREAD_START_ROUTINE)MdsDispatchEvent, 
					(LPVOID)sock, 
					(DWORD)NULL, 
					&e->thread_id);
		} else
			e->thread_handle = 0;
#else 
      if (!XTINPUTID) {
        Widget w1, w2;
        IDL_WidgetGetStubIds(parent_rec, (unsigned long *)&w1, (unsigned long *)&w2);
        if (sock >= 0) {
	  XtAppAddInput(XtWidgetToApplicationContext(w1), sock,  (XtPointer)XtInputExceptMask, MdsDispatchEvent, (void *)sock);
	}
	pipe(event_pipe);
	XTINPUTID = XtAppAddInput(XtWidgetToApplicationContext(w1), event_pipe[0], (XtPointer)XtInputReadMask, DoEventUpdate, 0);
      }
#endif
      e->stub_id = *stub_id;
      e->base_id = *base_id;
      e->loc_event_id = EventCount++;
      strncpy(e->name, name, sizeof(e->name));
      e->next = EventList;
      EventList = e;
      if (sock >=0) {
	MdsEventAst(sock, name,(void (*)(int))EventAst,e,&e->event_id);
      } else {
	MDSEventAst(name,(void (*)(int))EventAst,e,&e->event_id);
      }
      IDL_WidgetStubLock(FALSE);
      return e->loc_event_id;
    }
  }
  UnBlockSig(SIGALRM);
  /*  free((char *)e); */
  return -1;
}

#ifdef WIN32
static void EventAst(EventStruct *e,int len, char *data)
{
  char *stub_rec;
  char *base_rec;
  IDL_WidgetStubLock(TRUE);
  if (len > 0) memcpy(e->value,data,len > 12 ? 12 : len);
  if ((stub_rec = IDL_WidgetStubLookup(e->stub_id)) && (base_rec = IDL_WidgetStubLookup(e->base_id)))
  {
    HWND wid1, wid2;
    IDL_WidgetIssueStubEvent(stub_rec, (IDL_LONG)e);
    IDL_WidgetGetStubIds(stub_rec, (IDL_LONG *)&wid1, (IDL_LONG *)&wid2);
    PostMessage(wid1, WM_MOUSEMOVE, (WPARAM)NULL, (LPARAM)NULL);
  }

  IDL_WidgetStubLock(FALSE);
}
#endif
