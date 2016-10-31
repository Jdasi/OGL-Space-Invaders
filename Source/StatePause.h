#pragma once

#include "State.h"

class StatePause : public State
{
public:
    explicit StatePause(ObjectFactory& factory);
    virtual ~StatePause();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};
