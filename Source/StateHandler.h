#pragma once
#include <map>
#include <memory>

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

    void tick(float _dt) const
    {
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

private:
    std::map<GameState, std::unique_ptr<State>> state_map;
    State* current_state;
};
