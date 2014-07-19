#ifndef RENDER_H_
#define RENDER_H_

#include "game.h"
#include "vector"

class Stars {
public:
	Stars(float fieldWidth, float fieldHeight);
	void update(float distance, float dAngle);
	void render();
	~Stars();
private:
	float fieldWidth;
	float fieldHeight;
	Vector2 *stars;
	int starCount;
};

class GameRenderer : IPropertyObserver {
public:
	GameRenderer(Game& game);
	void update(float delta);
	void render();
	void handleTimeRemainingChange(float dTime);
	void handleScoreChange(int dScore) {}
	void handleComboChange(int dCombo) {}
	void handleMotion(float distance, float dAngle);
	~GameRenderer() {}
private:
	Game& game;
	float viewportWidth;
	float viewportHeight;
	Stars *stars;
	Sprite* scoreString;
	Sprite* timeString;
	Sprite* comboString;
	float timeChangeIndicatorValue, timeChangeIndicatorTime;
	void initGui();
	void prepareRender();
	void renderField();
	void renderBullets();
	void renderGui();
};

#endif /* RENDER_H_ */
