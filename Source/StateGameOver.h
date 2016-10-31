#pragma once

#include "State.h"

class StateGameOver : public State
{
public:
    explicit StateGameOver(ObjectFactory& factory);
    virtual ~StateGameOver();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};