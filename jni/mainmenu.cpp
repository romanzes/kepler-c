#include "app.h"
#include "textures.h"
#include "mainmenu.h"
#include "importgl.h"
#include "input.h"
#include "util.h"

static Sprite title1, title2;
static Sprite startButton;

static int scrW, scrH;

void mainMenuInit(int width, int height) {
	scrW = width;
	scrH = height;

	float sectorHeight = scrH / 3;
	float btnHeight = sectorHeight * 3 / 4;
	float btnWidth = btnHeight;

	TextureRegion title1Region = getTextureRegion(REGION_TITLE1_STRING);
	float title1RegionWidth = scrW * 5 / 6;
	float title1RegionHeight = title1RegionWidth * title1Region.height / title1Region.width;
	TextureRegion title2Region = getTextureRegion(REGION_TITLE2_STRING);
	float title2RegionHeight = sectorHeight;
	float title2RegionWidth = title2RegionHeight * title2Region.width / title2Region.height;
	createSprite(&title1, title1Region, (scrW - title1RegionWidth) / 2, scrH - sectorHeight + (sectorHeight - title1RegionHeight) / 2,
			title1RegionWidth, title1RegionHeight);
	createSprite(&title2, title2Region, (scrW - title2RegionWidth) / 2, (scrH - title2RegionHeight) / 2,
			title2RegionWidth, title2RegionHeight);
	createSprite(&startButton, getTextureRegion(REGION_START_BUTTON), (scrW - btnWidth) / 2, (sectorHeight - btnHeight) / 2,
			btnWidth, btnHeight);
}

void mainMenuProcessInput() {
	if (wasTouched()) {
		if (spriteTouched(&startButton, getTouchX(), getTouchY())) {
			startGame();
		}
	}
}

void mainMenuRender() {
	glViewport(0, 0, scrW, scrH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, scrW, 0, scrH, 1, -1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor(0xffffffff);
	drawSprite(&title1);
	drawSprite(&title2);
	drawSprite(&startButton);

	glDisable(GL_BLEND);
}
