#pragma once
#include "Commands.h"

class StateHandler;
class ObjectFactory;
class SpriteObject;

class State
{
friend class StateHandler;

public:
    explicit State(ObjectFactory& _factory)
        : factory(_factory)
        , handler(nullptr)
        , exit(false)
    {
    }

    virtual ~State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;
    virtual void tick(float _dt) = 0;

    virtual void onCommand(const Command _command, const CommandState _command_state) = 0;

protected:
    StateHandler* getHandler() const
    {
        return handler;
    }

    ObjectFactory& getObjectFactory() const
    {
        return factory;
    }

    bool shouldExit() const
    {
        return exit;
    };

    void setExit(bool b)
    {
        exit = b;
    }

private:
    void setHandler(StateHandler* _handler)
    {
        handler = _handler;
    }

    ObjectFactory& factory;
    StateHandler* handler;
    bool exit;
};
