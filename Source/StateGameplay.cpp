#include <iostream>

#include "StateGameplay.h"
#include "Game.h"
#include "Constants.h"
#include "ObjectBlock.h"

StateGameplay::StateGameplay(ObjectFactory& factory)
    : State(factory)
    , m_player_speed(400)
    , m_player_projectile_speed(600)
    , m_player_shooting(false)
    , m_player_direction(MoveDirection::NONE)
    , m_alien_tick_delay(1.0f)
    , m_alien_timer(0)
    , m_alien_side_speed(400)
    , m_alien_down_speed(800)
    , m_alien_projectile_speed(600)
    , m_aliens_direction(MoveDirection::RIGHT)
{
}



StateGameplay::~StateGameplay()
{
}



void StateGameplay::onStateEnter()
{
    initPlayer();
    initHUD();
    initAliens();
}



void StateGameplay::onStateLeave()
{
}



void StateGameplay::tick(float dt)
{
    playerShoot();
    updatePlayerProjectile(dt);
    movePlayer(dt);
    updateAliensDirection(dt);
}



void StateGameplay::onCommand(const Command c, const CommandState s)
{
    if (c == Command::MOVE_LEFT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_direction = MoveDirection::LEFT;
        }
        else
        {
            m_player_direction = MoveDirection::NONE;
        }
    }

    if (c == Command::MOVE_RIGHT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_direction = MoveDirection::RIGHT;
        }
        else
        {
            m_player_direction = MoveDirection::NONE;
        }
    }

    if (c == Command::SHOOT)
    {
        if (s == CommandState::PRESSED || s == CommandState::REPEATING)
        {
            m_player_shooting = true;
        }
        else
        {
            m_player_shooting = false;
        }
    }
}



void StateGameplay::initPlayer()
{
    Vector2 player_start{ WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100 };
    m_player = getObjectFactory().createSprite("..\\..\\Resources\\Textures\\player.png", player_start);
}



void StateGameplay::initHUD()
{
    m_score_text = getObjectFactory().createText("Score:", { 20, 30 }, 0.7f, ASGE::COLOURS::DARKORANGE);
}



void StateGameplay::initAliens()
{
    Vector2 alien_start{ 250, 100 };
    int max_columns = 11;
    int padding_x = 10;
    int padding_y = 20;

    m_aliens = std::make_unique<ObjectBlock>(alien_start, max_columns, padding_x, padding_y);

    std::string alien_img = "..\\..\\Resources\\Textures\\top_alien_0.png";
    for (int i = 0; i < max_columns; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    int double_row = max_columns * 2;
    alien_img = "..\\..\\Resources\\Textures\\middle_alien_0.png";
    for (int i = 0; i < double_row; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    alien_img = "..\\..\\Resources\\Textures\\bottom_alien_0.png";
    for (int i = 0; i < double_row; ++i)
    {
        m_aliens->addObject(getObjectFactory().createSprite(alien_img, alien_start));
    }

    m_aliens->updateLayout();
}



void StateGameplay::playerShoot()
{
    if (m_player_shooting && !m_player_projectile)
    {
        m_player_projectile = getObjectFactory().createSprite(
            "..\\..\\Resources\\Textures\\projectile.png",
            { m_player->getPosition().x + (m_player->getSize().x / 2),
            m_player->getPosition().y - 5 });
    }
}



void StateGameplay::updatePlayerProjectile(float dt)
{
    if (m_player_projectile)
    {
        m_player_projectile->modifyPosition(0, -m_player_projectile_speed * dt);

        // Destroy the projectile if it collides with something or hits the top edge.
        if (m_aliens->collisionTest(m_player_projectile) ||
            m_player_projectile->getPosition().y <= WINDOW_MARGIN)
        {
            m_player_projectile = nullptr;
        }
    }
}



void StateGameplay::movePlayer(float dt) const
{
    if (m_player_direction == MoveDirection::LEFT)
    {
        if (m_player->getPosition().x > WINDOW_LEFT_BOUNDARY)
        {
            m_player->modifyPosition(-m_player_speed * dt, 0);
        }
    }

    if (m_player_direction == MoveDirection::RIGHT)
    {
        if (m_player->getPosition().x + m_player->getSize().x < WINDOW_RIGHT_BOUNDARY)
        {
            m_player->modifyPosition(m_player_speed * dt, 0);
        }
    }
}



void StateGameplay::updateAliensDirection(float dt)
{
    if (m_alien_timer < m_alien_tick_delay)
    {
        m_alien_timer += dt;
    }
    else
    {
        m_alien_timer = 0;

        // Move aliens down at left edge.
        bool left_edge_hit = m_aliens->getEdgeLeft() <= WINDOW_LEFT_BOUNDARY;
        if (left_edge_hit && m_aliens_direction != MoveDirection::DOWN)
        {
            m_aliens_direction = MoveDirection::DOWN;
        }
        else if (left_edge_hit)
        {
            m_aliens_direction = MoveDirection::RIGHT;
        }

        // Move aliens down at right edge.
        bool right_edge_hit = m_aliens->getEdgeRight() >= WINDOW_RIGHT_BOUNDARY;
        if (right_edge_hit && m_aliens_direction != MoveDirection::DOWN)
        {
            m_aliens_direction = MoveDirection::DOWN;
        }
        else if (right_edge_hit)
        {
            m_aliens_direction = MoveDirection::LEFT;
        }

        moveAliens(dt);
    }
}



void StateGameplay::moveAliens(float dt) const
{
    if (m_aliens_direction == MoveDirection::DOWN)
    {
        m_aliens->moveBlock(0, m_alien_down_speed * dt);
    }

    if (m_aliens_direction == MoveDirection::LEFT)
    {
        m_aliens->moveBlock(-m_alien_side_speed * dt, 0);
    }

    if (m_aliens_direction == MoveDirection::RIGHT)
    {
        m_aliens->moveBlock(m_alien_side_speed * dt, 0);
    }
}


