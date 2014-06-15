#ifndef BUTTON_H_
#define BUTTON_H_

typedef struct {
	int textureId;
	float x, y;
	float width, height;
	float points[8];
} Button;

void initButton(Button *btn, int textureId, int x, int y, int width, int height);
int buttonTouched(Button *btn, float x, float y);
void drawButton(Button *btn);

#endif /* BUTTON_H_ */
