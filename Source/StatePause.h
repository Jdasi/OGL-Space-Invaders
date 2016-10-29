#pragma once

#include "State.h"

class StatePause : public State
{
public:
    StatePause(std::shared_ptr<ASGE::Renderer>& r);
    ~StatePause();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:

};
