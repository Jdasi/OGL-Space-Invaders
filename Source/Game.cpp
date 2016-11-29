#include <iostream>

#include <Engine/Input.h>
#include <Engine/Keys.h>
#include <irrKlang.h>

#include "Game.h"
#include "GameFont.h"
#include "StateStart.h"
#include "StateGameplay.h"
#include "StateGameOver.h"
#include "StatePause.h"

/**
*   @brief   Default Constructor.
*/
InvadersGame::InvadersGame()
    : callback_id(-1)
{
}



/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
InvadersGame::~InvadersGame()
{
	inputs->unregisterCallback(callback_id);

    // The state handler must be cleaned up before object renderer, since it removes
    // objects from the object renderer while deleting sprites.
    state_handler = nullptr;
    object_renderer = nullptr;

    audio_engine->stopAllSounds();
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
    if (!initRenderer())
    {
        return false;
    }

    // Input callback function.
	callback_id = inputs->addCallbackFnc(&InvadersGame::input, this);
	
    if (!initFonts())
    {
        return false;
    }

    // Delay object_renderer creation until renderer is initialised.
    object_renderer = std::make_unique<ObjectRenderer>(renderer);

    initStateHandler();

    if (!initAudio())
    {
        return false;
    }

    initGameData();

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
        state_handler->tick(timer.getTimeDifference());
        timer.reset();

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
	return (renderer->exit() || state_handler->shouldExit());
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
    if (object_renderer)
    {
        object_renderer->render();
    }
}



/**
*   @brief   Initialises all the game renderer for drawing sprites and text.
*   @return  True if there were no problems initialising the renderer.
*/
bool InvadersGame::initRenderer()
{
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;

    if (!initAPI())
    {
        return false;
    }

    renderer->setWindowTitle("Invaders - Exercise 1");
    renderer->setClearColour(ASGE::COLOURS::BLACK);

    return true;
}



/**
*   @brief   Initialises all the fonts we need in the game.
*   @details All the fonts needed in the game should be added here.
*   @return  True if there were no problems initialising fonts.
*/
bool InvadersGame::initFonts()
{
    GameFont::fonts[0] = new GameFont
    (renderer->loadFont("..\\..\\Resources\\Fonts\\Alien.ttf", 42), 42, "default");
    renderer->setFont(GameFont::fonts[0]->id);

    if (GameFont::fonts[0]->id == -1)
    {
        return false;
    }

    return true;
}



void InvadersGame::initGameData()
{
    game_data.object_factory = object_renderer.get();
    game_data.audio_engine = audio_engine.get();
}



bool InvadersGame::initAudio()
{
    audio_engine.reset(irrklang::createIrrKlangDevice());

    if (!audio_engine)
    {
        return false;
    }

    audio_engine->setSoundVolume(0.5f);

    return true;
}



/**
*   @brief   Initialises the State Handler for Game.
*   @details The game contains four states: Start, Gameplay, Gameover, Pause.
             Once these states have been registered with the 
             State Handler, the Start state is pushed to the state 
             queue so the game begins at the main menu.
*   @return  void
*/
void InvadersGame::initStateHandler()
{
    state_handler = std::make_unique<StateHandler>();

    state_handler->registerState
        (GameState::START, std::make_unique<StateStart>(game_data));
    state_handler->registerState
        (GameState::GAMEPLAY, std::make_unique<StateGameplay>(game_data));
    state_handler->registerState
        (GameState::GAMEOVER, std::make_unique<StateGameOver>(game_data));
    state_handler->registerState
        (GameState::PAUSE, std::make_unique<StatePause>(game_data));

    state_handler->pushState(GameState::START);
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
void InvadersGame::input(int _key, int _action) const
{
    CommandState command_state = static_cast<CommandState>(_action);

    switch (_key)
    {
        case ASGE::KEYS::KEY_A:
        {
            state_handler->onCommand(Command::MOVE_LEFT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_D:
        {
            state_handler->onCommand(Command::MOVE_RIGHT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_W:
        {
            state_handler->onCommand(Command::MOVE_UP, command_state);
            break;
        }

        case ASGE::KEYS::KEY_S:
        {
            state_handler->onCommand(Command::MOVE_DOWN, command_state);
            break;
        }

        case ASGE::KEYS::KEY_ENTER:
        case ASGE::KEYS::KEY_SPACE:
        {
            state_handler->onCommand(Command::SHOOT, command_state);
            break;
        }

        case ASGE::KEYS::KEY_P:
        {
            state_handler->onCommand(Command::PAUSE, command_state);
            break;
        }

        case ASGE::KEYS::KEY_ESCAPE:
        {
            state_handler->onCommand(Command::QUIT, command_state);
            break;
        }
    }

    // Input debug.
    std::cout << "command: " << static_cast<int>(_key) << " " 
              << static_cast<int>(_action) << std::endl;
}


