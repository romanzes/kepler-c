#include "gameover.h"

#include "common.h"
#include "game/game.h"
#include "game/render.h"
#include "importgl.h"
#include "input.h"
#include "textures.h"
#include "util.h"

GameOverScreen::GameOverScreen(Application& app) : Screen(app) {
	layout();
}

void GameOverScreen::resize(int width, int height) {
	layout();
}

void GameOverScreen::layout() {
	float scrW = getWidth();
	float scrH = getHeight();

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
