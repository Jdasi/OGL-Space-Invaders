#include "StateStart.h"

#include <iostream>

StateStart::StateStart()
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
    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << std::endl;
}
