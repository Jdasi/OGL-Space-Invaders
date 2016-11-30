#pragma once
#include <memory>

#include "State.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

/* Pause State is triggered when the player pauses the game while in State Gameplay.
 * A blinking TextObject is used to notify the player that the game is paused.
 * The player can return to the triggering Gameplay state by issuing the Pause command.
 */
class StatePause : public State
{
public:
    explicit StatePause(GameData& _game_data);
    virtual ~StatePause();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    void blinkPauseTitle(float _dt);

    std::unique_ptr<TextObject> pause_title;
    float blink_timer;
    float blink_delay;
};
