#pragma once
#include <Engine/OGLGame.h>

#include "StateHandler.h"
#include "ObjectRenderer.h"
#include "Timer.h"

/**
*  An OpenGL Game based on ASGE.
*/
class InvadersGame : public ASGE::OGLGame, public StateHandler
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
	void input(int key, int action) const;

	int  m_callback_id;           /**< The callback ID assigned by the game engine. */
	bool m_exit;                  /**< If true the game loop will exit. */
    std::unique_ptr<ObjectRenderer> m_objectRenderer;
    Timer m_timer;
};
