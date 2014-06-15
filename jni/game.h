#ifndef GAME_H_
#define GAME_H_

void gameInit(float width, float height);
void gameRestart();
void gameProcessInput(float interval);
void gameUpdate(float interval);
int gameIsRunning();
void gameRender();
void gameDeinit();

#endif /* GAME_H_ */
