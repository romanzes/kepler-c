#ifndef TEXTURES_H_
#define TEXTURES_H_

#include "importgl.h"
#include "vector"

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

struct RegionInfo {
	int textureId;
	int x, y, w, h;
};

typedef struct {
	GLuint textureId;
	int width, height;
} TextureInfo;

class TextureRegion {
public:
	int textureId;
	float u1, v1, u2, v2;
	float width, height;
	float texture[8];
	void draw(float x, float y, float width, float height);
};

class Sprite {
public:
	Sprite(TextureRegion *region, float x, float y, float width, float height);
	TextureRegion *region;
	float x, y;
	float width, height;
	int isTouched(float x, float y);
	void draw();
	~Sprite() {}
private:
	float points[8];
};

class TextureHelper {
public:
	TextureHelper();
	void reloadTextures();
	TextureRegion *getTextureRegion(int regionId);
	int drawNumber(int number, float x, float y, float height);
	int drawSignedNumber(int number, float x, float y, float height);
	int drawTime(int seconds, float x, float y, float height);
	~TextureHelper();
private:
	std::vector<TextureInfo> textures;
	TextureRegion* digits[10];
	TextureRegion* colon;
	TextureRegion* minus;
	TextureRegion* plus;
};


#endif /* TEXTURES_H_ */
