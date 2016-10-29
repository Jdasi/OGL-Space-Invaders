#pragma once
#include <Engine/Sprite.h>

#include "Commands.h"

class StateHandler;

class State
{
friend class StateHandler;

public:
    State() = default;
    virtual ~State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;
    virtual void tick() = 0;

    virtual void onCommand(const Command c, const CommandState s) = 0;

protected:
    StateHandler* getHandler() const
    {
        return m_handler;
    }

private:
    void setHandler(StateHandler* handler)
    {
        m_handler = handler;
    }

    StateHandler* m_handler;
};
