#include "StateGameplay.h"

StateGameplay::StateGameplay(InvadersGame& game)
    : State(game)
{
}

StateGameplay::~StateGameplay()
{
}

void StateGameplay::onStateEnter()
{
}

void StateGameplay::onStateLeave()
{
}

void StateGameplay::tick()
{
}

void StateGameplay::onCommand(const Command c, const CommandState s)
{
}