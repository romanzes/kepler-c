#include "app.h"
#include "importgl.h"
#include "textures.h"
#include "vector"

struct RegionInfo {
	int textureId;
	int x, y, w, h;
};

struct RegionInfo regions[] = {
		{ 0, 0, 0, 128, 128 }, // refresh button
		{ 1, 0, 0, 330, 70 }, // SCORE:
		{ 1, 331, 0, 55, 70 }, // 0
		{ 1, 0, 71, 55, 70 }, // 1
		{ 1, 56, 71, 55, 70 }, // 2
		{ 1, 111, 71, 55, 70 }, // 3
		{ 1, 166, 71, 55, 70 }, // 4
		{ 1, 221, 71, 55, 70 }, // 5
		{ 1, 276, 71, 55, 70 }, // 6
		{ 1, 331, 71, 55, 70 }, // 7
		{ 1, 386, 71, 55, 70 }, // 8
		{ 1, 426, 71, 55, 70 }, // 9
};

static short indices[] = { 0, 1, 2,
						   2, 3, 0 };

static std::vector<TextureInfo> textures;

static TextureRegion digits[10];

void loadTextures() {
	textures.push_back(loadTexture("refresh.png"));
	textures.push_back(loadTexture("strings.png"));
	for (int i = 0; i < 10; i++) {
		digits[i] = getTextureRegion(REGION_NUMBER_0 + i);
	}
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

void drawNumber(int number, float x, float y, float height) {
	int reverse = 0;
	while (number > 0)
	{
		reverse = reverse * 10;
		reverse = reverse + number % 10;
		number /= 10;
	}
	int offsetX = 0;
	do {
		int digit = reverse % 10;
		int digitWidth = digits[digit].width * height / digits[digit].height;
		drawTextureRegion(&digits[digit], x + offsetX, y, digitWidth, height);
		offsetX += digitWidth;
		reverse /= 10;
	} while (reverse > 0);
}

void freeTextures() {
	for (int i = 0; i < textures.size(); i++) {
		glDeleteTextures(1, &textures[i].textureId);
	}
	std::vector<TextureInfo>().swap(textures);
}
