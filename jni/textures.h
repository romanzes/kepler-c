#ifndef TEXTURES_H_
#define TEXTURES_H_

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
void drawNumber(int number, float x, float y, float height);
void freeTextures();

#endif /* TEXTURES_H_ */
