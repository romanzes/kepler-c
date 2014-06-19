#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "importgl.h"
#include "input.h"
#include "polygon.h"
#include "textures.h"
#include "time.h"
#include "util.h"
#include "vector"
#include "vector2.h"

#undef PI
#define PI 3.1415926535897932f

static float scrW, scrH;
static float scale;

static float screenRadius;
static float activeSpaceRadius;

static const float STAR_INTERVAL = 0.1f;
static int starCount;
static Vector2 *stars;

static const int SHIP_STATE_ACTIVE = 1;
static const int SHIP_STATE_DESTROYED = 2;
typedef struct Ship
{
	float speed;
	float acceleration;
	float topSpeed;
	int state;
	Vector2 *points;
} Ship;
static const float SHIP_WIDTH = 0.1f;
static const float SHIP_HEIGHT = 0.15f;
static const float SHIP_ACCELERATION = 0.2f;
static const float SHIP_TOP_SPEED = 0.4f;
static const float SHIP_MAX_ANGULAR_VELOCITY = PI;
static const float SHIP_MIN_ANGULAR_VELOCITY = PI / 6;
static Ship ship;

static const int ASTEROID_SIZE_SMALL = 1;
static const int ASTEROID_SIZE_LARGE = 2;
typedef struct
{
	int size;
	int vertexCount;
	Vector2 velocity;
	float angularVelocity;
	Vector2 *points;
} Asteroid;
static const int ASTEROID_VERTEX_COUNT = 7;
static const float ASTEROID_RADIUS = 0.15f;
static const float ASTEROID_MAX_VELOCITY = 0.3f;
static const float ASTEROID_MAX_ANGULAR_VELOCITY = PI;
static const float ASTEROID_EXPLOSION_VELOCITY = 0.1f;
static int largeAsteroidCount;
static const int ASTEROID_COUNT = 8;
static std::vector<Asteroid> asteroids;

typedef struct {
	Vector2 position;
	float angle;
	Vector2 velocity;
} Bullet;
static const float BULLET_LENGTH = 0.1f;
static const float BULLET_WIDTH = 0.01f;
static const float BULLET_SPEED = 1.0f;
static std::vector<Bullet> bullets;

static const int SPACE_COLOR = 0xff000000;
static const int STAR_COLOR = 0xffffffff;
static const int SHIP_COLOR = 0xff0000ff;
static const int ASTEROID_COLOR = 0xffff6a00;
static const int BULLET_COLOR = 0xffff0000;

extern const int REGION_SCORE_STRING;
static Sprite scoreString;
static int score = 0;
extern const int REGION_TIME_STRING;
static Sprite timeString;
static float timePassed = 0;
extern const int REGION_COMBO_STRING;
static Sprite comboString;
static int combo = 0;
static float comboTime = 0;
static const float COMBO_TIMEOUT = 3.0f;

static void initGui() {
	TextureRegion scoreRegion = getTextureRegion(REGION_SCORE_STRING);
	TextureRegion timeRegion = getTextureRegion(REGION_TIME_STRING);
	TextureRegion comboRegion = getTextureRegion(REGION_COMBO_STRING);
	float scoreW = scrW / 4;
	float scoreH = scoreW * scoreRegion.height / scoreRegion.width;
	float timeH = scoreH;
	float timeW = timeH * timeRegion.width / timeRegion.height;
	float comboH = scoreH;
	float comboW = comboH * comboRegion.width / comboRegion.height;
	createSprite(&scoreString, scoreRegion, 0, scrH - scoreH, scoreW, scoreH);
	createSprite(&timeString, timeRegion, scrW - drawTime(0, scrW, scrH, timeH) - timeW, scrH - scoreH, timeW, timeH);
	createSprite(&comboString, comboRegion, 0, scrH - scoreH - comboH * 2, comboW, comboH);
}

static void createStars() {
	int cellCountW = (int) (1 / STAR_INTERVAL) + 1;
	int cellCountH = (int) (cellCountW * scrH / scrW) + 1;
	starCount = cellCountW * cellCountH;
	float cellW = scrW / cellCountW;
	float cellH = scrH / cellCountH;
	stars = (Vector2 *) malloc(starCount * sizeof(Vector2));
	for (int i = 0; i < cellCountW; i++) {
		int relX = i * cellW - scrW / 2;
		for (int j = 0; j < cellCountH; j++) {
			int relY = j * cellH - scrH / 2;
			int index = j * cellCountW + i;
			stars[index].x = frand(0, cellW) + relX;
			stars[index].y = frand(0, cellH) + relY;
		}
	}
}

