#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include "importgl.h"
#include "app.h"
#include "input.h"

static int sWindowWidth = 320;
static int sWindowHeight = 480;

static JNIEnv *g_env;
static jobject g_pngmgr;

void Java_ru_footmade_keplerc_MainActivity_nativeAccelerometerChanged(JNIEnv* env, jobject thiz, jfloat x, jfloat y, jfloat z) {
	setAccelerometerValues(x, y, z);
}

/* Call to initialize the graphics state */
void Java_ru_footmade_keplerc_MainRenderer_nativeInit(JNIEnv* env, jobject thiz, jobject pngmgr) {
	g_env = env;
	g_pngmgr = (*g_env)->NewGlobalRef(g_env, pngmgr);
	importGLInit();
	appInit();
}

void Java_ru_footmade_keplerc_MainRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h) {
	sWindowWidth = w;
	sWindowHeight = h;
	appResize(w, h);
}

/* Call to finalize the graphics state */
void Java_ru_footmade_keplerc_MainRenderer_nativeDone(JNIEnv* env) {
	(*g_env)->DeleteGlobalRef(env, g_pngmgr);
	appDeinit();
	importGLDeinit();
}

void Java_ru_footmade_keplerc_MainGLSurfaceView_nativeTouchDown(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y) {
	touchScreen(x, sWindowHeight - y);
}

void Java_ru_footmade_keplerc_MainGLSurfaceView_nativePause(JNIEnv* env) {
}

void Java_ru_footmade_keplerc_MainGLSurfaceView_nativeResume(JNIEnv* env) {
}

static long _getTime(void) {
	struct timeval now;

	gettimeofday(&now, NULL);
	return (long) (now.tv_sec * 1000 + now.tv_usec / 1000);
}

long tick = -1;
/* Call to render the next GL frame */
void Java_ru_footmade_keplerc_MainRenderer_nativeRender(JNIEnv* env) {
	if (tick == -1)
		tick = _getTime();
	long curTime = _getTime();
	float interval = ((float) (curTime - tick)) / 1000;
	tick = curTime;
	appRender(interval, sWindowWidth, sWindowHeight);
}

TextureInfo loadTexture(char *path) {
	jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);
	jmethodID mid = (*g_env)->GetMethodID(g_env, cls, "loadTexture", "(Ljava/lang/String;)Lru/footmade/keplerc/TextureInfo;");
	jstring name = (*g_env)->NewStringUTF(g_env, path);
	jobject textureInfo = (*g_env)->CallObjectMethod(g_env, g_pngmgr, mid, name);
	jclass textureInfoClass = (*g_env)->GetObjectClass(g_env, textureInfo);
	jfieldID textureId = (*g_env)->GetFieldID(g_env, textureInfoClass, "textureId", "I");
	jfieldID width = (*g_env)->GetFieldID(g_env, textureInfoClass, "width", "I");
	jfieldID height = (*g_env)->GetFieldID(g_env, textureInfoClass, "height", "I");
	TextureInfo result;
	result.textureId = (*g_env)->GetIntField(g_env, textureInfo, textureId);
	result.width = (*g_env)->GetIntField(g_env, textureInfo, width);
	result.height = (*g_env)->GetIntField(g_env, textureInfo, height);
	return result;
}
