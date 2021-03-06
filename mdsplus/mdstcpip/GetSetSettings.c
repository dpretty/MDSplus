#include "mdsip_connections.h"

#ifdef HAVE_WINDOWS_H
#define DEFAULT_HOSTFILE "C:\\MDSIP.HOSTS"
#else
#define DEFAULT_HOSTFILE "/etc/mdsip.hosts"
#endif

static unsigned char multi = 0;
static int ContextSwitching = 0;
static int MaxCompressionLevel = 9;
static int CompressionLevel = 0;
static char *Portname = 0;
static char *protocol="tcp";
static char *hostfile = 0;
static int flags = 0;
static int socketHandle = 0;

int GetSocketHandle() {
  return socketHandle;
}

int SetSocketHandle(int handle) {
  int old=socketHandle;
  socketHandle=handle;
  return old;
}

int GetFlags() {
  return flags;
}

int SetFlags(f) {
  int old=flags;
  flags=f;
  return old;
}

char *GetProtocol() {
  return protocol;
}

char *SetProtocol(char *p) {
  char *old=protocol;
  protocol=p;
  return old;
}

char *GetPortname() {
  return Portname;
}

char *MdsGetServerPortname() {
  return Portname;
}

char *SetPortname(char *p) {
  char *old=Portname;
  Portname=p;
  return old;
}


char *GetHostfile() {
  return hostfile ? hostfile : DEFAULT_HOSTFILE;
}

char *SetHostfile(char *newhostfile) {
  char *old=hostfile;
  hostfile=newhostfile;
  return old;
}

unsigned char GetMulti() {
  return multi;
}

unsigned char SetMulti(unsigned char s) {
  unsigned char old_multi=multi;
  multi=s;
  return old_multi;
}

int GetContextSwitching() {
  return ContextSwitching;
}

int SetContextSwitching(int s) {
  int old_ctx_switching=ContextSwitching;
  ContextSwitching=s;
  return old_ctx_switching;
}

int GetMaxCompressionLevel() {
  return MaxCompressionLevel;
}

int SetMaxCompressionLevel(int s) {
  int old_max_compression=MaxCompressionLevel;
  MaxCompressionLevel=s;
  return old_max_compression;
}

int SetCompressionLevel(int level) {
  int old_level = CompressionLevel;
  CompressionLevel = level;
  return old_level;
}

int GetCompressionLevel() {
  return CompressionLevel;
}

static int connect_timeout = 0;

int SetMdsConnectTimeout(int sec) {
  int old = connect_timeout;
  connect_timeout = sec;
  return old;
}
int GetMdsConnectTimeout() {
  return connect_timeout;
}

static int ClientAddr=0;
int MdsGetClientAddr() {
  return ClientAddr;
}
void MdsSetClientAddr(int addr) {
  ClientAddr=addr;
}
