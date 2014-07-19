#include "importgl.h"
#include "util.h"
#include "cstdlib"

void clearScreen(int argb) {
	GLfloat a = (GLfloat)((argb & 0xff000000) >> 24) / 0xff;
	GLfloat r = (GLfloat)((argb & 0xff0000) >> 16) / 0xff;
	GLfloat g = (GLfloat)((argb & 0xff00) >> 8) / 0xff;
	GLfloat b = (GLfloat)(argb & 0xff) / 0xff;
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void glColor(int argb) {
	GLfloat a = (GLfloat)((argb & 0xff000000) >> 24) / 0xff;
	GLfloat r = (GLfloat)((argb & 0xff0000) >> 16) / 0xff;
	GLfloat g = (GLfloat)((argb & 0xff00) >> 8) / 0xff;
	GLfloat b = (GLfloat)(argb & 0xff) / 0xff;
	glColor4f(r, g, b, a);
}

static float inv_rand_max = (float) 1 / RAND_MAX;
float frand(float min, float max) {
	float fval = rand();
	max -= min;
	return (fval * inv_rand_max * max + min);
}
