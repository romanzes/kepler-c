#include "app.h"
#include "math.h"
#include "importgl.h"
#include "textures.h"
#include "vector"

struct RegionInfo {
	int textureId;
	int x, y, w, h;
};

struct RegionInfo regions[] = {
		{ 0, 0, 0, 128, 128 }, // refresh button
		{ 0, 128, 0, 128, 128 }, // start button
		{ 1, 0, 0, 330, 70 }, // SCORE:
		{ 1, 0, 142, 250, 70 }, // TIME:
		{ 1, 253, 142, 260, 70 }, // COMBO:
		{ 1, 300, 0, 25, 70 }, // :
		{ 1, 386, 0, 55, 70 }, // -
		{ 1, 441, 0, 55, 70 }, // +
		{ 1, 0, 213, 324, 70 }, // KEPLER
		{ 1, 333, 213, 128, 128 }, // C
		{ 1, 331, 0, 55, 70 }, // 0
		{ 1, 0, 72, 55, 70 }, // 1
		{ 1, 56, 72, 55, 70 }, // 2
		{ 1, 111, 72, 55, 70 }, // 3
		{ 1, 166, 72, 55, 70 }, // 4
		{ 1, 221, 72, 55, 70 }, // 5
		{ 1, 276, 72, 55, 70 }, // 6
		{ 1, 331, 72, 55, 70 }, // 7
		{ 1, 386, 72, 55, 70 }, // 8
		{ 1, 441, 72, 55, 70 } // 9
};

static short indices[] = { 0, 1, 2,
						   2, 3, 0 };

static std::vector<TextureInfo> textures;

static TextureRegion digits[10];
static TextureRegion colon, minus, plus;

void loadTextures() {
	textures.push_back(loadTexture("refresh.png"));
	textures.push_back(loadTexture("strings.png"));
	for (int i = 0; i < 10; i++) {
		digits[i] = getTextureRegion(REGION_NUMBER_0 + i);
	}
	colon = getTextureRegion(REGION_COLON_STRING);
	minus = getTextureRegion(REGION_MINUS_SIGN);
	plus = getTextureRegion(REGION_PLUS_SIGN);
}

static TextureInfo* getTexture(int id) {
	return &textures[id];
}

TextureRegion getTextureRegion(int regionId) {
	TextureRegion result;
	TextureInfo *texture = getTexture(regions[regionId].textureId);
	result.textureId = texture->textureId;
	result.u1 = (float) regions[regionId].x / texture->width;
	result.v1 = (float) regions[regionId].y / texture->height;
	result.u2 = result.u1 + (float) regions[regionId].w / texture->width;
	result.v2 = result.v1 + (float) regions[regionId].h / texture->height;
	result.width = regions[regionId].w;
	result.height = regions[regionId].h;
	result.texture[0] = result.u1;
	result.texture[1] = result.v2;
	result.texture[2] = result.u2;
	result.texture[3] = result.v2;
	result.texture[4] = result.u2;
	result.texture[5] = result.v1;
	result.texture[6] = result.u1;
	result.texture[7] = result.v1;
	return result;
}

void createSprite(Sprite *spr, TextureRegion region, float x, float y, float width, float height) {
	spr->region = region;

	spr->x = x;
	spr->y = y;
	spr->width = width;
	spr->height = height;

	spr->points[0] = x;
	spr->points[1] = y;
	spr->points[2] = x + width;
	spr->points[3] = y;
	spr->points[4] = x + width;
	spr->points[5] = y + height;
	spr->points[6] = x;
	spr->points[7] = y + height;
}

int spriteTouched(Sprite *spr, float x, float y) {
	float right = spr->x + spr->width;
	float top = spr->y + spr->height;
	if (x > spr->x && y > spr->y && x < right && y < top)
		return 1;
	return 0;
}

static void drawTexture(int textureId, float *points, float *texture) {
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glVertexPointer(2, GL_FLOAT, 8, points);
	glTexCoordPointer(2, GL_FLOAT, 8, texture);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

static float pointCache[8];

void drawTextureRegion(TextureRegion *region, float x, float y, float width, float height) {
	pointCache[0] = x;
	pointCache[1] = y;
	pointCache[2] = x + width;
	pointCache[3] = y;
	pointCache[4] = x + width;
	pointCache[5] = y + height;
	pointCache[6] = x;
	pointCache[7] = y + height;
	drawTexture(region->textureId, pointCache, region->texture);
}

void drawSprite(Sprite *spr) {
	drawTexture(spr->region.textureId, spr->points, spr->region.texture);
}

// returns width of resulting string
int drawNumber(int number, float x, float y, float height) {
	int width = 0;
	int number2 = number;
	int digitCount = 1;
	if (number > 0)
		digitCount = (int) log10((float) number) + 1;
	int digit[digitCount];
	for (int i = 0; i < digitCount; i++) {
		digit[i] = number2 % 10;
		number2 /= 10;
	}

	int offsetX = 0;
	for (int i = digitCount - 1; i >= 0; i--) {
		int digitWidth = digits[digit[i]].width * height / digits[digit[i]].height;
		drawTextureRegion(&digits[digit[i]], x + offsetX, y, digitWidth, height);
		offsetX += digitWidth;
	}
	return offsetX;
}

int drawSignedNumber(int number, float x, float y, float height) {
	TextureRegion sign = plus;
	if (number < 0) {
		sign = minus;
		number = -number;
	}
	int signWidth = sign.width * height / sign.height;
	drawTextureRegion(&sign, x, y, signWidth, height);
	return signWidth + drawNumber(number, x + signWidth, y, height);
}

// returns width of resulting string
int drawTime(int seconds, float x, float y, float height) {
	int minutes = seconds / 60;
	int minutes1 = minutes / 10;
	int minutes2 = minutes % 10;
	seconds %= 60;
	int seconds1 = seconds / 10;
	int seconds2 = seconds % 10;
	int offsetX = drawNumber(minutes1, x, y, height);
	offsetX += drawNumber(minutes2, x + offsetX, y, height);
	int colonWidth = colon.width * height / colon.height;
	drawTextureRegion(&colon, x + offsetX, y, colonWidth, height);
	offsetX += colonWidth;
	offsetX += drawNumber(seconds1, x + offsetX, y, height);
	offsetX += drawNumber(seconds2, x + offsetX, y, height);
	return offsetX;
}

void freeTextures() {
	for (TextureInfo &texture : textures) {
		glDeleteTextures(1, &texture.textureId);
	}
	std::vector<TextureInfo>().swap(textures);
}
