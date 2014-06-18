#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "importgl.h"

typedef struct {
	GLuint textureId;
	int width, height;
} TextureInfo;

// The simple framework expects the application code to define these functions.
extern void appAccelerometerUpdate(float x, float y, float z);
extern void appInit();
extern void appDeinit();
extern void appResize(int width, int height);
extern void appRender(float interval, int width, int height);
extern void appTouch(float touchX, float touchY);
extern TextureInfo loadTexture(char *path);

#endif // !APP_H_INCLUDED
