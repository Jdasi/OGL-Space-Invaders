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

class StateHandler
{
public:
    StateHandler()
        : current_state(nullptr)
    {
    }

    virtual ~StateHandler() = default;

    void pushState(const GameState _state)
    {
        std::lock_guard<std::mutex> guard(states_to_trigger_mutex);
        states_to_trigger.push(_state);
    }

    void tick(float _dt)
    {
        {
            std::lock_guard<std::mutex> guard(states_to_trigger_mutex);
            if (!states_to_trigger.empty())
            {
                triggerState(states_to_trigger.back());
                states_to_trigger.pop();
            }
        }

        if (current_state)
        {
            current_state->tick(_dt);
        }
    }

    void onCommand(const Command _command, const CommandState _command_state) const
    {
        if (current_state)
        {
            current_state->onCommand(_command, _command_state);
        }
    }

    void registerState(GameState _game_state, std::unique_ptr<State> _state)
    {
        _state->setHandler(this);
        state_map[_game_state] = std::move(_state);
    }

    bool shouldExit() const
    {
        if (current_state)
        {
            return current_state->shouldExit();
        }

        return false;
    }

private:
    void triggerState(const GameState _state)
    {
        auto result = state_map.find(_state);

        if (result == state_map.end())
        {
            throw std::runtime_error("Unknown state.");
        }

        if (current_state)
        {
            current_state->onStateLeave();
        }

        current_state = result->second.get();

        current_state->onStateEnter();
    }

    std::map<GameState, std::unique_ptr<State>> state_map;
    State* current_state;
    std::queue<GameState> states_to_trigger;
    std::mutex states_to_trigger_mutex;
};
