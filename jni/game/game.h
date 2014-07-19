#ifndef GAME_H_
#define GAME_H_

#include "asteroid.h"
#include "bullet.h"
#include "textures.h"
#include "ship.h"
#include "vector2.h"
#include "list"

const float GAME_STATE_IDLE = 0;
const float GAME_STATE_ACTIVE = 1;
const float GAME_STATE_OVER = 2;

class IPropertyObserver {
public:
    virtual ~IPropertyObserver() {}
    virtual void handleTimeRemainingChange(float dTime) = 0;
    virtual void handleScoreChange(int dScore) = 0;
    virtual void handleComboChange(int dCombo) = 0;
    virtual void handleMotion(float distance, float dAngle) = 0;
};

class Game {
	friend class GameLogic;
	friend class GameRenderer;
public:
	Game(float fieldWidth, float fieldHeight);
	float getFieldWidth();
	float getFieldHeight();
	int isPlaying();
	~Game() {}
private:
	float fieldWidth;
	float fieldHeight;
	float fieldRadius;
	float activeSpaceRadius;
	float scale;
	int state;
	Ship *ship;
	std::list<Asteroid*> asteroids;
	std::list<Bullet*> bullets;
	float timeRemaining;
	int score;
	int combo;
	void createShip();
	void changeTimeRemaining(float dTime);
	void changeScore(int dScore);
	void changeCombo(int dCombo);
	void handleMotion(float distance, float dAngle);
	std::list<IPropertyObserver*> observers;
	void addObserver(IPropertyObserver* observer);
};

class GameLogic {
public:
	GameLogic(Game& game);
	void update(float interval);
	void processInput();
	void restart();
private:
	Game& game;
	int largeAsteroidCount;
	float lastBulletTime;
	void updateStars(float shipDistance);
	void updateAsteroids(float interval, float distance, float dAngle);
	void updateBullets(float interval, float distance, float dAngle);
	void addNeccessaryAsteroids();
	int addAsteroid();
	void processCollisions();
	void processAsteroidWorldCollisions();
	void processBulletWorldCollisions();
	void processBulletAsteroidCollisions();
	void processShipAsteroidCollisions();
	void increaseScore();
	void checkDeath();
	void processInput(float interval);
	float angularVelocity;
	float comboTime;
};

#endif /* GAME_H_ */
