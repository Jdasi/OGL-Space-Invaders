#pragma once
#include <map>
#include <memory>
#include <queue>
#include <mutex>

#include "State.h"

enum class GameState
{
    START,
    GAMEPLAY,
    GAMEOVER,
    PAUSE
};

/* Handles all of the State transitions in the game.
 * InvadersGame registers the States it is going to use with the StateHandler, and then
 * triggers the initial State, choosing from those it registered.
 *
 * Tick and onCommand calls from InvadersGame are then passed through the StateHandler
 * to the current state. This ensures only the current state's behaviour occurs,
 * while the rest are "frozen".
 */
class StateHandler
{
public:
    StateHandler();
    virtual ~StateHandler() = default;

    void tick(float _dt);
    void onCommand(const Command _command, const CommandState _command_state) const;

    void registerState(GameState _game_state, std::unique_ptr<State> _state);
    void pushState(const GameState _state);

    bool shouldExit() const;

private:
    void triggerState(const GameState _state);

    std::map<GameState, std::unique_ptr<State>> state_map;
    State* current_state;
    std::queue<GameState> states_to_trigger;
    std::mutex states_to_trigger_mutex;
};
