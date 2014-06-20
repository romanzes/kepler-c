#ifndef TEXTURES_H_
#define TEXTURES_H_

const int REGION_REFRESH_BUTTON = 0;
const int REGION_START_BUTTON = 1;
const int REGION_SCORE_STRING = 2;
const int REGION_TIME_STRING = 3;
const int REGION_COMBO_STRING = 4;
const int REGION_COLON_STRING = 5;
const int REGION_MINUS_SIGN = 6;
const int REGION_PLUS_SIGN = 7;
const int REGION_TITLE1_STRING = 8;
const int REGION_TITLE2_STRING = 9;
const int REGION_NUMBER_0 = 10;

typedef struct {
	int textureId;
	float u1, v1, u2, v2;
	float width, height;
	float texture[8];
} TextureRegion;

typedef struct {
	TextureRegion region;
	float x, y;
	float width, height;
	float points[8];
} Sprite;

void loadTextures();
TextureRegion getTextureRegion(int regionId);
void createSprite(Sprite *spr, TextureRegion region, float x, float y, float width, float height);
int spriteTouched(Sprite *spr, float x, float y);
void drawTextureRegion(TextureRegion *region, float x, float y, float width, float height);
void drawSprite(Sprite *spr);
int drawNumber(int number, float x, float y, float height);
int drawSignedNumber(int number, float x, float y, float height);
int drawTime(int seconds, float x, float y, float height);
void freeTextures();

#endif /* TEXTURES_H_ */
