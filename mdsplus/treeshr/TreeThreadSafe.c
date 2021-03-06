#ifndef HAVE_VXWORKS_H
#include <config.h>
#endif
#include <libroutines.h>
#include <STATICdef.h>
#include "treethreadsafe.h"
#include <stdlib.h>
#include <mdsshr.h>
#include <strroutines.h>
STATIC_THREADSAFE  pthread_mutex_t mutex;
/* Key for the thread-specific buffer */
STATIC_THREADSAFE pthread_key_t buffer_key;

/* Once-only initialisation of the key */
STATIC_THREADSAFE pthread_once_t buffer_key_once = PTHREAD_ONCE_INIT;

STATIC_ROUTINE void buffer_key_alloc();

/* Return the thread-specific buffer */
TreeThreadStatic *TreeGetThreadStatic()
{
  TreeThreadStatic *p;
  pthread_once(&buffer_key_once, buffer_key_alloc);
  p = (TreeThreadStatic *) pthread_getspecific(buffer_key);
  if (p == NULL)
  {
    p = (TreeThreadStatic *)memset(malloc(sizeof(TreeThreadStatic)),0,sizeof(TreeThreadStatic));
    pthread_setspecific(buffer_key,(void *)p);
  }
  return p;
}

void *DBID=0;

void **TreeCtx() {
  TreeThreadStatic *p=TreeGetThreadStatic();
  return (p->privateCtx == 1) ? &p->DBID : &DBID;
}

int TreeUsePrivateCtx(int onoff) {
  TreeThreadStatic *p=TreeGetThreadStatic();
  int old=p->privateCtx;
  p->privateCtx=onoff;
  return old;
}

int TreeUsingPrivateCtx() {
  TreeThreadStatic *p=TreeGetThreadStatic();
  return p->privateCtx;
}

/* Free the thread-specific buffer */
STATIC_ROUTINE void buffer_destroy(void * buf)
{
  if (buf != NULL)
  {
    TreeThreadStatic *ts = (TreeThreadStatic *)buf;
    //_TreeClose(&ts->DBID,0,0);
    free(buf);
  }
}

/* Allocate the key */
STATIC_ROUTINE void buffer_key_alloc()
{
   pthread_key_create(&buffer_key, buffer_destroy);
}

void LockTreeMutex(pthread_mutex_t *mutex,int *initialized)
{
  if(!*initialized)
  {
#ifndef HAVE_WINDOWS_H
#ifndef HAVE_VXWORKS_H
    pthread_mutexattr_t m_attr;
    pthread_mutexattr_init(&m_attr);
#if !defined(PTHREAD_MUTEX_RECURSIVE)
#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#endif
#ifndef xxxx__sun
#ifdef HAVE_PTHREAD_MUTEXATTR_SETKIND_NP
    pthread_mutexattr_setkind_np(&m_attr,PTHREAD_MUTEX_RECURSIVE);
#else
    pthread_mutexattr_settype(&m_attr,PTHREAD_MUTEX_RECURSIVE);
#endif
#endif
    pthread_mutex_init(mutex,&m_attr);
#else
    pthread_mutex_init(mutex);
#endif
#endif
    *initialized = 1;
  }
  pthread_mutex_lock(mutex);
}

void UnlockTreeMutex(pthread_mutex_t *mutex)
{
  pthread_mutex_unlock(mutex);
}


