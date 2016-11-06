#pragma once
#include <memory>
#include <vector>

#include "State.h"
#include "Vector2.h"

class SpriteObject;
class TextObject;

enum class MoveDirection
{
    LEFT,
    RIGHT,
    NONE
};

class StateGameplay : public State
{
public:
    explicit StateGameplay(ObjectFactory& factory);
    virtual ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick(float dt) override;
    void onCommand(const Command c, const CommandState s) override;

private:
    std::shared_ptr<SpriteObject> m_player;
    std::shared_ptr<SpriteObject> m_player_projectile;

    std::shared_ptr<TextObject> m_text;

    std::vector<std::shared_ptr<SpriteObject>> m_aliens;

    Vector2 m_player_start;
    int m_player_speed;
    int m_player_projectile_speed;
    bool m_player_shooting;
    MoveDirection m_player_direction;
};
