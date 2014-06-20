#include "app.h"
#include "game.h"
#include "gameover.h"
#include "importgl.h"
#include "mainmenu.h"
#include "textures.h"
#include "util.h"

#define SCREEN_MAIN_MENU 1
#define SCREEN_GAMEPLAY 2
#define SCREEN_GAME_OVER 3
static int currentScreen;

// Called from the app framework.
void appInit() {
	loadTextures();
	currentScreen = SCREEN_MAIN_MENU;
}

// Called from the app framework.
void appResize(int width, int height) {
	switch (currentScreen) {
	case SCREEN_MAIN_MENU:
		gameInit(width, height);
		mainMenuInit(width, height);
		break;
	case SCREEN_GAMEPLAY:
		gameRestart();
		break;
	case SCREEN_GAME_OVER:
		gameOverInit(width, height);
		break;
	}
}

// Called from the app framework.
/* The tick is current time in milliseconds, width and height
 * are the image dimensions to be rendered.
 */
void appRender(float interval, int width, int height) {
	switch (currentScreen) {
	case SCREEN_MAIN_MENU:
		gameUpdate(interval);
		gameRender();
		mainMenuProcessInput();
		mainMenuRender();
		break;
	case SCREEN_GAMEPLAY:
		gameProcessInput(interval);
		gameUpdate(interval);
		gameRender();
		if (!gameIsRunning()) {
			gameOverInit(width, height);
			currentScreen = SCREEN_GAME_OVER;
		}
		break;
	case SCREEN_GAME_OVER:
		gameUpdate(interval);
		gameRender();
		gameOverProcessInput();
		gameOverRender();
		break;
	}
}

// Called from the app framework.
void appDeinit() {
	gameDeinit();
	freeTextures();
}

void startGame() {
	gameRestart();
	currentScreen = SCREEN_GAMEPLAY;
}
