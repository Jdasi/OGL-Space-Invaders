#include "StateGameOver.h"
#include "Game.h"
#include "GameData.h"

StateGameOver::StateGameOver(GameData& _game_data)
    : State(_game_data)
    , gameover_title(nullptr)
    , blink_timer(0)
    , blink_delay(0.3f)
    , returning(false)
{
}



StateGameOver::~StateGameOver() = default;



void StateGameOver::onStateEnter()
{
    initTitles();
}



void StateGameOver::onStateLeave()
{
    gameover_title = nullptr;
    instruction_title = nullptr;

    score_title = nullptr;
    score_text = nullptr;

    score_multiplier_title = nullptr;
    score_multiplier_text = nullptr;
}



void StateGameOver::tick(float _dt)
{
    blinkInstructionTitle(_dt);
    handleReturnCommand();
}



void StateGameOver::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::QUIT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            returning = true;
        }
    }
}



void StateGameOver::initTitles()
{
    gameover_title = gameData().object_factory->createText("Game Over", { 350, 300 },
        2.0f, ASGE::COLOURS::WHITE);

    // Score TextObjects.
    score_title = gameData().object_factory->createText("Final Score:", { 400, 350 },
        0.7f, ASGE::COLOURS::DARKORANGE);

    score_text = gameData().object_factory->createText(std::to_string(gameData().score),
        score_title->getPosition(), 0.7f, ASGE::COLOURS::WHITE);
    score_text->modifyPosition({ 300, 0 });

    // Score Multiplier TextObjects.
    score_multiplier_title = gameData().object_factory->createText("Highest Multiplier:",
        { 245, score_title->getPosition().y + 40 }, 0.7f, ASGE::COLOURS::DARKORANGE);

    score_multiplier_text = gameData().object_factory->createText(
        std::to_string(gameData().highest_score_multiplier), { 0, 0 }, 0.7f,
        ASGE::COLOURS::WHITE);
    score_multiplier_text->setPosition({ score_text->getPosition().x,
        score_multiplier_title->getPosition().y });

    // Instruction title.
    instruction_title = gameData().object_factory->createText(
        "Press 'ESC' to return to the Main Menu", { 160, 650 }, 0.75f,
        ASGE::COLOURS::YELLOWGREEN);
}



void StateGameOver::blinkInstructionTitle(float _dt)
{
    blink_timer += _dt;

    if (blink_timer >= blink_delay)
    {
        blink_timer = 0;

        instruction_title->setVisible(!instruction_title->isVisible());
    }
}



void StateGameOver::handleReturnCommand()
{
    if (returning)
    {
        returning = false;

        getHandler()->pushState(GameState::START);
        gameData().audio_engine->play2D((AUDIO_PATH + MENU_ACCEPT_CUE).c_str(), false);
    }
}


