#include "StatePause.h"
#include "Game.h"

StatePause::StatePause(ObjectFactory& _factory)
    : State(_factory)
{
}



StatePause::~StatePause()
{
}



void StatePause::onStateEnter()
{
    pause_title = getObjectFactory().createText("PAUSED", { 100, 100 }, 1.0f, ASGE::COLOURS::ALICEBLUE);
}



void StatePause::onStateLeave()
{
    pause_title = nullptr;
}



void StatePause::tick(float _dt)
{
}



void StatePause::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::PAUSE)
    {
        if (_command_state == CommandState::PRESSED)
        {
            getHandler()->pushState(GameState::GAMEPLAY);
        }
    }
}


