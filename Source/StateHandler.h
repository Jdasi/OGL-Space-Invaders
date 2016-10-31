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
        : m_current_state(nullptr)
    {
    }

    virtual ~StateHandler() = default;

    void triggerState(const GameState state)
    {
        auto result = m_state_map.find(state);

        if (result == m_state_map.end())
        {
            throw std::runtime_error("Unknown state.");
        }

        if (m_current_state)
        {
            m_current_state->onStateLeave();
        }

        m_current_state = result->second.get();

        m_current_state->onStateEnter();
    }

    void tick() const
    {
        if (m_current_state)
        {
            m_current_state->tick();
        }
    }

    void onCommand(const Command c, const CommandState s) const
    {
        if (m_current_state)
        {
            m_current_state->onCommand(c, s);
        }
    }

protected:
    void registerState(GameState game_state, std::unique_ptr<State> state)
    {
        state->setHandler(this);
        m_state_map[game_state] = std::move(state);
    }

private:
    std::map<GameState, std::unique_ptr<State>> m_state_map;
    State* m_current_state;
};
