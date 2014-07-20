#include "gameover.h"

#include "common.h"
#include "game/game.h"
#include "game/render.h"
#include "importgl.h"
#include "input.h"
#include "textures.h"
#include "util.h"

static const int BLOOD_COLOR = 0x5dff0000;
static float bloodPoints[8];

GameOverScreen::GameOverScreen(Application& app) : Screen(app) {
	layout();
}

void GameOverScreen::resize(int width, int height) {
	layout();
}

void GameOverScreen::layout() {
	float scrW = getWidth();
	float scrH = getHeight();

	bloodPoints[0] = 0;
	bloodPoints[1] = 0;
	bloodPoints[2] = scrW;
	bloodPoints[3] = 0;
	bloodPoints[4] = 0;
	bloodPoints[5] = scrH;
	bloodPoints[6] = scrW;
	bloodPoints[7] = scrH;

	float sectorHeight = scrH / 3;
	float btnHeight = sectorHeight * 3 / 4;
	float btnWidth = btnHeight;

	TextureHelper *helper = Common::getTextureHelper();
	restartButton = new Sprite(helper->getTextureRegion(REGION_REFRESH_BUTTON),
			(scrW - btnWidth) / 2, (sectorHeight - btnHeight) / 2,
			btnWidth, btnHeight);
}

void GameOverScreen::render() {
	Common::getGameRenderer()->render();

	float scrW = getWidth();
	float scrH = getHeight();

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
	restartButton->draw();

	glDisable(GL_BLEND);
}

void GameOverScreen::update(float delta) {
	Common::getGameLogic()->update(delta);
	Common::getGameRenderer()->update(delta);
	if (wasTouched()) {
		if (restartButton->isTouched(getTouchX(), getTouchY())) {
			getApplication().setScreen(SCREEN_GAMEPLAY);
		}
	}
}

GameOverScreen::~GameOverScreen() {
	delete restartButton;
}
