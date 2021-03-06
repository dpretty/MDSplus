#include "SharedMemManager.h"

#include "NetworkManager.h"
#include "TCPNetworkManager.h"
#include "UDPNetworkManager.h"
#include "Lock.h"
#include "UnnamedSemaphore.h"
#include "EventManager.h"
#include "Runnable.h"
#include "Thread.h"
#include "Delay.h"
#include "Event.h"
#include <stdio.h>
#define TCP_PORT 4000 
#define UDP_PORT 4001 
#define STATE_PORT 4002 

class ExternalEventManager;

static ExternalEventManager *extEventManager;
static NetworkAddress *extAddresses[512];
static NetworkAddress *udpExtAddresses[512];
static NetworkManager *msgManager;
static NetworkManager *udpMsgManager;

///State management
static NetworkManager *stateMsgManager;
SharedMemManager eventMemManager;


static int numExtAddresses;

static IPAddress *getMulticastAddr(char *eventName)
{
	int len = strlen(eventName);
	unsigned int hash = 0;
	for(int i = 0; i < len; i++)
		hash += eventName[i];
	char multicastAddrStr[64];
	sprintf(multicastAddrStr, "225.0.0.%d", hash%256);
	return new IPAddress(multicastAddrStr, UDP_PORT);
}




static void eventCallback(char *name, char *buf, int bufSize, void *arg, bool isSynch, int retSize, char *retData, int type);

//Class InternalPending describes an event received from outside (and therefore for which an 
//internal listener registers) for which another
//node registered. In this case, EventConnector has registered as listener for this event
//but its associated callback has not to send the event to the other node (it will receive it directly
//from the event originator).
class InternalPending
{
public:
	char *buf;
	int size;
	InternalPending *nxt, *prv;
	InternalPending(){nxt = prv = 0;}
	InternalPending(char *inBuf, int inSize)
	{
		size = inSize;
		buf = new char[inSize];
		memcpy(buf, inBuf, inSize);
	}

	bool corresponds(char *inBuf, int inSize)
	{
		if(size != inSize) return false;
		for(int i = 0; i < size; i++)
			if(buf[i] != inBuf[i])
				return false;
		return true;
	}
	~InternalPending()
	{
		if(size > 0)
			delete [] buf;
	}
};


//Class External pending describes the propagation to another node of  a synchronous event. 
//When the message acknowledge will be received (indicating that the remote client processed the event)
//the receiver thread will post the associate semaphore (using the unique id), thus letting the event
//callback terminate.

class ExternalPending
{
public:
	unsigned int id;
	UnnamedSemaphore sem;
	int bufSize;
	char *buf;
	NetworkAddress *addr;
	ExternalPending *nxt, *prv;
	ExternalPending(int id, NetworkAddress *addr)
	{
		this->id = id;
		this->addr = addr;
		buf = 0;
		bufSize = 0;
		nxt = prv = 0;
		sem.initialize(0);
	}
	~ExternalPending()
	{
		if(bufSize > 0)
			delete [] buf;
		sem.dispose();
	}
};

//Class ExternalListener describes the remote listener for an event. 
class ExternalListener
{
public:
	NetworkAddress *addr;
	ExternalListener *nxt, *prv;
	ExternalListener(NetworkAddress *addr)
	{
		this->addr = addr;
		nxt = prv = 0;
	}
};


#define IS_SYNCH_EVENT 1
#define IS_ASYNCH_EVENT 2
#define IS_EVENT_REGISTRATION 3
#define IS_EVENT_ACK 4
#define IS_SYNCH_COLLECT_EVENT 5
#define IS_EVENT_COLLECT_ACK 6
//Class Event Message describes the event message sent over the network
class EventMessage
{
public:
	char mode;
	char *name;
	unsigned int waitId;
	char *buf;
	int bufSize;
	int retSize;
	EventAnswer *evAnsw;
	
