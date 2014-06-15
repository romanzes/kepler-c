#include "input.h"

float accelerometerX, accelerometerY, accelerometerZ;
static int wasTouchedFlag;
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

void touchScreen(float x, float y) {
	wasTouchedFlag = 1;
	touchX = x;
	touchY = y;
}

int wasTouched() {
	int result = wasTouchedFlag;
	wasTouchedFlag = 0;
	return result;
}

float getTouchX() {
	return touchX;
}

float getTouchY() {
	return touchY;
}