static void createShip() {
	float shipWidth = scale * SHIP_WIDTH;
	float shipHeight = scale * SHIP_HEIGHT;
	ship.points = (Vector2 *) malloc(4 * sizeof(Vector2));
	ship.points[0].x = -shipWidth / 2;
	ship.points[0].y = -shipHeight / 2;
	ship.points[1].x = 0;
	ship.points[1].y = -shipHeight / 4;
	ship.points[2].x = 0;
	ship.points[2].y = shipHeight / 2;
	ship.points[3].x = shipWidth / 2;
	ship.points[3].y = -shipHeight / 2;

	ship.acceleration = scale * SHIP_ACCELERATION;
	ship.topSpeed = scale * SHIP_TOP_SPEED;
	ship.state = SHIP_STATE_ACTIVE;
}

static void updateStars(float yDistance) {
	for (int i = 0; i < starCount; i++) {
		stars[i].y -= yDistance;
		if (stars[i].y < -scrH / 2)
			stars[i].y += scrH;
		if (stars[i].x < -scrW / 2)
			stars[i].x += scrW;
		else if (stars[i].x > scrW / 2)
			stars[i].x -= scrW;
	}
}

static int createLargeAsteroid() {
	for (int tries = 0; tries < 10; tries++) {
		float radius = frand(screenRadius, activeSpaceRadius);
		float angle = frand(-PI, PI);
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		int satisfy = 1;
		for (int i = 0; i < asteroids.size(); i++) {
			if (vecDistanceF(x, y, asteroids[i].points[0].x, asteroids[i].points[0].y) < scale * ASTEROID_RADIUS * 2) {
				satisfy = 0;
				break;
			}
		}
		if (satisfy) {
			Asteroid newAsteroid;
			newAsteroid.points = (Vector2 *) malloc((ASTEROID_VERTEX_COUNT + 2) * sizeof(Vector2));
			newAsteroid.points[0].x = x;
			newAsteroid.points[0].y = y;
			float angularStep = PI * 2 / ASTEROID_VERTEX_COUNT;
			float radius = scale * ASTEROID_RADIUS;
			for (int i = 1; i <= ASTEROID_VERTEX_COUNT; i++) {
				float vertexAngle = angularStep * i;
				float vertexRadius = frand(radius / 2, radius);
				newAsteroid.points[i].x = x + cos(vertexAngle) * vertexRadius;
				newAsteroid.points[i].y = y + sin(vertexAngle) * vertexRadius;
			}
			newAsteroid.points[ASTEROID_VERTEX_COUNT + 1] = newAsteroid.points[1];
			float velocityAngle = atan2(x, y);
			float velocityMagnitude = frand(0, scale * ASTEROID_MAX_VELOCITY);
			newAsteroid.velocity.x = cos(velocityMagnitude) * velocityMagnitude;
			newAsteroid.velocity.y = sin(velocityAngle) * velocityMagnitude;
			newAsteroid.angularVelocity = frand(-ASTEROID_MAX_ANGULAR_VELOCITY, ASTEROID_MAX_ANGULAR_VELOCITY);
			newAsteroid.size = ASTEROID_SIZE_LARGE;
			newAsteroid.vertexCount = ASTEROID_VERTEX_COUNT + 2;
			asteroids.push_back(newAsteroid);
			largeAsteroidCount++;
			return 1;
		}
	}
	return 0;
}

static void updateAsteroids(float interval, float yDistance) {
	for (int i = 0; i < asteroids.size(); i++) {
		for (int j = 0; j < asteroids[i].vertexCount; j++) {
			asteroids[i].points[j].y -= yDistance;
			vecMulAddV(&asteroids[i].points[j], &asteroids[i].velocity, interval);
			vecRotateAround(&asteroids[i].points[j], &asteroids[i].points[0], asteroids[i].angularVelocity * interval);
		}
		if (vecLenV(&asteroids[i].points[0]) > activeSpaceRadius) {
			free(asteroids[i].points);
			if (asteroids[i].size == ASTEROID_SIZE_LARGE) {
				largeAsteroidCount--;
			}
			asteroids.erase(asteroids.begin() + i);
			i--;
		}
	}
	while (largeAsteroidCount < ASTEROID_COUNT) {
		if (!createLargeAsteroid())
			break;
	}
}

