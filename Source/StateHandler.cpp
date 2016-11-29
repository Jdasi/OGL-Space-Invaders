#include "StateHandler.h"

StateHandler::StateHandler()
    : current_state(nullptr)
{
}



void StateHandler::tick(float _dt)
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



void StateHandler::onCommand(const Command _command, const CommandState _command_state) 
    const
{
    if (current_state)
    {
        current_state->onCommand(_command, _command_state);
    }
}



void StateHandler::registerState(GameState _game_state, std::unique_ptr<State> _state)
{
    _state->setHandler(this);
    state_map[_game_state] = std::move(_state);
}



void StateHandler::pushState(const GameState _state)
{
    std::lock_guard<std::mutex> guard(states_to_trigger_mutex);
    states_to_trigger.push(_state);
}



bool StateHandler::shouldExit() const
{
    if (current_state)
    {
        return current_state->shouldExit();
    }

    return false;
}



void StateHandler::triggerState(const GameState _state)
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


