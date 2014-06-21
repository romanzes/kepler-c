#include "input.h"

float accelerometerX, accelerometerY, accelerometerZ;
static int wasTouchedFlag;
static int isTouchedFlag;
static float touchX, touchY;

void setAccelerometerValues(float x, float y, float z) {
	accelerometerX = x;
	accelerometerY = y;
	accelerometerZ = z;
}

float getAccelerometerX() {
	return accelerometerX;
}

float getAccelerometerY() {
	return accelerometerY;
}

float getAccelerometerZ() {
	return accelerometerZ;
}

void touchDown(float x, float y) {
	isTouchedFlag = 1;
	touchX = x;
	touchY = y;
}

void touchUp(float x, float y) {
	if (isTouchedFlag) {
		wasTouchedFlag = 1;
		isTouchedFlag = 0;
		touchX = x;
		touchY = y;
	}
}

int wasTouched() {
	int result = wasTouchedFlag;
	wasTouchedFlag = 0;
	return result;
}

int isTouched() {
	return isTouchedFlag;
}

float getTouchX() {
	return touchX;
}

float getTouchY() {
	return touchY;
}

void resetInput() {
	isTouchedFlag = 0;
	wasTouchedFlag = 0;
}