static void splitAsteroid(int index) {
	for (int i = ASTEROID_VERTEX_COUNT, j = 1; j <= ASTEROID_VERTEX_COUNT; i = j++) {
		Asteroid newAsteroid;
		newAsteroid.size = ASTEROID_SIZE_SMALL;
		newAsteroid.vertexCount = 3;
		newAsteroid.points = (Vector2 *) malloc(3 * sizeof(Vector2));
		newAsteroid.points[0].x = asteroids[index].points[0].x;
		newAsteroid.points[0].y = asteroids[index].points[0].y;
		newAsteroid.points[1].x = asteroids[index].points[i].x;
		newAsteroid.points[1].y = asteroids[index].points[i].y;
		newAsteroid.points[2].x = asteroids[index].points[j].x;
		newAsteroid.points[2].y = asteroids[index].points[j].y;
		newAsteroid.angularVelocity = 0;
		float velocityAngle = atan2f(newAsteroid.points[1].x - newAsteroid.points[0].x,
				newAsteroid.points[1].y - newAsteroid.points[0].y);
		newAsteroid.velocity.x = asteroids[index].velocity.x + sin(velocityAngle) * scale * ASTEROID_EXPLOSION_VELOCITY;
		newAsteroid.velocity.y = asteroids[index].velocity.y + cos(velocityAngle) * scale * ASTEROID_EXPLOSION_VELOCITY;
		asteroids.push_back(newAsteroid);
	}
}

static void updateBullets(float interval, float yDistance) {
	for (int i = 0; i < bullets.size(); i++) {
		vecMulAddV(&bullets[i].position, &bullets[i].velocity, interval);
		bullets[i].position.y -= yDistance;
		if (vecLenV(&bullets[i].position) > activeSpaceRadius) {
			bullets.erase(bullets.begin() + i);
			i--;
		} else {
			for (int j = 0; j < asteroids.size(); j++) {
				int offset = 0, count = asteroids[j].vertexCount;
				if (asteroids[j].size == ASTEROID_SIZE_LARGE) {
					offset = 1;
					count = ASTEROID_VERTEX_COUNT;
				}
				if (polyContainsV(asteroids[j].points, offset, count, &bullets[i].position)) {
					int index = j;
					if (asteroids[j].size == ASTEROID_SIZE_LARGE) {
						largeAsteroidCount--;
						splitAsteroid(j);
					}
					free(asteroids[index].points);
					asteroids.erase(asteroids.begin() + index);
					bullets.erase(bullets.begin() + i);
					i--;
					score++;
					comboTime = 0;
					if (comboTime < COMBO_TIMEOUT)
						combo++;
					break;
				}
			}
		}
	}
}

static void checkDeath() {
	for (int i = 0; i < asteroids.size(); i++) {
		int offset = 0, count = asteroids[i].vertexCount;
		if (asteroids[i].size == ASTEROID_SIZE_LARGE) {
			offset = 1;
			count = ASTEROID_VERTEX_COUNT;
		}
		if (polygonsIntersect(asteroids[i].points, offset, count, ship.points, 0, 4)) {
			ship.state = SHIP_STATE_DESTROYED;
			break;
		}
	}
}

static void rotateSpace(float angle) {
	int i;
	for (i = 0; i < starCount; i++) {
		vecRotate(&stars[i], angle);
	}
	for (i = 0; i < asteroids.size(); i++) {
		int j;
		for (j = 0; j < asteroids[i].vertexCount; j++) {
			vecRotate(&asteroids[i].points[j], angle);
		}
		vecRotate(&asteroids[i].velocity, angle);
	}
	for (i = 0; i < bullets.size(); i++) {
		vecRotate(&bullets[i].position, angle);
		vecRotate(&bullets[i].velocity, angle);
		bullets[i].angle -= angle;
	}
}

static void prepareRender() {
	//__android_log_print(ANDROID_LOG_INFO, "Asteroids", "scrW=%f scrH=%f", scrW, scrH);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-scrW/2, scrW/2, -scrH/2, scrH/2, -1.0f, 1.0f);
}

static void renderStars() {
	glColor(STAR_COLOR);
	glVertexPointer(2, GL_FLOAT, 0, stars);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, starCount);
}

