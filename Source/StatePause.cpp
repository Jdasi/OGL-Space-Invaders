#include "StatePause.h"

StatePause::StatePause(std::shared_ptr<ASGE::Renderer>& r)
    : State(r)
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