	EventMessage(char *name, char *buf, int bufSize, bool isCollect, bool synch, unsigned int waitId)
	{
		if(synch)
		{
			if(isCollect)
				mode = IS_SYNCH_COLLECT_EVENT;
			else
				mode = IS_SYNCH_EVENT;
		}
		else
			mode = IS_ASYNCH_EVENT;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->buf = buf; 
		this->bufSize = bufSize;
		this->waitId = waitId;
	}
	
	EventMessage(char *name, int waitId)
	{
		mode = IS_EVENT_ACK;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->waitId = waitId;
	}
	
	EventMessage(char *name, int waitId, EventAnswer *evAnsw)
	{
		mode = IS_EVENT_COLLECT_ACK;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->waitId = waitId;
		this->evAnsw = evAnsw;
	}
	
	EventMessage(char *buf)
	{
		mode = IS_EVENT_REGISTRATION;
		this->name = new char[strlen(buf) + 1];
		sscanf(buf, "%s %d", this->name, &this->retSize);
	}
	
	EventMessage(char *inBuf, int inBufSize, NetworkManager *msgManager)
	{
		retSize = 0;
		char *ptr = inBuf;
		mode = *ptr;
		ptr++;
		int nameLen = msgManager->toNative(*(unsigned int *)ptr);
		name = new char[nameLen+1];
		ptr += 4;
		memcpy(name, ptr, nameLen);
		name[nameLen] = 0;
		ptr += nameLen;
		switch(mode) {
			case IS_ASYNCH_EVENT:
				bufSize = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				buf = ptr; //Note that buffer is not copied
				ptr += bufSize;
				waitId = 0;
				break;
			case IS_SYNCH_EVENT:
			case IS_SYNCH_COLLECT_EVENT:
				waitId = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				bufSize = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				buf = ptr; //Note that buffer is not copied
				ptr += bufSize;
				break;
			case IS_EVENT_ACK:
				bufSize = 0;
				buf = 0;
				waitId = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				break;
			case IS_EVENT_COLLECT_ACK:
				bufSize = 0;
				buf = 0;
				waitId = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				bufSize = msgManager->toNative(*(unsigned int *)ptr);
				ptr += 4;
				buf = ptr;//Note that buffer is not copied
				ptr += bufSize;
				break;
			case IS_EVENT_REGISTRATION:
				retSize = msgManager->toNative(*(unsigned int *)ptr);
				bufSize = 0;
				buf = 0;
				waitId = 0;
				ptr += 4;
				break;
		}
		if(ptr - inBuf != inBufSize)
			printf("INTERNAL ERROR: WRONG MESSAGE SIZE IN DISTRIBUTED EVENTS");
	}
	
