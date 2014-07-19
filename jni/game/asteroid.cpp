#include "app.h"
#include "asteroid.h"
#include "importgl.h"
#include "math.h"
#include "polygon.h"
#include "vector2.h"
#include "util.h"

static const int ASTEROID_VERTEX_COUNT = 7;
static const float ASTEROID_MAX_VELOCITY = 0.2f;
static const float ASTEROID_MAX_ANGULAR_VELOCITY = M_PI;
static const int ASTEROID_COLOR_COUNT = 4;
static const int ASTEROID_COLORS[] = { 0xff6a00, 0xa0a0a0, 0xb200ff, 0xffd800 };
static const float ASTEROID_EXPLOSION_VELOCITY = 0.1f;

Asteroid* Asteroid::largeAsteroid(float x, float y, float scale) {
	Asteroid *result = new Asteroid();
	result->points = new Vector2[ASTEROID_VERTEX_COUNT + 2];
	result->points[0].x = x;
	result->points[0].y = y;
	float angularStep = M_PI * 2 / ASTEROID_VERTEX_COUNT;
	for (int i = 1; i <= ASTEROID_VERTEX_COUNT; i++) {
		float vertexAngle = angularStep * i;
		float radius = scale * ASTEROID_RADIUS;
		float vertexRadius = frand(radius / 2, radius);
		result->points[i].x = x + cos(vertexAngle) * vertexRadius;
		result->points[i].y = y + sin(vertexAngle) * vertexRadius;
	}
	result->points[ASTEROID_VERTEX_COUNT + 1] = result->points[1];
	float velocityAngle = atan2(x, y);
	float velocityMagnitude = frand(0, scale * ASTEROID_MAX_VELOCITY);
	result->velocity.x = cos(velocityMagnitude) * velocityMagnitude;
	result->velocity.y = sin(velocityAngle) * velocityMagnitude;
	result->angularVelocity = frand(-ASTEROID_MAX_ANGULAR_VELOCITY, ASTEROID_MAX_ANGULAR_VELOCITY);
	result->size = ASTEROID_SIZE_LARGE;
	result->pointCount = ASTEROID_VERTEX_COUNT + 2;
	result->vertexOffset = 1;
	result->vertexCount = ASTEROID_VERTEX_COUNT;
	result->color = ASTEROID_COLORS[rand() % ASTEROID_COLOR_COUNT];
	return result;
}

std::vector<Asteroid*> Asteroid::split(float scale) {
	std::vector<Asteroid*> result;
	for (int i = ASTEROID_VERTEX_COUNT, j = 1; j <= ASTEROID_VERTEX_COUNT; i = j++) {
		Asteroid *asteroid = new Asteroid();
		asteroid->size = ASTEROID_SIZE_SMALL;
		asteroid->pointCount = 3;
		asteroid->vertexOffset = 0;
		asteroid->vertexCount = 3;
		asteroid->points = new Vector2[3];
		asteroid->points[0].x = points[0].x;
		asteroid->points[0].y = points[0].y;
		asteroid->points[1].x = points[i].x;
		asteroid->points[1].y = points[i].y;
		asteroid->points[2].x = points[j].x;
		asteroid->points[2].y = points[j].y;
		asteroid->angularVelocity = 0;
		float velocityAngle = atan2f(asteroid->points[1].x - asteroid->points[0].x,
				asteroid->points[1].y - asteroid->points[0].y);
		asteroid->velocity.x = velocity.x + sin(velocityAngle) * scale * ASTEROID_EXPLOSION_VELOCITY;
		asteroid->velocity.y = velocity.y + cos(velocityAngle) * scale * ASTEROID_EXPLOSION_VELOCITY;
		asteroid->color = color;
		result.push_back(asteroid);
	}
	return result;
}

void Asteroid::update(float interval, float distance, float dAngle) {
	for (int j = 0; j < pointCount; j++) {
		points[j].y -= distance;
		vecMulAddV(&points[j], &velocity, interval);
		vecRotate(&points[j], dAngle);
		vecRotateAround(&points[j], &points[0], angularVelocity * interval);
	}
}

int Asteroid::containsPoint(Vector2* point) {
	return polyContainsV(points, vertexOffset, vertexCount, point);
}

int Asteroid::intersectsPolygon(Vector2* points, int offset, int length) {
	return polygonsIntersect(this->points, vertexOffset, vertexCount, points, offset, length);
}

void Asteroid::render() {
	glColor(color);
	glVertexPointer(2, GL_FLOAT, 0, points + vertexOffset);
	GLenum mode = size == ASTEROID_SIZE_LARGE ? GL_TRIANGLE_FAN : GL_TRIANGLES;
	glDrawArrays(mode, 0, vertexCount);
}

float Asteroid::getX() {
	return points[0].x;
}

float Asteroid::getY() {
	return points[0].y;
}
