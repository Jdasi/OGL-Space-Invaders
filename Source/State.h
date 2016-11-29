#pragma once
#include "Commands.h"

class StateHandler;
class SpriteObject;
struct GameData;

class State
{
friend class StateHandler;

public:
    explicit State(GameData& _game_data);
    virtual ~State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;

    virtual void tick(float _dt) = 0;
    virtual void onCommand(const Command _command, const CommandState _command_state) = 0;

protected:
    StateHandler* getHandler() const;
    GameData& gameData() const;

    bool shouldExit() const;
    void setExit(bool b);

private:
    void setHandler(StateHandler* _handler);

    GameData& game_data;
    StateHandler* handler;
    bool exit;
};