	char *serialize(int &outSize, NetworkManager *msgManager)
	{
		int size;
		int retMsgSize = 0;
		switch(mode) 
		{
			case IS_ASYNCH_EVENT:
				size = 1 /* mode*/ + strlen(name) + 4/*name and name length*/+ bufSize + 4 /*buf and length */;
				break;
			case IS_SYNCH_EVENT:
			case IS_SYNCH_COLLECT_EVENT:
				size = 1 /* mode*/ + strlen(name) + 4/*name and name length*/+ 
					bufSize + 4 /*buf and length */ + 4 /*waitId*/;
				break;
			case IS_EVENT_ACK:
				size = 1 /* mode*/ + strlen(name) + 4/*name and name length*/+ 4 /*waitId*/;
				break;
			case IS_EVENT_REGISTRATION:
				size = 1 /* mode*/ + strlen(name) + 4/*name and name length*/ + 4 /*ret size*/;
				break;
			case IS_EVENT_COLLECT_ACK:
				retMsgSize = 0;
				for(int i = 0; i < evAnsw->getNumMsg(); i++)
					retMsgSize+= evAnsw->getMsgSizeAt(i);
				size = 1 /* mode*/ + strlen(name) + 4/*name and name length*/+ 4 /*waitId*/ 
					+ 4 + retMsgSize /*retMessage and retMessage len*/;
				break;

		}

		char *retBuf = new char[size];
		char *ptr = retBuf;
		*ptr = mode;
		ptr++;
		int nameLen = strlen(name);
		*((unsigned int *)ptr) = msgManager->fromNative(nameLen);
		ptr += 4;
		memcpy(ptr, name, nameLen);
		ptr += nameLen;
		switch (mode)
		{
			case IS_ASYNCH_EVENT:
				*((unsigned int *)ptr) = msgManager->fromNative(bufSize);
				ptr += 4;
				memcpy(ptr, buf, bufSize);
				break;
			case IS_SYNCH_EVENT:
			case IS_SYNCH_COLLECT_EVENT:
				*((unsigned int *)ptr) = msgManager->fromNative(waitId);
				ptr += 4;
				*((unsigned int *)ptr) = msgManager->fromNative(bufSize);
				ptr += 4;
				memcpy(ptr, buf, bufSize);
				ptr += retMsgSize;
				break;
			case IS_EVENT_ACK:
				*((unsigned int *)ptr) = msgManager->fromNative(waitId);
				break;
			case IS_EVENT_REGISTRATION:
				*((unsigned int *)ptr) = msgManager->fromNative(retSize);
				break;
			case IS_EVENT_COLLECT_ACK:
				*((unsigned int *)ptr) = msgManager->fromNative(waitId);
				ptr +=4;
				*((unsigned int *)ptr) = msgManager->fromNative(retMsgSize);
				ptr += 4;
				int currSize;
				for(int i = 0; i < evAnsw->getNumMsg(); i++)
				{
					char *currMsg = evAnsw->getMsgAt(i, currSize);
					memcpy(ptr, currMsg, currSize);
					ptr += currSize;
				}
				break;

		}
		outSize = size;
		return retBuf;
	}
	
	~EventMessage()
	{
		delete [] name;
		//Buffer is not allocated
	}
};


static int extPendingId = 0;

//Class External event is the root of all the data structures required to describe an event
//for which some external node registered
class ExternalEvent
{
public:
	char *eventName;
	void *listenerAddr;
	int retSize;
	int totRetSize;
	InternalPending *intPendingHead;
	ExternalPending *extPendingHead;
	ExternalListener *extListenerHead;
	ExternalListener *extMulticastListener;
	ExternalEvent *nxt, *prv;
	
	ExternalEvent(const char *name, int retSize)
	{
		this->retSize = this->totRetSize = retSize;
		eventName = new char[strlen(name) + 1];
		strcpy(eventName, name);
		intPendingHead = NULL;
		extPendingHead = NULL;
		extListenerHead = NULL;
		extMulticastListener = NULL;
		nxt = prv = NULL;
	}
	//Check if the callback has been triggered by the reception of an external event.
	//in this case it is only required to discard the corresponding InternalPending instance
	bool isExternalEvent(char *buf, int size)
	{
		InternalPending *currPend = intPendingHead;
		while(currPend)
		{
			if(currPend->corresponds(buf, size))
			{
				if(currPend == intPendingHead)
					intPendingHead = currPend->nxt;
				else
					currPend->prv->nxt = currPend->nxt;
				if(currPend->nxt)
					currPend->nxt->prv = currPend->prv;
				delete currPend;
				return true;
			}
			currPend = currPend->nxt;
		}
		return false;
	}

//Add a new InternalPending structure to record the receipt of an external event for which
//someone registered remotely
	void addInternalPending(char *buf, int bufSize)
	{
		InternalPending *newIntPending = new InternalPending(buf, bufSize);
		newIntPending->nxt = intPendingHead;
		if(intPendingHead)
			intPendingHead->prv = newIntPending;
		intPendingHead = newIntPending;
	}

//Handle the receipt of a network message indicating the termination of a synchronous trigger
//the message will bring the unique long id of ExternalPending instance
	void signalExternalTermination(unsigned int id, int bufSize, char *buf)
	{
		ExternalPending *currPend = extPendingHead;
		while(currPend)
		{
			if(currPend->id == id)
			{
				currPend->bufSize = bufSize;
				if(bufSize > 0)
				{
					currPend->buf = new char[bufSize];
					memcpy(currPend->buf, buf, bufSize);
				}
				currPend->sem.post();
				break;
			}
			currPend = currPend->nxt;
		}
	}

	
//SendEvent sends a message describing the event. 
	void sendSynchEvent(char *buf, int bufSize, bool isCollect, NetworkManager *msgManager, 
			UnnamedSemaphore ** &retSems, unsigned int * &waitIds, int &numSems)
	{
		//count listeners
		int numListeners = 0;
		ExternalListener *currListener = extListenerHead;
		while(currListener)
		{
			numListeners++;
			currListener = currListener->nxt;
		}
		retSems = new UnnamedSemaphore *[numListeners];
		waitIds = new unsigned int[numListeners];
		currListener = extListenerHead;
		int semIdx = 0;
		while(currListener)
		{
			addExternalPending(currListener->addr, retSems[semIdx], waitIds[semIdx]);
			EventMessage msg(eventName, buf, bufSize, isCollect, true, waitIds[semIdx]);
			int msgLen;
			char *msgBuf = msg.serialize(msgLen, msgManager);
			msgManager->sendMessage(currListener->addr, msgBuf, msgLen);
			delete [] msgBuf;
			currListener = currListener->nxt;
			semIdx++;
		}
		numSems = semIdx;
	}
	
