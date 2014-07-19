#include "common.h"
#include "input.h"
#include "mainmenu.h"
#include "textures.h"
#include "util.h"

MainMenu::MainMenu(Application& app) : Screen(app) {
	layout();
}

void MainMenu::resize(int width, int height) {
	layout();
}

void MainMenu::layout() {
	float scrW = getWidth();
	float scrH = getHeight();

	float sectorHeight = scrH / 3;
	float btnHeight = sectorHeight * 3 / 4;
	float btnWidth = btnHeight;

	TextureHelper *helper = Common::getTextureHelper();
	TextureRegion *title1Region = helper->getTextureRegion(REGION_TITLE1_STRING);
	float title1RegionWidth = scrW * 5 / 6;
	float title1RegionHeight = title1RegionWidth * title1Region->height / title1Region->width;
	TextureRegion *title2Region = helper->getTextureRegion(REGION_TITLE2_STRING);
	float title2RegionHeight = sectorHeight;
	float title2RegionWidth = title2RegionHeight * title2Region->width / title2Region->height;
	title1 = new Sprite(title1Region, (scrW - title1RegionWidth) / 2,
			scrH - sectorHeight + (sectorHeight - title1RegionHeight) / 2,
			title1RegionWidth, title1RegionHeight);
	title2 = new Sprite(title2Region, (scrW - title2RegionWidth) / 2, (scrH - title2RegionHeight) / 2,
			title2RegionWidth, title2RegionHeight);
	startButton = new Sprite(helper->getTextureRegion(REGION_START_BUTTON),
			(scrW - btnWidth) / 2, (sectorHeight - btnHeight) / 2,
			btnWidth, btnHeight);
}

void MainMenu::render() {
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
	title1->draw();
	title2->draw();
	startButton->draw();

	glDisable(GL_BLEND);
}

void MainMenu::update(float delta) {
	Common::getGameLogic()->update(delta);
	Common::getGameRenderer()->update(delta);
	if (wasTouched()) {
		if (startButton->isTouched(getTouchX(), getTouchY())) {
			getApplication().setScreen(SCREEN_GAMEPLAY);
		}
	}
}

MainMenu::~MainMenu() {
	delete title1;
	delete title2;
	delete startButton;
}
