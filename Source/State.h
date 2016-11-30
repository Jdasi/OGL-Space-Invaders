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
    virtual ~State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    // Typically handles the re-initialisation of a state.
    virtual void onStateEnter() = 0;
    // Typically handles the deletion or hiding of objects.
    virtual void onStateLeave() = 0;

    // All state loop behaviour should be contained here.
    virtual void tick(float _dt) = 0;
    // All input handling should be contained here.
    virtual void onCommand(const Command _command, const CommandState _command_state) = 0;

protected:
    explicit State(GameData& _game_data);

    StateHandler* getHandler() const;
    GameData& gameData() const;

    bool shouldExit() const;
    void setExit(bool _b);

private:
    void setHandler(StateHandler* _handler);

    GameData& game_data;
    StateHandler* handler;
    bool exit;
};
