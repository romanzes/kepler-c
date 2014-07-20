#ifndef LOGIC_H_
#define LOGIC_H_

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

#endif /* LOGIC_H_ */
