#ifndef GAMEOVER_H_
#define GAMEOVER_H_

#include "app.h"

class GameOverScreen : public Screen {
public:
	GameOverScreen(Application& app);
	void resize(int width, int height);
	void update(float delta);
	void render();
	~GameOverScreen();
private:
	void layout();
	Sprite* restartButton;
};

#endif /* GAMEOVER_H_ */
