#ifndef BULLET_H_
#define BULLET_H_

#include "vector2.h"

const float BULLET_LENGTH = 0.05f;
const float BULLET_WIDTH = 0.01f;
const float BULLET_SPEED = 1.0f;

class Bullet {
public:
	~Bullet() {}
	Vector2 position;
	float angle;
	Vector2 velocity;
	void update(float interval, float distance, float dAngle);
	void render();
	static float halfWidth;
	static float halfLength;
};

#endif /* BULLET_H_ */
