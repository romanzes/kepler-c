#ifndef INPUT_H_
#define INPUT_H_

void setAccelerometerValues(float x, float y, float z);
float getAccelerometerX();
float getAccelerometerY();
float getAccelerometerZ();
void touchScreen(float x, float y);
int wasTouched();
float getTouchX();
float getTouchY();

#endif /* INPUT_H_ */