	//SendEvent sends a message describing the event. 
	void sendAsynchEvent(char *buf, int bufSize, NetworkManager *msgManager, bool isUdp)
	{
		int msgLen;
		EventMessage msg(eventName, buf, bufSize, false, false, 0);
		char *msgBuf = msg.serialize(msgLen, msgManager);
		if(isUdp)
		{
			printf("UDP\n");
			ExternalListener *multicastListener = extMulticastListener;
			if(!multicastListener)
			{
				printf("INTERNAL ERROR: Missing Multicast Address\n");
				return;
			}
			IPAddress currAddr((IPAddress *)extMulticastListener->addr);
			msgManager->sendMessage(&currAddr,  msgBuf, msgLen);
		}
		else
		{
			ExternalListener *currListener = extListenerHead;
			while(currListener)
			{
				IPAddress currAddr((IPAddress *)currListener->addr);
/*				if(isUdp)
					currAddr.setPort(UDP_PORT);
				else
*/					currAddr.setPort(TCP_PORT);

				msgManager->sendMessage(&currAddr, msgBuf, msgLen);
				currListener = currListener->nxt;
			}
		}
		delete [] msgBuf;
	}
	
	void addExternalPending(NetworkAddress *addr, UnnamedSemaphore * &retSem, unsigned int &retId)
	{
		retId = extPendingId++;
		ExternalPending *currPending = new ExternalPending(retId, addr);
		currPending->nxt = extPendingHead;
		extPendingHead = currPending;
		retSem = &currPending->sem;
	}
	void removeExternalPending(unsigned int id)
	{
		ExternalPending *currPend = extPendingHead;
		while(currPend)
		{
			if(currPend->id == id)
			{
				if(currPend == extPendingHead)
					extPendingHead = currPend->nxt;
				else
				{
					if(currPend->prv)
						currPend->prv->nxt = currPend->nxt;
				}
				if(currPend->nxt)
					currPend->nxt->prv = currPend->prv;
				delete currPend;
				return;
			}
			currPend = currPend->nxt;
		}
	}
	
	void collectExternalPendingData(unsigned int id, int &retSize, char * &retBuf)
	{
		retSize = 0;
		retBuf = 0;
		ExternalPending *currPend = extPendingHead;
		while(currPend)
		{
			if(currPend->id == id)
			{
				retSize = currPend->bufSize;
				retBuf = currPend->buf;
				return;
			}
			currPend = currPend->nxt;
		}
	}
		
