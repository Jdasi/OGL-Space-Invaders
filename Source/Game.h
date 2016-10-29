#pragma once
#include <Engine/OGLGame.h>

#include "StateHandler.h"

struct GameFont;

/**
*  An OpenGL Game based on ASGE.
*/

class InvadersGame : public ASGE::OGLGame, public StateHandler
{
public:
	InvadersGame();
	~InvadersGame();

	// Inherited via Game.
	virtual bool run() override;
	bool shouldExit() const;
	void render();

	// Inherited via OGLGame.
	virtual bool init() override;
	virtual void drawFrame() override;

private:
	void processGameActions(); 
	void input(int key, int action) const;

	int  callback_id;           /**< The callback ID assigned by the game engine. */
	bool m_exit;                /**< If true the game loop will exit. */
	std::unique_ptr<ASGE::Sprite> sprite;   /**< The space invader sprite. */
};
