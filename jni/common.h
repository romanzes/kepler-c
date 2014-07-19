#ifndef COMMON_H_
#define COMMON_H_

#include "app.h"
#include "textures.h"
#include "game/game.h"
#include "game/render.h"

class Application;

class Common {
	friend class Application;
public:
	static TextureHelper* getTextureHelper();
	static Game* getGame();
	static GameLogic* getGameLogic();
	static GameRenderer* getGameRenderer();
private:
	static void init(Application& app);
	static void deinit();
	static TextureHelper* textureHelper;
	static Game* game;
	static GameLogic* gameLogic;
	static GameRenderer* gameRenderer;
};

#endif /* COMMON_H_ */
