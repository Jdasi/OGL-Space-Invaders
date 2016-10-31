#include "StatePause.h"

StatePause::StatePause(InvadersGame& game)
    : State(game)
{
}

StatePause::~StatePause()
{
}

void StatePause::onStateEnter()
{
}

void StatePause::onStateLeave()
{
}

void StatePause::tick()
{
}

void StatePause::onCommand(const Command c, const CommandState s)
{
}