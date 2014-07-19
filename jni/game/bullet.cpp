#include "app.h"
#include "bullet.h"
#include "importgl.h"
#include "math.h"
#include "vector2.h"

void Bullet::update(float interval, float distance, float dAngle) {
	position.y -= distance;
	vecMulAddV(&position, &velocity, interval);
	vecRotate(&position, dAngle);
	vecRotate(&velocity, dAngle);
	angle -= dAngle;
}

float Bullet::halfWidth = 0;
float Bullet::halfLength = 0;

static float bulletPointCache[8];
void Bullet::render() {
	float sn = sin(angle);
	float cs = cos(angle);
	float cosHalfWidth = cs * halfWidth;
	float cosHalfLength = cs * halfLength;
	float sinHalfWidth = sn * halfWidth;
	float sinHalfLength = sn * halfLength;
	bulletPointCache[0] = position.x + cosHalfWidth + sinHalfLength;
	bulletPointCache[1] = position.y + cosHalfLength - sinHalfWidth;
	bulletPointCache[2] = position.x + cosHalfWidth - sinHalfLength;
	bulletPointCache[3] = position.y - cosHalfLength - sinHalfWidth;
	bulletPointCache[4] = position.x - cosHalfWidth + sinHalfLength;
	bulletPointCache[5] = position.y + cosHalfLength + sinHalfWidth;
	bulletPointCache[6] = position.x - cosHalfWidth - sinHalfLength;
	bulletPointCache[7] = position.y - cosHalfLength + sinHalfWidth;
	glVertexPointer(2, GL_FLOAT, 0, bulletPointCache);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
