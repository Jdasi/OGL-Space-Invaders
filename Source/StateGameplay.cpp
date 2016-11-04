#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_start({ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 })
    , m_player_speed(WINDOW_WIDTH / 100)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    m_player = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\Player.png", m_player_start);
    m_text = getObjectFactory().createText("Test", { 375, 325 }, 1.0f, ASGE::COLOURS::DARKORANGE);
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick()
{
}



void StateGameplay::onCommand(const Command c, const CommandState s)
{
    if (c == Command::MOVE_LEFT && s == CommandState::PRESSED &&
        m_player->getPosition().x > WINDOW_LEFT_BOUNDARY)
    {
        m_player->modifyPosition({ -m_player_speed, 0 });
    }

    if (c == Command::MOVE_RIGHT && s == CommandState::PRESSED &&
        m_player->getPosition().x + m_player->getSize().x < WINDOW_RIGHT_BOUNDARY)
    {
        m_player->modifyPosition({ m_player_speed, 0 });
    }

    std::cout << "command: " << static_cast<int>(c) << " " << static_cast<int>(s) << " " << m_player->getPosition().x << ", " << m_player->getPosition().y << std::endl;
}


