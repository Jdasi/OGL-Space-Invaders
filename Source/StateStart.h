#pragma once

#include "State.h"

class StateStart : public State
{
public:
    StateStart(InvadersGame& game);
    ~StateStart();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};
