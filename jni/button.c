#include "button.h"
#include "importgl.h"

static float texture[] = { 0.0f, 1.0f,
						   1.0f, 1.0f,
						   1.0f, 0.0f,
						   0.0f, 0.0f };
static short indices[] = { 0, 1, 2,
						   2, 3, 0 };

void initButton(Button *btn, int textureId, int x, int y, int width, int height) {
	btn->textureId = textureId;
	btn->x = x;
	btn->y = y;
	btn->width = width;
	btn->height = height;

	btn->points[0] = x;
	btn->points[1] = y;
	btn->points[2] = x + width;
	btn->points[3] = y;
	btn->points[4] = x + width;
	btn->points[5] = y + height;
	btn->points[6] = x;
	btn->points[7] = y + height;
}

int buttonTouched(Button *btn, float x, float y) {
	float right = btn->x + btn->width;
	float top = btn->y + btn->height;
	if (x > btn->x && y > btn->y && x < right && y < top)
		return 1;
	return 0;
}

void drawButton(Button *btn) {
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, btn->textureId);

	glVertexPointer(2, GL_FLOAT, 8, btn->points);
	glTexCoordPointer(2, GL_FLOAT, 8, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
