#include "StateStart.h"
#include "Game.h"


StateStart::StateStart(ObjectFactory& _factory)
    : State(_factory)
    , menu_index(0)
{
}



StateStart::~StateStart()
{
}




void StateStart::onStateEnter()
{
    menu_index = 0;

    logo = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\logo.png", { 150, 100 });

    initMenuTitles();
    initMenuFunctions();

    updateMenuSelection();
}



void StateStart::onStateLeave()
{
    logo = nullptr;
    menu_titles.clear();
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
            setExit(true);
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



void StateStart::initMenuTitles()
{
    menu_titles.push_back(getObjectFactory().createText("Play Game", { 500, 400 }, 1.0f,
        ASGE::COLOURS::WHITE));

    menu_titles.push_back(getObjectFactory().createText("Quit Game", { 500, 450 }, 1.0f,
        ASGE::COLOURS::WHITE));
}



void StateStart::initMenuFunctions()
{
    menu_functions.push_back([this]() { getHandler()->pushState(GameState::GAMEPLAY); });
    menu_functions.push_back([this]() { setExit(true); });
}



void StateStart::updateMenuSelection() const
{
    for (auto& title : menu_titles)
    {
        title->setColour(ASGE::COLOURS::WHITE);
    }

    menu_titles.at(menu_index)->setColour(ASGE::COLOURS::GREENYELLOW);
}



void StateStart::cycleIndexUp()
{
    if (--menu_index < 0)
    {
        menu_index = menu_titles.size() - 1;
    }

    updateMenuSelection();
}



void StateStart::cycleIndexDown()
{
    if (++menu_index >= menu_titles.size())
    {
        menu_index = 0;
    }

    updateMenuSelection();
}



void StateStart::executeMenuFunction()
{
    menu_functions.at(menu_index)();
}


