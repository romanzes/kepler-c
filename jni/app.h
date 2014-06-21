#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "importgl.h"
#include "string"

typedef struct {
	GLuint textureId;
	int width, height;
} TextureInfo;

// The simple framework expects the application code to define these functions.
extern void appAccelerometerUpdate(float x, float y, float z);
extern void appTouch(float touchX, float touchY);
extern TextureInfo loadTexture(std::string path);
extern void vibrate(int millis);
void appInit();
void appDeinit();
void appResize(int width, int height);
void appRender(float interval, int width, int height);
void startGame();

#endif // !APP_H_INCLUDED
