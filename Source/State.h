#pragma once
#include "Commands.h"

class StateHandler;
class SpriteObject;
struct GameData;

/* Abstract base class for all States used in the game.
 * The StateHandler communicates with all registered objects that derive from this class.
 * 
 * State has a reference to the GameData package so important game information can be
 * updated and passed between states with ease.
 *
 * State also has a reference to its StateHandler to allow for State transitions from
 * within States.
 */
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
