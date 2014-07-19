#include "app.h"
#include "common.h"
#include "gameover.h"
#include "gamescreen.h"
#include "mainmenu.h"

Application::Application(int screenWidth, int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	currentScreen = NULL;
}

void Application::start() {
	Common::init(*this);
	setScreen(SCREEN_MAIN_MENU);
}

void Application::resizeScreen(int width, int height) {
	screenWidth = width;
	screenHeight = height;
	currentScreen->resize(width, height);
}

int Application::getScreenWidth() const {
	return screenWidth;
}

int Application::getScreenHeight() const {
	return screenHeight;
}

void Application::setScreen(int screenId) {
	size_t pointer = (size_t) currentScreen;
	if (currentScreen != NULL)
		delete currentScreen;
	switch (screenId) {
	case SCREEN_MAIN_MENU:
		currentScreen = new MainMenu(*this);
		break;
	case SCREEN_GAMEPLAY:
		currentScreen = new GameScreen(*this);
		break;
	case SCREEN_GAME_OVER:
		currentScreen = new GameOverScreen(*this);
		break;
	}
}

void Application::render(float delta) {
	currentScreen->update(delta);
	currentScreen->render();
}

void Application::pause() {
	currentScreen->pause();
}

void Application::resume() {
	currentScreen->resume();
}

Application::~Application() {
	delete currentScreen;
	Common::deinit();
}

Screen::Screen(Application& app) : owner(app) {}

Application& Screen::getApplication() {
	return owner;
}

int Screen::getWidth() {
	return getApplication().getScreenWidth();
}

int Screen::getHeight() {
	return getApplication().getScreenHeight();
}
