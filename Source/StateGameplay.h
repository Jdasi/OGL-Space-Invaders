#pragma once

#include "State.h"

class StateGameplay : public State
{
public:
    StateGameplay(std::shared_ptr<ASGE::Renderer>& r);
    ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};
