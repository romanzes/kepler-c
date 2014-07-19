#ifndef SHIP_H_
#define SHIP_H_

#include "math.h"
#include "vector2.h"

const int SHIP_STATE_ALIVE = 0;
const int SHIP_STATE_DESTROYING = 1;
const int SHIP_STATE_DESTROYED = 2;
const int SHIP_STATE_INVULNERABLE = 3;

const float SHIP_MAX_ANGULAR_VELOCITY = M_PI;
const float SHIP_MIN_ANGULAR_VELOCITY = M_PI / 12;

class Ship {
public:
	Ship(float scale);
	void update(float interval);
	void render();
	~Ship();
	Vector2 *points;
	int state;
	float invulnerabilityTime;
	float destroyTime;
	float speed;
	float acceleration;
	float topSpeed;
};

#endif /* SHIP_H_ */