	void unblockExternalPending(NetworkAddress *addr)
	{
		ExternalPending *currPend = extPendingHead;
		while(currPend)
		{
			if(currPend->addr->equals(addr))
				currPend->sem.post();
			currPend = currPend->nxt;
		}
	}
	void addExternalListener(NetworkAddress *addr)
	{
		bool exists = false;
		ExternalListener *currListener = extListenerHead;
		while(!exists && currListener)
		{
			exists = currListener->addr->equals(addr);
			currListener = currListener->nxt;
		}
		if(exists)
			return;
		ExternalListener *newListener = new ExternalListener(addr);
		newListener->nxt = extListenerHead;
		if(extListenerHead)
			extListenerHead->prv = newListener;
		extListenerHead = newListener;

		if(!extMulticastListener)
		{
			extMulticastListener = new ExternalListener(getMulticastAddr(eventName));
		}

	}
	void removeExternalListeners(NetworkAddress *addr)
	{
		ExternalListener *currListener = extListenerHead;
		while(currListener)
		{
			if(currListener->addr->equals(addr))
			{
				if(currListener == extListenerHead)
					extListenerHead = currListener->nxt;
				else
					currListener->prv->nxt = currListener->nxt;
				if(currListener->nxt)
					currListener->nxt->prv = currListener->prv;
				delete currListener;
				return;
			}
			currListener = currListener->nxt;
		}
	}

};

//Top structure class
class ExternalEventManager
{
public:
	Lock lock;
	ExternalEvent *extEventHead;
	
	ExternalEventManager()
	{
		lock.initialize();
		extEventHead = 0;
	}
	~ExternalEventManager()
	{
		lock.dispose();
	}
	
	ExternalEvent *findEvent(const char *name)
	{
		ExternalEvent *currEvent = extEventHead;
		while(currEvent)
		{
			if(!strcmp(name, currEvent->eventName))
				break;
			currEvent = currEvent->nxt;
		}
		return currEvent;
	}
	
	ExternalEvent *newEvent(const char *name, int retSize)
	{
		ExternalEvent *newEv = new ExternalEvent(name, retSize);
		newEv->nxt = extEventHead;
		if(extEventHead)
			extEventHead->prv = newEv;
		extEventHead = newEv;
		return newEv;
	}
	
	bool isExternalEvent(const char *name, char *buf, int size)
	{
		bool isExt = false;
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNAL ERROR: received event with no event structure!!");
		}
		else
			isExt = extEvent->isExternalEvent(buf, size);
		lock.unlock();
		return  isExt;
	}

	void signalExternalTermination(const char *name, unsigned int id, int bufSize, char *buf)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNAL ERROR: received event message with no event structure!!");
		}
		else
			extEvent->signalExternalTermination(id, bufSize, buf);
		lock.unlock();
	}
	void sendAsynchEvent(const char *name, char *buf, int bufSize, NetworkManager *msgManager, bool isUdp)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNAL ERROR: received event message with no event structure!!");
		}
		else
			extEvent->sendAsynchEvent(buf, bufSize, msgManager, isUdp);
		lock.unlock();
	}
	void sendSynchEvent(const char *name, char *buf, int bufSize, bool isCollect, NetworkManager *msgManager, 
			UnnamedSemaphore ** &retSems, unsigned int * &waitIds, int &numSems)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNAL ERROR: received event message with no event structure!!");
		}
		else
			extEvent->sendSynchEvent(buf, bufSize, isCollect, msgManager, retSems, waitIds, numSems);
		lock.unlock();
	}
	void removeExternalPending(const char *name, unsigned int id)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNALE ERROR: received event message with no event structure!!");
		}
		else
			extEvent->removeExternalPending(id);
		lock.unlock();
		
	}
	void collectExternalPendingData(const char *name, unsigned int i, int &retSize, char * &retBuf)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			printf("INTERNALE ERROR: received event message with no event structure!!");
		}
		else
			extEvent->collectExternalPendingData(i, retSize, retBuf);
		lock.unlock();
		
	}

	void addInternalPending(const char *name, char *buf, int bufSize)
	{
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(extEvent)
			extEvent->addInternalPending(buf, bufSize);
		lock.unlock();
	}	


	void addExternalListener(const char *name, int retSize, NetworkAddress *addr)
	{
		Event ev;
		bool isNewEvent = false;
		lock.lock();
		ExternalEvent *extEvent = findEvent(name);
		if(!extEvent)
		{
			extEvent = newEvent(name, retSize);
			isNewEvent = true;
		}
		extEvent->addExternalListener(addr);
		if(isNewEvent)
			extEvent->listenerAddr = ev.addListener(name, eventCallback, false, retSize);
		else
		{
			int prevSize = extEvent->retSize;

//If ret size larger than previous (singleEvent) retSize, make enough room
			if(prevSize < retSize)
			{
				extEvent->totRetSize = (extEvent->totRetSize/extEvent->retSize)*retSize;
				extEvent->totRetSize = retSize;
			}
			else
				extEvent->totRetSize += extEvent->retSize;
			ev.resizeListener(extEvent->listenerAddr, extEvent->totRetSize);
		}
		lock.unlock();
	}
	//Called upon connection termination
	void removeExternalListeners(NetworkAddress *addr)
	{
		lock.lock();
		ExternalEvent *extEvent = extEventHead;
		while(extEvent)
		{
			extEvent->removeExternalListeners(addr);
			extEvent = extEvent->nxt;
		}
		lock.unlock();
	}
	void unblockExternalPending(NetworkAddress *addr)
	{
		lock.lock();
		ExternalEvent *extEvent = extEventHead;
		while(extEvent)
		{
			extEvent->unblockExternalPending(addr);
			extEvent = extEvent->nxt;
		}
		lock.unlock();
	}

};

