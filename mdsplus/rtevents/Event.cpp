#include "SharedMemManager.h"
#include "event.h"

extern "C" EXPORT void EventWait(char *name, char *buf, int size, int *retSize);
extern "C" EXPORT void EventReset();
extern "C" EXPORT void * EventAddListenerGlobal(char *name,  void (*callback)(char *, char *, int, void *, bool, int, char *, int), void *callbackArg);
extern "C" EXPORT void * EventAddListener(char *name,  void (*callback)(char *, char *, int, void *, bool, int, char*, int), void *callbackArg);
extern "C" EXPORT void EventRemoveListener(void *eventHandler);
extern "C" EXPORT  int EventTrigger(char *name, char *buf, int size);
extern "C" EXPORT int EventTriggerAndWait(char *name, char *buf, int size);
extern "C" EXPORT int EventTriggerAndTimedWait(char *name, char *buf, int size, int millisecs);
extern "C" EXPORT void EventClean();

//////////////////////////External C Interface/////////////////////

void Event::reset()
{
	GlobalLock eventLock;
	eventLock.initialize(EVENT_ID);
	eventLock.lock();
	bool firstCreated = memManager->initialize(EVENT_ID, EVENT_SIZE);
	memManager->reset();
	evManager = (EventManager *)memManager->allocate(sizeof(EventManager));
	evManager->initialize();
	eventLock.unlock();
	eventLock.dispose();
}

EXPORT void EventReset()
{
	Event ev;
	ev.reset();
}

EXPORT void * EventAddListenerGlobal(char *name,  void (*callback)(char *, char *, int, void *, bool, int, char *, int), void *callbackArg)
{
	Event ev;
	try {
		void *handl = ev.addListenerGlobal(name, callback, callbackArg);
		return handl;
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
		return NULL;
	}
}
EXPORT void * EventAddListener(char *name,  void (*callback)(char *, char *, int, void *, bool, int, char*, int), void *callbackArg)
{
	Event ev;
	try {
		void *handl = ev.addListener(name, callback, callbackArg);
		return handl;
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
		return NULL;
	}
}
EXPORT void EventRemoveListener(void *eventHandler)
{
	Event ev;
	try {
		ev.removeListener(eventHandler);
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
	}
	
}

EXPORT  int EventTrigger(char *name, char *buf, int size)
{
	Event ev;
	try {
		ev.trigger(name, buf, size);
		return 0;
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
		return -1;
	}
	
}
EXPORT int EventTriggerAndWait(char *name, char *buf, int size)
{
	Event ev;
	try {
		ev.triggerAndWait(name, buf, size);
		return 0;
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
		return -1;
	}
}
//	bool triggerAndWait(const char *eventName, char *buf, int size, bool copyBuf = true, Timeout *timeout = 0)

EXPORT int EventTriggerAndTimedWait(char *name, char *buf, int size, int millisecs)
{
	Event ev;
	try {
		Timeout *timout = new Timeout(millisecs);
		ev.triggerAndWait(name,  buf, size, true, timout);
		delete timout;
		return 0;
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
		return -1;
	}
}

EXPORT void EventClean()
{
	Event ev;
	try {
		ev.clean(100);
	}
	catch(SystemException *exc)
	{
		printf("%s\n", exc->what());
	}
}

struct WaitDescr
{
	UnnamedSemaphore *sem;
	char *buf;
	int size;
	int *retSize;
};

static void waitCallback(char *name, char *buf, int size, void *arg, bool isTimed, int retSize, char *retBuf, int type)
{
	WaitDescr *waitDsc = (WaitDescr *)arg;
	int actSize = (size < waitDsc->size)?size:waitDsc->size;
	memcpy(waitDsc->buf, buf, actSize);
	*(waitDsc->retSize) = actSize;
	waitDsc->sem->post();
}




EXPORT void EventWait(char *name, char *buf, int size, int *retSize)
{

	UnnamedSemaphore sem;
	sem.initialize(0);
	Event ev;
	WaitDescr waitDsc;
	waitDsc.sem = &sem;http://www.repubblica.it/2009/07/sezioni/spettacoli_e_cultura/trailer-classifica/trailer-classifica/trailer-classifica.html?ref=hpspr1
	waitDsc.size = size;
	waitDsc.retSize = retSize;
	waitDsc.buf = buf;

	void *evHandler = ev.addListener(name, waitCallback, (void *)&waitDsc);
	sem.wait();
	ev.removeListener(evHandler);
	sem.dispose();
}
