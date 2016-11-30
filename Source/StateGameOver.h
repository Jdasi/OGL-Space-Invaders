#pragma once
#include <memory>
#include <atomic>

#include "State.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

/* Game Over State triggers after the Gameplay State concludes.
 * The player's final score and highest score multiplier is displayed here.
 * A blinking TextObject is used to instruct the player on how to proceed.
 */
class StateGameOver : public State
{
public:
    explicit StateGameOver(GameData& _game_data);
    virtual ~StateGameOver();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    void initTitles();
    void blinkInstructionTitle(float _dt);
    void handleReturnCommand();

    std::unique_ptr<TextObject> gameover_title;
    std::unique_ptr<TextObject> instruction_title;

    std::unique_ptr<TextObject> score_title;
    std::unique_ptr<TextObject> score_multiplier_title;

    std::unique_ptr<TextObject> score_text;
    std::unique_ptr<TextObject> score_multiplier_text;

    float blink_timer;
    float blink_delay;

    std::atomic<bool> returning;
};
