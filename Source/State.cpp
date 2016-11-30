#include "State.h"

State::State(GameData& _game_data)
    : game_data(_game_data)
    , handler(nullptr)
    , exit(false)
{
}



StateHandler* State::getHandler() const
{
    return handler;
}



GameData& State::gameData() const
{
    return game_data;
}



bool State::shouldExit() const
{
    return exit;
};



void State::setExit(bool _b)
{
    exit = _b;
}



void State::setHandler(StateHandler* _handler)
{
    handler = _handler;
}