#include "common.h"
#include "gamescreen.h"
#include "game/game.h"
#include "game/render.h"

GameScreen::GameScreen(Application& app) : Screen(app) {
	game = Common::getGame();
	gameLogic = Common::getGameLogic();
	gameRenderer = Common::getGameRenderer();
	gameLogic->restart();
}

void GameScreen::update(float delta) {
	gameRenderer->update(delta);
	gameLogic->update(delta);
	gameLogic->processInput();
	if (!game->isPlaying())
		getApplication().setScreen(SCREEN_GAME_OVER);
}

void GameScreen::render() {
	gameRenderer->render();
}
