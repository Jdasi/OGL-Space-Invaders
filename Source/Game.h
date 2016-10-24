#pragma once
#include <Engine/OGLGame.h>
#include <string>

struct GameFont;

/**
*  Invaders Game. An OpenGL Game based on ASGE.
*/

class InvadersGame:
	public ASGE::OGLGame
{
public:
	InvadersGame();
	~InvadersGame();

	// Inherited via Game
	virtual bool run() override;
	bool shouldExit() const;
	void render();

	// Inherited via OGLGame
	virtual bool init();
	virtual void drawFrame();

private:
	void processGameActions(); 
	void input(int key, int action) const;

	int  callback_id = -1;                             /**< Input Callback ID. The callback ID assigned by the game engine. */
	bool exit = false;                                 /**< Exit boolean. If true the game loop will exit. */
	std::unique_ptr<ASGE::Sprite> sprite = nullptr;    /**< Sprite Object. The space invader sprite. */
};

