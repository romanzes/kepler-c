#include "app.h"
#include "textures.h"
#include "gameover.h"
#include "importgl.h"
#include "input.h"
#include "util.h"

static const int BLOOD_COLOR = 0x5dff0000;
static float bloodPoints[8];
static Sprite restartButton;

static int scrW, scrH;

void gameOverInit(int width, int height) {
	scrW = width;
	scrH = height;

	bloodPoints[0] = 0;
	bloodPoints[1] = 0;
	bloodPoints[2] = width;
	bloodPoints[3] = 0;
	bloodPoints[4] = 0;
	bloodPoints[5] = height;
	bloodPoints[6] = width;
	bloodPoints[7] = height;

	float btnWidth = scrW / 3;
	float btnHeight = btnWidth;

	createSprite(&restartButton, getTextureRegion(REGION_REFRESH_BUTTON), (scrW - btnWidth) / 2, btnHeight / 2, btnWidth, btnHeight);

	resetInput();
}

void gameOverProcessInput() {
	if (wasTouched()) {
		if (spriteTouched(&restartButton, getTouchX(), getTouchY())) {
			startGame();
		}
	}
}

void gameOverRender() {
	glViewport(0, 0, scrW, scrH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, scrW, 0, scrH, 1, -1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor(BLOOD_COLOR);
	glVertexPointer(2, GL_FLOAT, 8, bloodPoints);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor(0xffffffff);
	drawSprite(&restartButton);

	glDisable(GL_BLEND);
}
