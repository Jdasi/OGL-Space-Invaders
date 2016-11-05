#pragma once
#include "Commands.h"

class StateHandler;
class ObjectFactory;

class State
{
friend class StateHandler;

public:
    explicit State(ObjectFactory& factory)
        : m_factory(factory)
        , m_handler(nullptr)
    {
    }

    virtual ~State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;
    virtual void tick(float dt) = 0;

    virtual void onCommand(const Command c, const CommandState s) = 0;

protected:
    StateHandler* getHandler() const
    {
        return m_handler;
    }

    ObjectFactory& getObjectFactory() const
    {
        return m_factory;
    }

private:
    void setHandler(StateHandler* handler)
    {
        m_handler = handler;
    }

    ObjectFactory& m_factory;
    StateHandler* m_handler;
};