static void renderShip() {
	if (ship.state == SHIP_STATE_ACTIVE) {
		glColor(SHIP_COLOR);
		glVertexPointer(2, GL_FLOAT, 0, ship.points);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

static void renderAsteroids() {
	glColor(ASTEROID_COLOR);
	glEnableClientState(GL_VERTEX_ARRAY);
	int i;
	for (i = 0; i < asteroids.size(); i++) {
		glVertexPointer(2, GL_FLOAT, 0, asteroids[i].points);
		GLenum mode = asteroids[i].size == ASTEROID_SIZE_LARGE ? GL_TRIANGLE_FAN : GL_TRIANGLES;
		glDrawArrays(mode, 0, asteroids[i].vertexCount);
	}
}

float bulletPointCache[8];
static void renderBullets() {
	glColor(BULLET_COLOR);
	float halfBulletLength = scale * BULLET_LENGTH / 2;
	float halfBulletWidth = scale * BULLET_WIDTH / 2;
	int i;
	for (i = 0; i < bullets.size(); i++) {
		float sn = sin(bullets[i].angle);
		float cs = cos(bullets[i].angle);
		bulletPointCache[0] = bullets[i].position.x + cs * halfBulletWidth + sn * halfBulletLength;
		bulletPointCache[1] = bullets[i].position.y + cs * halfBulletLength - sn * halfBulletWidth;
		bulletPointCache[2] = bullets[i].position.x + cs * halfBulletWidth - sn * halfBulletLength;
		bulletPointCache[3] = bullets[i].position.y - cs * halfBulletLength - sn * halfBulletWidth;
		bulletPointCache[4] = bullets[i].position.x - cs * halfBulletWidth + sn * halfBulletLength;
		bulletPointCache[5] = bullets[i].position.y + cs * halfBulletLength + sn * halfBulletWidth;
		bulletPointCache[6] = bullets[i].position.x - cs * halfBulletWidth - sn * halfBulletLength;
		bulletPointCache[7] = bullets[i].position.y - cs * halfBulletLength + sn * halfBulletWidth;
		glVertexPointer(2, GL_FLOAT, 0, bulletPointCache);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

static void renderGui() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0.0f, scrW, 0, scrH, -1.0f, 1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawSprite(&scoreString);
	drawNumber(score, scoreString.x + scoreString.width, scoreString.y, scoreString.height);
	drawSprite(&timeString);
	drawTime((int) timePassed, timeString.x + timeString.width, timeString.y, timeString.height);
	if (combo > 1) {
		drawSprite(&comboString);
		drawNumber(combo, comboString.x + comboString.width, comboString.y, comboString.height);
	}
	glDisable(GL_BLEND);
}

void gameInit(float width, float height) {
	scrW = width;
	scrH = height;
	initGui();
	scale = scrW;
	screenRadius = vecLenF(scrW, scrH) / 2 + ASTEROID_RADIUS * scale;
	activeSpaceRadius = screenRadius * 3 / 2;
	srand(time(0));
	createStars();
	createShip();
}

void gameRestart() {
	for (int i = 0; i < asteroids.size(); i++) {
		free(asteroids[i].points);
	}
	std::vector<Asteroid>().swap(asteroids);
	std::vector<Bullet>().swap(bullets);
	largeAsteroidCount = 0;
	ship.state = SHIP_STATE_ACTIVE;
	ship.speed = 0;
	score = 0;
	timePassed = 0;
	combo = 0;
}

void gameProcessInput(float interval) {
	if (wasTouched()) {
		Bullet newBullet;
		newBullet.position.x = 0;
		newBullet.position.y = 0;
		newBullet.angle = 0;
		newBullet.velocity.x = 0;
		newBullet.velocity.y = scale * BULLET_SPEED;
		bullets.push_back(newBullet);
	}
	float angularVelocity = -SHIP_MAX_ANGULAR_VELOCITY * getAccelerometerX() / 10;
	if (fabs(angularVelocity) >= SHIP_MIN_ANGULAR_VELOCITY)
		rotateSpace(angularVelocity * interval);
}

int gameIsRunning() {
	return ship.state != SHIP_STATE_DESTROYED;
}

void gameUpdate(float interval) {
	ship.speed += ship.acceleration * interval;
	if (ship.speed > ship.topSpeed)
		ship.speed = ship.topSpeed;
	float yDistance = ship.speed * interval;
	updateStars(yDistance);
	updateAsteroids(interval, yDistance);
	updateBullets(interval, yDistance);
	checkDeath();
	comboTime += interval;
	if (comboTime > COMBO_TIMEOUT) {
		combo = 0;
	}
	if (gameIsRunning())
		timePassed += interval;
}

void gameRender() {
	prepareRender();
	clearScreen(SPACE_COLOR);
	renderStars();
	renderAsteroids();
	renderBullets();
	renderShip();
	renderGui();
}

void gameDeinit() {
	free(stars);
	free(ship.points);
	int i;
	for (i = 0; i < asteroids.size(); i++) {
		free(asteroids[i].points);
	}
	std::vector<Asteroid>().swap(asteroids);
	std::vector<Bullet>().swap(bullets);
}
