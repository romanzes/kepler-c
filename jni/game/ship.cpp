#include "importgl.h"
#include "math.h"
#include "ship.h"
#include "util.h"

static const float SHIP_WIDTH = 0.05f;
static const float SHIP_HEIGHT = 0.075f;
static const float SHIP_ACCELERATION = 0.1f;
static const float SHIP_TOP_SPEED = 0.3f;
static const float SHIP_INVULNERABILITY_PERIOD = 2.0f;
static const float SHIP_DESTROY_PERIOD = 1.0f;
static const float SHIP_DESTROY_ROTATE_VELOCITY = M_PI * 4;
static const float SHIP_INVULNERABILITY_BLINK_PERIOD = 0.1f;

static const int SHIP_COLOR = 0xff0000ff;

Ship::Ship(float scale) {
	float width = scale * SHIP_WIDTH;
	float height = scale * SHIP_HEIGHT;
	points = new Vector2[4];
	points[0].x = -width / 2;
	points[0].y = -height / 2;
	points[1].x = 0;
	points[1].y = -height / 4;
	points[2].x = 0;
	points[2].y = height / 2;
	points[3].x = width / 2;
	points[3].y = -height / 2;

	speed = 0;
	acceleration = scale * SHIP_ACCELERATION;
	topSpeed = scale * SHIP_TOP_SPEED;
}

void Ship::update(float interval) {
	switch (state) {
	case SHIP_STATE_INVULNERABLE:
		invulnerabilityTime += interval;
		if (invulnerabilityTime > SHIP_INVULNERABILITY_PERIOD) {
			state = SHIP_STATE_ALIVE;
		}
		break;
	case SHIP_STATE_DESTROYING:
		destroyTime += interval;
		if (destroyTime > SHIP_DESTROY_PERIOD) {
			state = SHIP_STATE_DESTROYED;
		} else {
			for (int i = 0; i < 4; i++) {
				vecRotate(&points[i], SHIP_DESTROY_ROTATE_VELOCITY * interval);
			}
		}
		break;
	}
}

void Ship::render() {
	if (state != SHIP_STATE_DESTROYED) {
		if (state == SHIP_STATE_INVULNERABLE) {
			int periodNumber = (int) (invulnerabilityTime / SHIP_INVULNERABILITY_BLINK_PERIOD);
			if (periodNumber % 2)
				return;
		}
		float scaleRatio = 1.0f;
		if (state == SHIP_STATE_DESTROYING) {
			scaleRatio = (SHIP_DESTROY_PERIOD - destroyTime) / SHIP_DESTROY_PERIOD;
			glMatrixMode(GL_PROJECTION_MATRIX);
			glScalef(scaleRatio, scaleRatio, scaleRatio);
		}
		glColor(SHIP_COLOR);
		glVertexPointer(2, GL_FLOAT, 0, points);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		if (state == SHIP_STATE_DESTROYING) {
			scaleRatio = 1 / scaleRatio;
			glMatrixMode(GL_PROJECTION_MATRIX);
			glScalef(scaleRatio, scaleRatio, scaleRatio);
		}
	}
}
