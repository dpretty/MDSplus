#include "Notifier.h"

#ifdef HAVE_WINDOWS_H
#include "Notifier.h"

#define NUM_HANDLES 10000

extern "C" void handleEvents(ThreadInfo *info)
{
	while(TRUE)
	{
		WaitForSingleObject(info->handle, INFINITE);
		if(info->killed) 
		{
			_endthread();
		}
		info->callback(info->nid);
	}
}



HANDLE Notifier::getHandle(int nid)
{
	int i;
	for(i = 0; i < NUM_HANDLES; i++)
	{
		if(nids[i] == 0 || nids[i] == nid)
			break;
	}
	if(i < NUM_HANDLES)
	{
		if(!nids[i])
		{
			char *name[128];
			sprintf((char *)name, "NID_%d", nid);
			handles[i] =  OpenEvent(
					EVENT_ALL_ACCESS,
					FALSE,
					(char *)name);
			nids[i] = nid;
		}
		return handles[i];
	}
	return 0;
}

void Notifier::initialize(int nid, void (*callback)(int))
{
    info.nid = nid;
    info.killed = 0;
    info.callback = callback;
    
    char name[128];
    sprintf((char *)name, "NID_%d", nid);

    HANDLE eventHandle = CreateEvent(NULL, TRUE, FALSE, (char *)name);
    info.handle = eventHandle;
    _beginthread((void (*)(void *))handleEvents, 0, &info);
}

void Notifier::notify()
{
    PulseEvent(info.handle);
}

void Notifier::dispose()
{
    info.killed = 1;
    notify();
}

#else

#ifdef HAVE_VXWORKS_H
extern "C" void handleEvents(ThreadInfo *info)
{
	while(1)
	{
		semTake(info->semaphore, WAIT_FOREVER);
		if(info->killed) 
		{
			return;
		}
		info->callback(info->nid);
	}
}

void Notifier::initialize(int nid, void (*callback)(int))
{
    info.nid = nid;
    info.killed = 0;
    info.callback = callback;
    info.semaphore = semBCreate(SEM_Q_FIFO, SEM_EMPTY); 
    if(info.semaphore == (void *)ERROR)
    {
	perror("Cannot create semaphore");
	exit(0); //Fatal error
    }
    //100KBytes stack size
    int status = taskSpawn(NULL, 10, VX_FP_TASK, 100000, (FUNCPTR)handleEvents, (int)&info,0,0,0,0,0,0,0,0,0);
    if(status == ERROR)
    {
	perror("Cannot create task");
	exit(0); //Fatal error
    }
}



void Notifier::notify()
{
	semGive(info.semaphore);
}    
    
void Notifier::dispose()
{
    info.killed = 1;
    notify();
}    



#else

extern "C" void handleEvents(ThreadInfo *info)
{
	while(1)
	{
		sem_wait(&info->semaphore);
		if(info->killed) 
		{
			return;
		}
		info->callback(info->nid);
	}
}


void Notifier::initialize(int nid, void (*callback)(int))
{
    info.nid = nid;
    info.killed = 0;
    info.callback = callback;
    int sts = sem_init(&info.semaphore, 1, 0);
    if(sts == -1)
    {
	perror("Cannot create semaphore");
	exit(0); //Fatal error
    }
    pthread_t thread;
    int rc = pthread_create(&thread, NULL,(void *(*)(void *))handleEvents, (void *)&info); 
}

void Notifier::notify()
{
	sem_post(&info.semaphore);
}    
    
void Notifier::dispose()
{
    info.killed = 1;
    notify();
}    

#endif
#endif
