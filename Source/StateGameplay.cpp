#include "StateGameplay.h"

StateGameplay::StateGameplay(std::shared_ptr<ASGE::Renderer>& r)
    : State(r)
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