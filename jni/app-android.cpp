#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include "importgl.h"
#include "app.h"
#include "input.h"
#include "string"

static int sWindowWidth = 320;
static int sWindowHeight = 480;

static JNIEnv *g_env;
static jobject g_pngmgr;

extern "C"
{
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainActivity_nativeAccelerometerChanged(JNIEnv* env, jobject thiz,
			jfloat x, jfloat y, jfloat z) {
		setAccelerometerValues(x, y, z);
	}

	/* Call to initialize the graphics state */
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeInit(JNIEnv* env, jobject thiz, jobject pngmgr) {
		g_env = env;
		g_pngmgr = g_env->NewGlobalRef(pngmgr);
		importGLInit();
		appInit();
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h) {
		sWindowWidth = w;
		sWindowHeight = h;
		appResize(w, h);
	}

	/* Call to finalize the graphics state */
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeDone(JNIEnv* env) {
		g_env->DeleteGlobalRef(g_pngmgr);
		appDeinit();
		importGLDeinit();
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeTouchDown(JNIEnv* env,
			jobject thiz, jfloat x, jfloat y) {
		touchDown(x, sWindowHeight - y);
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeTouchUp(JNIEnv* env,
			jobject thiz, jfloat x, jfloat y) {
		touchUp(x, sWindowHeight - y);
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativePause(JNIEnv* env) {
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeResume(JNIEnv* env) {
	}

	static long _getTime(void) {
		struct timeval now;

		gettimeofday(&now, NULL);
		return (long) (now.tv_sec * 1000 + now.tv_usec / 1000);
	}

	long tick = -1;
	/* Call to render the next GL frame */
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeRender(JNIEnv* env) {
		if (tick == -1)
			tick = _getTime();
		long curTime = _getTime();
		float interval = ((float) (curTime - tick)) / 1000;
		tick = curTime;
		appRender(interval, sWindowWidth, sWindowHeight);
	}
} // extern "C"

TextureInfo loadTexture(std::string path) {
	jclass cls = g_env->GetObjectClass(g_pngmgr);
	jmethodID mid = g_env->GetMethodID(cls, "loadTexture", "(Ljava/lang/String;)Lru/footmade/keplerc/TextureInfo;");
	jstring name = g_env->NewStringUTF(path.c_str());
	jobject textureInfo = g_env->CallObjectMethod(g_pngmgr, mid, name);
	jclass textureInfoClass = g_env->GetObjectClass(textureInfo);
	jfieldID textureId = g_env->GetFieldID(textureInfoClass, "textureId", "I");
	jfieldID width = g_env->GetFieldID(textureInfoClass, "width", "I");
	jfieldID height = g_env->GetFieldID(textureInfoClass, "height", "I");
	TextureInfo result;
	result.textureId = g_env->GetIntField(textureInfo, textureId);
	result.width = g_env->GetIntField(textureInfo, width);
	result.height = g_env->GetIntField(textureInfo, height);
	return result;
}