//Class TrigWaitRunnable provides trigger and wait for a given event on a separate thread
class TrigWaitRunnable: public Runnable
{
public:
	NetworkManager *msgManager;
	NetworkAddress *addr;
	char *name;
	char *buf;
	int bufSize;
	bool isCollect;
	unsigned int waitId;
	
	TrigWaitRunnable(char *name,char *buf, int bufSize,  unsigned int waitId, bool isCollect, NetworkManager *msgManager, NetworkAddress *addr)
	{
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->buf = new char[bufSize];
		memcpy(this->buf, buf, bufSize);
		this->bufSize = bufSize;
		this->isCollect = isCollect;
		this->msgManager = msgManager;
		this->addr = addr;
		this->waitId = waitId;
	}
	virtual void dispose()
	{
		printf("ECCO LA DISPOSE\n");
		delete []name;
		delete[] buf;
	}
	virtual void run(void *arg)
	{
		Event ev;
		if(isCollect)
		{
			EventAnswer *answ = ev.triggerAndCollect(name, buf, bufSize);
			EventMessage msg(name, waitId, answ);
			int msgLen;
			char *msgBuf = msg.serialize(msgLen, msgManager);
			msgManager->sendMessage(addr, msgBuf, msgLen);
			delete []msgBuf;
			delete answ;
			Thread *thread = (Thread *)arg;
			delete thread;
		}
		else
		{
			ev.triggerAndWait(name, buf, bufSize);
			EventMessage msg(name, waitId);
			int msgLen;
			char *msgBuf = msg.serialize(msgLen, msgManager);
			msgManager->sendMessage(addr, msgBuf, msgLen);
			delete []msgBuf;
			Thread *thread = (Thread *)arg;
			delete thread;
		}
	}
};


//Class EventMessageReceiver handles the reception of event messages over the network
class EventMessageReceiver:public NetworkReceiver
{
public:
	NetworkManager *msgManager;
	ExternalEventManager *extEventManager;
	EventMessageReceiver(ExternalEventManager *extEventManager, NetworkManager *msgManager)
	{
		this->extEventManager = extEventManager;
		this->msgManager = msgManager;
	}
	
