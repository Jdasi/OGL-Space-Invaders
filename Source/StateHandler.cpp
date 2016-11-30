#include "StateHandler.h"

StateHandler::StateHandler()
    : current_state(nullptr)
{
}



void StateHandler::tick(float _dt)
{
    /* Because input is on a different thread and we sometimes push states from input, 
     * we need some protection against two threads simultaneously trying to access the 
     * same data.
     *
     * When a thread triggers the lock guard, all other threads must wait for that same
     * thread to unlock the guard again before any thread can access the data.
     *
     * Without this, the program has a chance to crash seemingly at random when two
     * threads access the same data while it is potentially changing.
     */
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



/* Adds a state transition to the queue.
 * The StateHandler's tick will carry out the transition when possible.
 */
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



/* Tries to find the passed GameState. Throws an exception if it can't.
 * If the GameState is found, the current state changes to the new state.
 * The appropriate onStateLeave and onStateEnter functions are called for each state.
 */
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


