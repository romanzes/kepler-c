#ifndef INPUT_H_
#define INPUT_H_

void setAccelerometerValues(float x, float y, float z);
float getAccelerometerX();
float getAccelerometerY();
float getAccelerometerZ();
void touchDown(float x, float y);
void touchUp(float x, float y);
int wasTouched();
int isTouched();
float getTouchX();
float getTouchY();
void resetInput();

#endif /* INPUT_H_ */