	virtual void messageReceived(NetworkAddress *addr, char *buf, int size)
	{
		
		Thread *thread;
		EventMessage evMsg(buf, size, msgManager);
		switch(evMsg.mode)
		{
			case IS_ASYNCH_EVENT:
				printf("Received IS_ASYNCH_EVENT %s\n", evMsg.name);
				extEventManager->addInternalPending(evMsg.name, evMsg.buf, evMsg.bufSize);
				EventTrigger(evMsg.name, evMsg.buf, evMsg.bufSize);
				break;
			case IS_SYNCH_EVENT:
				printf("Received IS_SYNCH_EVENT %s\n", evMsg.name);
				extEventManager->addInternalPending(evMsg.name, evMsg.buf, evMsg.bufSize);
				thread = new Thread();
				thread->start((Runnable *)new TrigWaitRunnable(evMsg.name, evMsg.buf, evMsg.bufSize, evMsg.waitId, false, msgManager, addr), thread);
				break;
			case IS_SYNCH_COLLECT_EVENT:
				printf("Received IS_SYNCH_COLLECT_EVENT %s\n", evMsg.name);
				extEventManager->addInternalPending(evMsg.name, evMsg.buf, evMsg.bufSize);
				thread = new Thread();
				thread->start((Runnable *)new TrigWaitRunnable(evMsg.name, evMsg.buf, evMsg.bufSize, evMsg.waitId, true, msgManager, addr), thread);
				break;
			case IS_EVENT_ACK:
				printf("Received IS_EVENT_ACK %s\n", evMsg.name);
				extEventManager->signalExternalTermination(evMsg.name, evMsg.waitId, 0, 0);
				break;
			case IS_EVENT_COLLECT_ACK:
				printf("Received IS_EVENT_COLLECT_ACK %s\n", evMsg.name);
				extEventManager->signalExternalTermination(evMsg.name, evMsg.waitId, evMsg.bufSize, evMsg.buf);
				break;
			case IS_EVENT_REGISTRATION:
				printf("Received IS_EVENT_REGISTRATION %s\n", evMsg.name);
				extEventManager->addExternalListener(evMsg.name, evMsg.retSize, addr);
				break;
		}
	}
	virtual void connectionTerminated(NetworkAddress *addr)
	{
		extEventManager->unblockExternalPending(addr);
		extEventManager->removeExternalListeners(addr);
	}

};




static void eventCallback(char *name, char *buf, int bufSize, void *arg, bool isSynch, int retSize, char *retData, int type)
{
//printf("EVENT CALLBACK %s %d\n", name, type);

//The Event has been triggered as response from an external message.
	if(extEventManager->isExternalEvent(name, buf, bufSize))
		return;

	if(isSynch)
	{
		int i;
		int numSems;
		UnnamedSemaphore **sems;
		unsigned int *waitIds;
		extEventManager->sendSynchEvent(name, buf, bufSize, (retSize > 0), msgManager, sems, waitIds, numSems);

		int *retSizes = new int[numSems];
		char **retBufs = new char *[numSems];

		for(i = 0; i < numSems; i++)
		{
			sems[i]->wait();
			extEventManager->collectExternalPendingData(name, waitIds[i], retSizes[i], retBufs[i]);
		}
		//All external data  collected
		int totRetSize = 0;
		for(i = 0; i < numSems; i++)
			totRetSize += retSizes[i];

		if(totRetSize != retSize)
			printf("UNEXPECTED RET EVENT SIZE: Expected %d, Actual: %d\n", retSize, totRetSize);

		int currSize = 0;
		for(i = 0; i < numSems; i++)
		{
			if(currSize + retSizes[i] <= retSize)
				memcpy(&retData[currSize], retBufs[i], retSizes[i]);
			else
				break;
			currSize += retSizes[i];
			//delete [] retBufs[i];
		}

		//Deallocate ExternalPendings
		for(i = 0; i < numSems; i++)
		{
			extEventManager->removeExternalPending(name, waitIds[i]);
		}
		delete []sems;
		delete [] waitIds;
		delete [] retSizes;
		delete [] retBufs;

	}
	else
	{
		if(type)
			extEventManager->sendAsynchEvent(name, buf, bufSize, udpMsgManager, true);
		else
			extEventManager->sendAsynchEvent(name, buf, bufSize, msgManager, false);
	}
}

