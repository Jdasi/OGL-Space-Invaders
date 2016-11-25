#include "StateGameOver.h"
#include "Game.h"

StateGameOver::StateGameOver(ObjectFactory& _factory)
    : State(_factory)
    , gameover_title(nullptr)
{
}



StateGameOver::~StateGameOver()
{
}



void StateGameOver::onStateEnter()
{
    gameover_title = getObjectFactory().createText("Game Over", { 100, 100 }, 1.0f, ASGE::COLOURS::ALICEBLUE);
}



void StateGameOver::onStateLeave()
{
    gameover_title = nullptr;
}



void StateGameOver::tick(float _dt)
{
}



void StateGameOver::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::SHOOT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            getHandler()->pushState(GameState::START);
        }
    }
}


