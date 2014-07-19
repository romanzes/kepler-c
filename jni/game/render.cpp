#include "render.h"

#include "../common.h"
#include "../importgl.h"
#include "../textures.h"
#include "../util.h"
#include "../vector2.h"
#include "asteroid.h"
#include "bullet.h"
#include "ship.h"

static const int SPACE_COLOR = 0xff000000;
static const int STAR_COLOR = 0xffffffff;
static const int BULLET_COLOR = 0xffff0000;
static const int INCREASE_TIME_COLOR = 0xff00ff00;
static const int DECREASE_TIME_COLOR = 0xffff0000;
static const int LOW_TIME_COLOR = 0xffff0000;
static const int COMBO_COLOR = 0xffffd800;

static const float STAR_INTERVAL = 0.1f;
static const float TIME_CHANGE_INDICATOR_TIMEOUT = 1.0f;

Stars::Stars(float fieldWidth, float fieldHeight) {
	this->fieldWidth = fieldWidth;
	this->fieldHeight = fieldHeight;
	int cellCountW = (int) (1 / STAR_INTERVAL) + 1;
	int cellCountH = (int) (cellCountW * fieldHeight / fieldWidth) + 1;
	starCount = cellCountW * cellCountH;
	stars = new Vector2[starCount];
	float cellW = fieldWidth / cellCountW;
	float cellH = fieldHeight / cellCountH;
	for (int i = 0; i < cellCountW; i++) {
		int relX = i * cellW - fieldWidth / 2;
		for (int j = 0; j < cellCountH; j++) {
			int relY = j * cellH - fieldHeight / 2;
			int index = j * cellCountW + i;
			stars[index].x = frand(0, cellW) + relX;
			stars[index].y = frand(0, cellH) + relY;
		}
	}
}

void Stars::update(float distance, float dAngle) {
	for (int i = 0; i < starCount; i++) {
		stars[i].y -= distance;
		vecRotate(&stars[i], dAngle);
		if (stars[i].y < -fieldHeight / 2) {
			stars[i].x = frand(-fieldWidth / 2, fieldWidth / 2);
			stars[i].y += fieldHeight;
		}
		if (stars[i].x < -fieldWidth / 2) {
			stars[i].x += fieldWidth;
			stars[i].y = frand(-fieldHeight / 2, fieldHeight / 2);
		} else if (stars[i].x > fieldWidth / 2) {
			stars[i].x -= fieldWidth;
			stars[i].y = frand(-fieldHeight / 2, fieldHeight / 2);
		}
	}
}

void Stars::render() {
	glColor(STAR_COLOR);
	glVertexPointer(2, GL_FLOAT, 0, stars);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, starCount);
}

GameRenderer::GameRenderer(Game& game) : game(game) {
	game.addObserver(this);
	viewportWidth = game.fieldWidth;
	viewportHeight = game.fieldHeight;
	stars = new Stars(viewportWidth, viewportHeight);
	initGui();
}

void GameRenderer::initGui() {
	TextureHelper *helper = Common::getTextureHelper();
	TextureRegion *scoreRegion = helper->getTextureRegion(REGION_SCORE_STRING);
	TextureRegion *timeRegion = helper->getTextureRegion(REGION_TIME_STRING);
	TextureRegion *comboRegion = helper->getTextureRegion(REGION_COMBO_STRING);
	float scoreW = viewportWidth / 4;
	float scoreH = scoreW * scoreRegion->height / scoreRegion->width;
	float timeH = scoreH;
	float timeW = timeH * timeRegion->width / timeRegion->height;
	float comboH = scoreH;
	float comboW = comboH * comboRegion->width / comboRegion->height;
	scoreString = new Sprite(scoreRegion, 0, viewportHeight - scoreH, scoreW, scoreH);
	timeString = new Sprite(timeRegion, viewportWidth - helper->drawTime(0, viewportWidth, viewportHeight, timeH) - timeW,
			viewportHeight - scoreH, timeW, timeH);
	comboString = new Sprite(comboRegion, 0, viewportHeight - scoreH - comboH * 2, comboW, comboH);
}

void GameRenderer::handleTimeRemainingChange(float dTime) {
	timeChangeIndicatorValue = dTime;
	timeChangeIndicatorTime = 0;
}

void GameRenderer::handleMotion(float distance, float dAngle) {
	stars->update(distance, dAngle);
}

void GameRenderer::prepareRender() {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-viewportWidth/2, viewportWidth/2, -viewportHeight/2, viewportHeight/2, -1.0f, 1.0f);
}

void GameRenderer::renderField() {
	glEnableClientState(GL_VERTEX_ARRAY);
	for (Asteroid *asteroid : game.asteroids) {
		asteroid->render();
	}
}

void GameRenderer::renderBullets() {
	Bullet::halfLength = game.scale * BULLET_LENGTH / 2;
	Bullet::halfWidth = game.scale * BULLET_WIDTH / 2;
	glColor(BULLET_COLOR);
	for (Bullet *bullet : game.bullets) {
		bullet->render();
	}
}

void GameRenderer::renderGui() {
	if (game.state == GAME_STATE_IDLE)
		return;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0.0f, viewportWidth, 0, viewportHeight, -1.0f, 1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	scoreString->draw();
	TextureHelper *helper = Common::getTextureHelper();
	helper->drawNumber(game.score, scoreString->x + scoreString->width, scoreString->y, scoreString->height);
	timeString->draw();
	if (game.timeRemaining <= 5) {
		glColor(LOW_TIME_COLOR);
	}
	helper->drawTime((int) game.timeRemaining, timeString->x + timeString->width, timeString->y, timeString->height);
	if (game.combo > 1) {
		glColor(COMBO_COLOR);
		comboString->draw();
		helper->drawNumber(game.combo, comboString->x + comboString->width, comboString->y, comboString->height);
	}
	if (timeChangeIndicatorTime <= TIME_CHANGE_INDICATOR_TIMEOUT && fabs(timeChangeIndicatorValue) >= 1) {
		if (timeChangeIndicatorValue < 0)
			glColor(DECREASE_TIME_COLOR);
		else
			glColor(INCREASE_TIME_COLOR);
		helper->drawSignedNumber(timeChangeIndicatorValue, timeString->x + timeString->width,
				timeString->y - timeString->height * 2, timeString->height);
	}
	glDisable(GL_BLEND);
}

void GameRenderer::update(float delta) {
	timeChangeIndicatorTime += delta;
}

void GameRenderer::render() {
	prepareRender();
	clearScreen(SPACE_COLOR);
	stars->render();
	for (Asteroid *asteroid : game.asteroids) {
		asteroid->render();
	}
	renderBullets();
	game.ship->render();
	renderGui();
}
