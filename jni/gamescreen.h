#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

#include "app.h"
#include "game/game.h"
#include "game/render.h"

class GameScreen : public Screen {
public:
	GameScreen(Application& app);
	void update(float delta);
	void render();
private:
	Game *game;
	GameLogic *gameLogic;
	GameRenderer *gameRenderer;
};

#endif /* GAMESCREEN_H_ */
