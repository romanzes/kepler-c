#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "importgl.h"
#include "textures.h"
#include "string"
#include "android/log.h"

#define LOG_TAG    __FILE__
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

// The simple framework expects the application code to define these functions.
void appAccelerometerUpdate(float x, float y, float z);
void appTouch(float touchX, float touchY);
TextureInfo loadTexture(std::string path);
void vibrate(int millis);

const int SCREEN_MAIN_MENU = 1;
const int SCREEN_GAMEPLAY = 2;
const int SCREEN_GAME_OVER = 3;

class Application;

class Screen {
public:
	Screen(Application& app);
	Application& getApplication();
	virtual void resize(int width, int height) {}
	int getWidth();
	int getHeight();
	virtual void update(float delta) {}
	virtual void render() {}
	virtual void pause() {}
	virtual void resume() {}
	virtual ~Screen() {}
private:
	Application& owner;
};

class Application {
public:
	Application(int screenWidth, int screenHeight);
	void start();
	void resizeScreen(int width, int height);
	int getScreenWidth() const;
	int getScreenHeight() const;
	void setScreen(int screenId);
	void render(float delta);
	void pause();
	void resume();
	~Application();
private:
	Screen* currentScreen;
	int screenWidth, screenHeight;
};

#endif // !APP_H_INCLUDED
