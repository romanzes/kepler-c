#include <math.h>
#include <float.h>
#include "vector2.h"

void vecRotate(Vector2 *vec, float angle) {
	float cs = cos(angle);
	float sn = sin(angle);
	float newX = vec->x * cs - vec->y * sn;
	float newY = vec->x * sn + vec->y * cs;
	vec->x = newX;
	vec->y = newY;
}

void vecRotateAround(Vector2 *vec1, Vector2 *vec2, float angle) {
	float cs = cos(angle);
	float sn = sin(angle);
	float dx = vec1->x - vec2->x;
	float dy = vec1->y - vec2->y;
	float newX = dx * cs - dy * sn;
	float newY = dx * sn + dy * cs;
	vec1->x = vec2->x + newX;
	vec1->y = vec2->y + newY;
}

float vecLenV(Vector2 *vec) {
	return vecLenF(vec->x, vec->y);
}

float vecLenF(float x, float y) {
	return sqrt(x * x + y * y);
}

float vecDistanceV(Vector2 *vec1, Vector2 *vec2) {
	return vecDistanceF(vec1->x, vec1->y, vec2->x, vec2->y);
}

float vecDistanceF(float x1, float y1, float x2, float y2) {
	float dx = x1 - x2;
	float dy = y1 - y2;
	return sqrt(dx * dx + dy * dy);
}

void vecMulAddV(Vector2 *vec1, Vector2 *vec2, float scale) {
	vec1->x += vec2->x * scale;
	vec1->y += vec2->y * scale;
}
