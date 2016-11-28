#include "StatePause.h"
#include "Game.h"

StatePause::StatePause(GameData& _game_data)
    : State(_game_data)
    , blink_timer(0)
    , blink_delay(0.1f)
{
}



StatePause::~StatePause()
{
}



void StatePause::onStateEnter()
{
    pause_title = gameData().object_factory->createText("PAUSED", { 550, 300 }, 1.0f, 
        ASGE::COLOURS::ALICEBLUE);
}



void StatePause::onStateLeave()
{
    pause_title = nullptr;
}



void StatePause::tick(float _dt)
{
    blink_timer += _dt;

    if (blink_timer >= blink_delay)
    {
        blink_timer = 0;

        pause_title->setVisible(!pause_title->isVisible());
    }
}



void StatePause::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::PAUSE)
    {
        if (_command_state == CommandState::PRESSED)
        {
            getHandler()->pushState(GameState::GAMEPLAY);
        }
    }
}


