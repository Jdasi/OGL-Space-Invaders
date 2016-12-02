#include "StateStart.h"
#include "Game.h"
#include "MenuItem.h"

StateStart::StateStart(GameData& _game_data)
    : State(_game_data)
    , menu_index(0)
{
}



StateStart::~StateStart() = default;



void StateStart::onStateEnter()
{
    menu_index = 0;

    logo = gameData().object_factory->createSprite(
        TEXTURE_PATH + LOGO_IMG, { 150, 100 });

    initMenuItems();

    updateMenuSelection();
}



void StateStart::onStateLeave()
{
    logo = nullptr;
    menu_items.clear();
}



void StateStart::tick(float _dt)
{
}



void StateStart::onCommand(const Command _command, const CommandState _command_state)
{
    if (_command == Command::SHOOT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            executeMenuFunction();
        }
    }

    if (_command == Command::QUIT)
    {
        if (_command_state == CommandState::PRESSED)
        {
            gameData().exit = true;
        }
    }

    if (_command == Command::MOVE_UP)
    {
        if (_command_state == CommandState::PRESSED)
        {
            cycleIndexUp();
        }
    }

    if (_command == Command::MOVE_DOWN)
    {
        if (_command_state == CommandState::PRESSED)
        {
            cycleIndexDown();
        }
    }
}



void StateStart::initMenuItems()
{
    menu_items.push_back(MenuItem(gameData().object_factory->createText(
        "Play Game", { 500, 400 }, 1.0f, ASGE::COLOURS::WHITE),
        [this]() { getHandler()->pushState(GameState::GAMEPLAY); }));

    menu_items.push_back(MenuItem(gameData().object_factory->createText(
        "Quit Game", { 500, 450 }, 1.0f, ASGE::COLOURS::WHITE),
        [this]() { gameData().exit = true; }));
}



void StateStart::updateMenuSelection() const
{
    gameData().audio_engine->play2D((AUDIO_PATH + MENU_CLICK_CUE).c_str(), false);

    for (auto& item : menu_items)
    {
        item.title->setColour(ASGE::COLOURS::WHITE);
    }

    menu_items.at(menu_index).title->setColour(ASGE::COLOURS::GREENYELLOW);
}



void StateStart::cycleIndexUp()
{
    menu_index = ++menu_index % menu_items.size();

    updateMenuSelection();
}



void StateStart::cycleIndexDown()
{
    menu_index = --menu_index % menu_items.size();

    updateMenuSelection();
}



void StateStart::executeMenuFunction()
{
    gameData().audio_engine->play2D((AUDIO_PATH + MENU_ACCEPT_CUE).c_str(), false);

    menu_items.at(menu_index).function();
}


