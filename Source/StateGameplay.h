#pragma once

#include "State.h"

class StateGameplay : public State
{
public:
    explicit StateGameplay(ObjectFactory& factory);
    virtual ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};
