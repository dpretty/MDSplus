/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class MDSplus_Event */

#ifndef _Included_MDSplus_Event
#define _Included_MDSplus_Event
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     MDSplus_Event
 * Method:    sendEvent
 * Signature: (Ljava/lang/String;[B)V
 */
JNIEXPORT void JNICALL Java_MDSplus_Event_sendEvent
  (JNIEnv *, jclass, jstring, jbyteArray);

/*
 * Class:     MDSplus_Event
 * Method:    registerEvent
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_MDSplus_Event_registerEvent
  (JNIEnv *, jobject, jstring);

/*
 * Class:     MDSplus_Event
 * Method:    unregisterEvent
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_MDSplus_Event_unregisterEvent
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
