#ifndef THREAD_H_
#define THREAD_H_
#ifdef HAVE_WINDOWS_H
#include "WindowsThread.h"
#else
#include <pthread.h>
#include "Runnable.h"

struct  WithArg{
	Runnable *rtn;
	void *arg;
};
extern  "C" void handlerWithArg(WithArg *);

class Thread
{
	pthread_t thread;
public:
	void start(Runnable *rtn, void *arg = 0);
	void join();
	friend void handlerWithArg(WithArg *rtn);
};



#endif
#endif /*THREAD_H_*/
