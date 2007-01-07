#ifndef RELIABLE_CHANNEL
#define RELIABLE_CHANNEL
#include "ChannelAddress.h"
#include "ChannelListener.h"
#include "Runnable.h"
#include "LockManager.h"
#include "Thread.h"

#define MAX_ADDERSSES 512;
#define MAX_LISTENERS 64
#define MAX_TYPES 8

/*
struct HandlerArgs
{
	ChannelAddress *addr;
	char *buf;
	int bufLen;
	char type;
	int senderIdx;
	ChannelListener *chanListener;
};
*/

class CommunicationChannel
{
	ChannelListener *listeners[MAX_TYPES][MAX_LISTENERS];
	int numListeners[MAX_TYPES];
	char types[MAX_TYPES];
	int numTypes;
	LockManager lock;
protected:
	char thisIdx;
	bool checkAddress(ChannelAddress *address);
			
public:
	void messageReceived(ChannelAddress *addr, int senderIdx, char type, char *buf, int bufLen);
	CommunicationChannel(int idx);
	virtual bool connectSender(ChannelAddress *addr) = 0;
	virtual bool connectReceiver(ChannelAddress *addr) = 0;
	virtual bool sendMessage(ChannelAddress *addr, char *buf, int bufLen, char type) = 0;
	virtual char *receiveMessage(ChannelAddress *addr, int *retLen, char *retType) = 0;
	void attachListener(ChannelListener *listener, char type);
	virtual unsigned int fromNative(int n) = 0;
	virtual int toNative(unsigned int l) = 0;
	void run(void *arg);
};
#endif
