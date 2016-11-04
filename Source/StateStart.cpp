#include <iostream>

#include "StateStart.h"
#include "Game.h"
#include "Constants.h"

StateStart::StateStart(ObjectFactory& factory)
    : State(factory)
{
}



StateStart::~StateStart()
{
}



void StateStart::onStateEnter()
{
}



void StateStart::onStateLeave()
{
}



void StateStart::tick()
{
}



void StateStart::onCommand(const Command c, const CommandState s)
{
}


