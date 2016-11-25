#include "StateStart.h"
#include "Game.h"


StateStart::StateStart(ObjectFactory& _factory)
    : State(_factory)
{
}



StateStart::~StateStart()
{
}



void StateStart::onStateEnter()
{
    logo = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\logo.png", { 150, 100 });
}



void StateStart::onStateLeave()
{
    logo = nullptr;
}



void StateStart::tick(float _dt)
{
}



void StateStart::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::SHOOT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            getHandler()->pushState(GameState::GAMEPLAY);
        }
    }

    if (_command == Command::QUIT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            setExit(true);
        }
    }
}


