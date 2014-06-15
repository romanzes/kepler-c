#include "app.h"
#include "game.h"
#include "gameover.h"
#include "importgl.h"
#include "util.h"

#define SCREEN_GAMEPLAY 1
#define SCREEN_GAME_OVER 2
static int currentScreen;

// Called from the app framework.
void appInit() {
	currentScreen = SCREEN_GAMEPLAY;
}

// Called from the app framework.
void appResize(int width, int height) {
	switch (currentScreen) {
	case SCREEN_GAMEPLAY:
		gameInit(width, height);
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
}

void restartGame() {
	gameRestart();
	currentScreen = SCREEN_GAMEPLAY;
}
