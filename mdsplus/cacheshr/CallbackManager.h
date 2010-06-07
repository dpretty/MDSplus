#ifndef  CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H
#include <Notifier.h>


class CallbackManager:Runnable
{
	_int64 next, prev;
	Notifier notifier;
	int nid;
	void (*callback)(int, void *);

public:
	void setNext(char *nxt)
	{
		if(nxt == NULL)
			next = 0;
		else
			next = reinterpret_cast<_int64>(nxt) - reinterpret_cast<_int64>(this);
	}
	void setPrev(char *prv)
	{
		if(prv == NULL)
			prev = 0;
		else 
			prev = reinterpret_cast<_int64>(prv) - reinterpret_cast<_int64>(this);
	}

	CallbackManager *getNext()
	{
		if(next == 0) return NULL;
		return (CallbackManager *)((char *)this + next);
	}
	CallbackManager *getPrev()
	{
		if(prev == 0) return NULL;
		return (CallbackManager *)((char *)this + prev);
	}

	void run(void *arg)
	{
		callback(nid, arg);
	}
	void initialize(int nid, void *argument, void (*callback)(int, void *))
	{
		notifier.initialize(NULL, this, argument);
	}


	void dispose()
	{
		notifier.dispose();
	}
	void callCallback()
	{
		notifier.trigger();
		CallbackManager *nxt = getNext();
		if(nxt)
			nxt->callCallback();
	}

};
#endif
