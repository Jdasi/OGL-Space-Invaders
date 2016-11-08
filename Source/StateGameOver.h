#pragma once
#include "State.h"

class StateGameOver : public State
{
public:
    explicit StateGameOver(ObjectFactory& _factory);
    virtual ~StateGameOver();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:

};
