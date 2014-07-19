#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "app.h"

class MainMenu : public Screen {
public:
	MainMenu(Application& app);
	void resize(int width, int height);
	void update(float delta);
	void render();
	~MainMenu();
private:
	Sprite* title1;
	Sprite* title2;
	Sprite* startButton;
	void layout();
};

#endif /* MAINMENU_H_ */
