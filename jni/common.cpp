#include "common.h"

TextureHelper* Common::textureHelper = NULL;
Game* Common::game = NULL;
GameLogic* Common::gameLogic = NULL;
GameRenderer* Common::gameRenderer = NULL;

void Common::init(Application& app) {
	textureHelper = new TextureHelper();
	game = new Game(app.getScreenWidth(), app.getScreenHeight());
	gameLogic = new GameLogic(*game);
	gameRenderer = new GameRenderer(*game);
}

TextureHelper* Common::getTextureHelper() {
	return textureHelper;
}

Game* Common::getGame() {
	return game;
}

GameLogic* Common::getGameLogic() {
	return gameLogic;
}

GameRenderer* Common::getGameRenderer() {
	return gameRenderer;
}

void Common::deinit() {
	delete textureHelper;
	delete game;
	delete gameLogic;
	delete gameRenderer;
}
