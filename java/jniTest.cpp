//  Disclaimers
//  NO WARRANTY FOR SOURCE CODE
//  ALL SOURCE CODE ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include "JNITest.h"

JNIEXPORT jstring JNICALL Java_JNITest_nativeHelloFromC (JNIEnv *env, jobject obj)
{
	jstring ret = 0;

	ret = env->NewStringUTF("hello from c");

	return ret;
}