static void registerEventCallback(char *name, char *buf, int bufSize, void *arg, bool isSynch, int retSize, char *retData, int type)
{
printf("REGISTER EVENT CALLBACK %s %s\n", name, buf);

	EventMessage *evMessage = new EventMessage(buf);
	int msgLen;
	char *msg = evMessage->serialize(msgLen, msgManager); 
	for(int i = 0; i < numExtAddresses; i++)
	{
		try {
			msgManager->sendMessage(extAddresses[i], msg, msgLen);
		}catch(SystemException *exc)
		{
			printf("Error Sending registration message: %s\n", exc->what());
		}
	}
	((UDPNetworkManager*)udpMsgManager)->join(getMulticastAddr(evMessage->name));
	delete [] msg;
	delete evMessage;
}

static void trim(char *line)
{
	int i;
	for(i = strlen(line) - 1; i >= 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n'); i--);
	line[i+1] = 0;
}
	
static void readExtAddresses(char *fileName)
{
	FILE *confFile = fopen(fileName, "r");
	if(!confFile)
	{
		printf("Cannot open configuration file\n");
		exit(0);
	}
	char line[512];
	char *s = fgets(line, 512, confFile);
	trim(line);
	if(!strcmp(line, "TCP") || !strcmp(line, "tcp"))
	{

		int addrIdx = 0;
		while(fgets(line, 512, confFile))
		{
			trim(line);
			if(strlen(line) == 0)
				continue;
			printf("%s\n", line);
			udpExtAddresses[addrIdx] = new IPAddress(line, UDP_PORT);
			extAddresses[addrIdx++] = new IPAddress(line, TCP_PORT);

		}
		numExtAddresses = addrIdx;
		msgManager = new TCPNetworkManager();
		udpMsgManager = new UDPNetworkManager();
	}
	//other Networks not yet supported
}

class OrphanChecker:public Runnable
{
public:
	void run(void *arg)
	{
		Delay delay(1000); //Wait 1 sec
		while(true)
		{
			EventClean();
			delay.wait();
		}
	}
};

#define GET_STATE 1
#define CLEAN_EVENTS 2
class StateMessageReceiver:public NetworkReceiver
{
public:
	void messageReceived(NetworkAddress *addr, char *buf, int size)
	{
		if(size != 1)
		{
			printf("Unexpected State Request message size: %d\n", size);
			return;
		}
		switch(buf[0]) {
		case GET_STATE: {
			SharedMemState *currState = eventMemManager.getState();
			int currSize;
			char *serState = currState->serialize(currSize);
			stateMsgManager->sendMessage(addr, serState, currSize);
			delete currState;
			delete [] serState;
			Event ev;
			EventState *evState = ev.getState();
			serState = evState->serialize(currSize);
			stateMsgManager->sendMessage(addr, serState, currSize);
			delete evState;
			delete []serState;
			break;}
		case CLEAN_EVENTS: {
			Event ev;
			ev.clean();
			break;}
		default:
			printf("Unexpected State Request message code: %d\n", buf[0]);
		}

	}
	void connectionTerminated(NetworkAddress *addr){}
};





		
		
int main(int argc, char *argv[])
{
	Event ev;
	//ev.reset();
	if(argc != 2)
	{
		printf("Usage: EventConnector <configuration file>\n");
		exit(0);
	}
	readExtAddresses(argv[1]);

	extEventManager = new ExternalEventManager();
	EventMessageReceiver messageReceiver(extEventManager, msgManager);

	msgManager->connectReceiver(new IPAddress(TCP_PORT), &messageReceiver);
	udpMsgManager->connectReceiver(new IPAddress(UDP_PORT), &messageReceiver);
	ev.addListener("@@@EVENT_MANAGER@@@",  registerEventCallback);

	Thread checkerThread;
	//checkerThread.start(new OrphanChecker, 0);

//Set-up state server
	StateMessageReceiver stateReceiver;
	eventMemManager.initialize(EVENT_ID, EVENT_SIZE);
	stateMsgManager = new TCPNetworkManager();
	stateMsgManager->connectReceiver(new IPAddress(STATE_PORT), &stateReceiver);


	UnnamedSemaphore sem;
	sem.initialize(0);
	sem.wait(); //Suspend forever
	return 0;
}
