#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include "string"
#include "app.h"
#include "common.h"
#include "importgl.h"
#include "input.h"

static int sWindowWidth = 0;
static int sWindowHeight = 0;

static JNIEnv *g_env;
static jobject g_pngmgr, g_vibrator;

static Application *app = NULL;

extern "C"
{
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainActivity_nativeAccelerometerChanged(JNIEnv* env, jobject thiz,
			jfloat x, jfloat y, jfloat z) {
		setAccelerometerValues(x, y, z);
	}

	int textureReloadFlag = 0;
	/* Call to initialize the graphics state */
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeInit(JNIEnv* env, jobject thiz, jobject pngmgr, jobject vibrator) {
		LOGI("nativeInit");
		g_env = env;
		g_pngmgr = g_env->NewGlobalRef(pngmgr);
		g_vibrator = g_env->NewGlobalRef(vibrator);
		if (textureReloadFlag) {
			Common::getTextureHelper()->reloadTextures();
		}
	}

	int inited = 0;
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h) {
		LOGI("nativeResize");
		sWindowWidth = w;
		sWindowHeight = h;
		if (inited)
			app->resizeScreen(w, h);
		else {
			importGLInit();
			app = new Application(w, h);
			app->start();
		}
		inited = 1;
		textureReloadFlag = 1;
	}

	/* Call to finalize the graphics state */
	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainRenderer_nativeDone(JNIEnv* env) {
		LOGI("nativeDone");
		g_env->DeleteGlobalRef(g_pngmgr);
		g_env->DeleteGlobalRef(g_vibrator);
		importGLDeinit();
		delete app;
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeTouchDown(JNIEnv* env,
			jobject thiz, jfloat x, jfloat y) {
		LOGI("nativeTouchDown");
		touchDown(x, sWindowHeight - y);
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeTouchUp(JNIEnv* env,
			jobject thiz, jfloat x, jfloat y) {
		LOGI("nativeTouchUp");
		touchUp(x, sWindowHeight - y);
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativePause(JNIEnv* env) {
		LOGI("nativePause");
		if (inited) app->pause();
	}

	JNIEXPORT void JNICALL Java_ru_footmade_keplerc_MainGLSurfaceView_nativeResume(JNIEnv* env) {
		LOGI("nativeResume");
		if (inited) app->resume();
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

		if (inited) app->render(interval);
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

void vibrate(int millis) {
	jclass cls = g_env->GetObjectClass(g_vibrator);
	jmethodID mid = g_env->GetMethodID(cls, "vibrate", "(I)V");
	g_env->CallVoidMethod(g_vibrator, mid, millis);
}
