#pragma once
#include "State.h"

class StatePause : public State
{
public:
    explicit StatePause(ObjectFactory& _factory);
    virtual ~StatePause();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick(float dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:

};
