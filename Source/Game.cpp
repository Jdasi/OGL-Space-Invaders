#include <iostream>

#include <Engine/Input.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "Game.h"
#include "Actions.h"
#include "Constants.h"
#include "GameFont.h"
#include "TextObject.h"

#include "StateStart.h"
#include "StateGameplay.h"
#include "StateGameOver.h"
#include "StatePause.h"

/**
*   @brief   Default Constructor.
*/
InvadersGame::InvadersGame()
    : callback_id(-1)
    , m_exit(false)
{
    registerState(GameState::START, std::make_unique<StateStart>(m_renderer));
    registerState(GameState::GAMEPLAY, std::make_unique<StateGameplay>(m_renderer));
    registerState(GameState::GAMEOVER, std::make_unique<StateGameOver>(m_renderer));
    registerState(GameState::PAUSE, std::make_unique<StatePause>(m_renderer));
}


/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
InvadersGame::~InvadersGame()
{
	inputs->unregisterCallback(callback_id);
}


/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The input callback should also
			 be set in the initialise function. 
*   @return  True if the game initialised correctly.
*/
bool InvadersGame::init()
{
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	if (!initAPI())
	{
		return false;
	}

	m_renderer->setWindowTitle("Invaders - Exercise 1");
	m_renderer->setClearColour(ASGE::COLOURS::BLACK);

	// Input callback function.
	callback_id = inputs->addCallbackFnc(&InvadersGame::input, this);
	
	// Load fonts we need.
	GameFont::fonts[0] = new GameFont(m_renderer->loadFont("..\\..\\Resources\\Fonts\\Comic.ttf", 42), "default", 42);
    m_renderer->setFont(GameFont::fonts[0]->id);
    	
	if (GameFont::fonts[0]->id == -1)
	{
		return false;
	}

    triggerState(GameState::START);

	return true;
}


/**
*   @brief   The main game loop. 
*   @details The main loop should be responsible for updating the game
			 and rendering the current scene. Runs until the shouldExit
			 signal is received.
*   @return  True if the game ran correctly. 
*/
bool InvadersGame::run()
{
	while (!shouldExit())
	{
		processGameActions();
		render();
	}

	return true;
}


/**
*   @brief   Should the game exit?
*   @details Has the renderer terminated or the game requesting to exit?
*   @return  True if the game should exit.
*/
bool InvadersGame::shouldExit() const
{
	return (m_renderer->exit() || m_exit);
}


/**
*   @brief   Renders the scene.
*   @details Prepares the renderer subsystem before drawing the 
			 current frame. Once the current frame is has finished
			 the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void InvadersGame::render()
{
	beginFrame();
	drawFrame();
	endFrame();
}


/**
*   @brief   Renderers the contents for this frame.
*   @details All game objects that need rendering should be done
			 in this function, as it ensures they are completed
			 before the buffers are swapped.
*   @return  void
*/
void InvadersGame::drawFrame()
{
    draw();
}


/**
*   @brief   Processes any key inputs and translates them to a GameAction.
*   @details This function is added as a callback to handle the game's 
			 input. Key presseses are translated in to GameActions which 
			 are then processed by the main game loop.
*   @param   key is the key the action relates to.
*   @param   action whether the key was released or pressed.
*   @return  void
*/
void InvadersGame::input(int key, int action) const
{
    // There is no constexpr for "Holding" or "Released" in the library;
    // So there's no point in checking for "Pressed" either.
    CommandState command_state = CommandState::RELEASED;
    if (action == 1 || action == 2)
    {
        command_state = CommandState::PRESSED;
    }

    switch (key)
    {
        case ASGE::KEYS::KEY_A:
        {
            onCommand(Command::MOVE_LEFT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_D:
        {
            onCommand(Command::MOVE_RIGHT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_W:
        {
            onCommand(Command::MOVE_UP, command_state);
            break;
        }

        case ASGE::KEYS::KEY_S:
        {
            onCommand(Command::MOVE_DOWN, command_state);
            break;
        }

        case ASGE::KEYS::KEY_ENTER:
        case ASGE::KEYS::KEY_SPACE:
        {
            onCommand(Command::SHOOT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_P:
        {
            onCommand(Command::PAUSE, command_state);
            break;
        }

        case ASGE::KEYS::KEY_ESCAPE:
        {
            onCommand(Command::QUIT, command_state);
            break;
        }
    }
}


/**
*   @brief   Processes the next game action.
*   @details Uses the game action that was a direct result of 
*            user input. It allows input to processed in a different
             thread and the game actions performed in the main thread. 
*   @return  void
*/
void InvadersGame::processGameActions()
{
	if (game_action == GameAction::EXIT)
	{
		m_exit = true;
	}

	game_action = GameAction::NONE;
}

