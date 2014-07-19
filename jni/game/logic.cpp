#include "../app.h"
#include "../input.h"
#include "../util.h"
#include "../vector2.h"
#include "asteroid.h"
#include "bullet.h"
#include "game.h"
#include "math.h"
#include "ship.h"
#include "malloc.h"

static const int COLLISION_TIME_PENALTY = 30;
static const int INITIAL_TIME = 60;
static const float BULLET_INTERVAL = 0.25f;
static const float COMBO_TIMEOUT = 2.5f;
static const int LARGE_ASTEROID_LIMIT = 8;

GameLogic::GameLogic(Game& game) : game(game) {
	angularVelocity = 0;
	lastBulletTime = 0;
	comboTime = 0;
	largeAsteroidCount = 0;
}

int GameLogic::addAsteroid() {
	for (int tries = 0; tries < 10; tries++) {
		float radius = frand(game.fieldRadius, game.activeSpaceRadius);
		float angle = frand(0, M_PI);
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		for (Asteroid *asteroid : game.asteroids) {
			if (vecDistanceF(x, y, asteroid->getX(), asteroid->getY()) < game.scale * ASTEROID_RADIUS * 2) {
				continue;
			}
		}
		game.asteroids.push_back(Asteroid::largeAsteroid(x, y, game.scale));
		largeAsteroidCount++;
		return 1;
	}
	return 0;
}

void GameLogic::addNeccessaryAsteroids() {
	while (largeAsteroidCount < LARGE_ASTEROID_LIMIT) {
		if (!addAsteroid())
			break;
	}
}

void GameLogic::updateAsteroids(float interval, float distance, float dAngle) {
	for (Asteroid *asteroid : game.asteroids) {
		asteroid->update(interval, distance, dAngle);
	}
	addNeccessaryAsteroids();
}

void GameLogic::updateBullets(float interval, float distance, float dAngle) {
	lastBulletTime += interval;
	for (Bullet *bullet : game.bullets) {
		bullet->update(interval, distance, dAngle);
	}
}

void GameLogic::processCollisions() {
	processAsteroidWorldCollisions();
	processBulletWorldCollisions();
	processBulletAsteroidCollisions();
	processShipAsteroidCollisions();
}

void GameLogic::processAsteroidWorldCollisions() {
	for (auto asteroid = game.asteroids.begin(); asteroid != game.asteroids.end(); ++asteroid) {
		float vecLen = vecLenF((*asteroid)->getX(), (*asteroid)->getY());
		if (vecLen > game.activeSpaceRadius) {
			if ((*asteroid)->size == ASTEROID_SIZE_LARGE) {
				largeAsteroidCount--;
			}
			game.asteroids.erase(asteroid);
			delete *asteroid;
			asteroid--;
		}
	}
}

void GameLogic::processBulletWorldCollisions() {
	for (auto bullet = game.bullets.begin(); bullet != game.bullets.end(); ++bullet) {
		if (vecLenV(&(*bullet)->position) > game.activeSpaceRadius) {
			game.bullets.erase(bullet);
			//delete *bullet; TODO uncomment
			bullet--;
		}
	}
}

void GameLogic::processBulletAsteroidCollisions() {
	for (auto bullet = game.bullets.begin(); bullet != game.bullets.end(); ++bullet) {
		for (auto asteroid = game.asteroids.begin(); asteroid != game.asteroids.end(); ++asteroid) {
			if ((*asteroid)->containsPoint(&(*bullet)->position)) {
				if ((*asteroid)->size == ASTEROID_SIZE_LARGE) {
					largeAsteroidCount--;
					std::vector<Asteroid*> debris = (*asteroid)->split(game.scale);
					game.asteroids.insert(game.asteroids.end(), debris.begin(), debris.end());
				}
				game.asteroids.erase(asteroid);
				//delete *asteroid; TODO uncomment
				game.bullets.erase(bullet);
				bullet--;
				//delete *bullet; TODO uncomment
				increaseScore();
				break;
			}
		}
	}
}

void GameLogic::processShipAsteroidCollisions() {
	if (game.ship->state == SHIP_STATE_ALIVE) {
		for (Asteroid *asteroid : game.asteroids) {
			if (asteroid->intersectsPolygon(game.ship->points, 0, 4)) {
				game.ship->state = SHIP_STATE_INVULNERABLE;
				game.combo = 0;
				game.ship->invulnerabilityTime = 0;
				game.changeTimeRemaining(-COLLISION_TIME_PENALTY);
				vibrate(200); // TODO move to renderer
				break;
			}
		}
	}
}

void GameLogic::increaseScore() {
	game.changeCombo(1);
	game.changeScore(game.combo);
	comboTime = 0;
	if (game.combo % 5 == 0) {
		int bonusTime = game.combo / 5;
		game.changeTimeRemaining(bonusTime);
	}
}

void GameLogic::checkDeath() {
	if (game.timeRemaining < 0) {
		game.ship->state = SHIP_STATE_DESTROYING;
		game.ship->destroyTime = 0.0f;
		game.timeRemaining = 0;
	}
}

void GameLogic::restart() {
	for (Asteroid *asteroid : game.asteroids) {
		free(asteroid->points);
	}
	largeAsteroidCount = 0;
	game.ship = new Ship(game.scale);
	game.score = 0;
	game.timeRemaining = INITIAL_TIME;
	game.combo = 0;
	game.ship->state = SHIP_STATE_ALIVE;
	game.state = GAME_STATE_ACTIVE;
}

void GameLogic::processInput() {
	if (isTouched() && lastBulletTime >= BULLET_INTERVAL) {
		switch (game.ship->state) {
		case SHIP_STATE_ALIVE:
		case SHIP_STATE_INVULNERABLE:
			Bullet *bullet = new Bullet();
			bullet->position.x = 0;
			bullet->position.y = 0;
			bullet->angle = 0;
			bullet->velocity.x = 0;
			bullet->velocity.y = game.scale * BULLET_SPEED;
			game.bullets.push_back(bullet);
			lastBulletTime = 0;
			break;
		}
	}
	angularVelocity = -SHIP_MAX_ANGULAR_VELOCITY * getAccelerometerX() / 10;
	if (fabs(angularVelocity) < SHIP_MIN_ANGULAR_VELOCITY)
		angularVelocity = 0;
}

void GameLogic::update(float interval) {
	game.ship->speed += game.ship->acceleration * interval;
	if (game.ship->speed > game.ship->topSpeed)
		game.ship->speed = game.ship->topSpeed;
	float distance = game.ship->speed * interval;
	game.ship->update(interval);
	float rotateAngle = angularVelocity * interval;
	game.handleMotion(distance, rotateAngle);
	if (game.state == GAME_STATE_ACTIVE) {
		updateAsteroids(interval, distance, rotateAngle);
		updateBullets(interval, distance, rotateAngle);
		processCollisions();
		if (game.ship->state == SHIP_STATE_ALIVE) {
			game.timeRemaining -= interval;
			checkDeath();
		}
	}
	comboTime += interval;
	if (comboTime > COMBO_TIMEOUT)
		game.combo = 0;
	if (game.ship->state == SHIP_STATE_DESTROYED)
		game.state = GAME_STATE_OVER;
}
