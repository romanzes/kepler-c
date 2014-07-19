#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "vector2.h"
#include "vector"

const int ASTEROID_SIZE_SMALL = 1;
const int ASTEROID_SIZE_LARGE = 2;

const float ASTEROID_RADIUS = 0.1f;

class Asteroid {
public:
	static Asteroid* largeAsteroid(float x, float y, float radius);
	void update(float interval, float distance, float dAngle);
	int containsPoint(Vector2* point);
	int intersectsPolygon(Vector2* points, int offset, int length);
	std::vector<Asteroid*> split(float scale);
	void render();
	float getX();
	float getY();
	int size;
	Vector2* points;
	~Asteroid() {}
private:
	Asteroid() {}
	Vector2 velocity;
	float angularVelocity;
	int color;
	int pointCount;
	int vertexOffset;
	int vertexCount;
};

#endif /* ASTEROID_H_ */
