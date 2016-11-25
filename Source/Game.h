#pragma once
#include <Engine/OGLGame.h>

#include "Constants.h"
#include "StateHandler.h"
#include "ObjectRenderer.h"
#include "Timer.h"

/**
*  An OpenGL Game based on ASGE.
*/
class InvadersGame : public ASGE::OGLGame
{
public:
    InvadersGame();
	virtual ~InvadersGame();

	// Inherited via Game.
	virtual bool run() override;
	bool shouldExit() const;
	void render();

	// Inherited via OGLGame.
	virtual bool init() override;
	virtual void drawFrame() override;

private:
	void input(int _key, int _action) const;

	int  callback_id;           /**< The callback ID assigned by the game engine. */
    std::unique_ptr<ObjectRenderer> object_renderer;
    std::unique_ptr<StateHandler> state_handler;
    Timer timer;
};
