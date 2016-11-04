#pragma once
#include <memory>

#include "State.h"
#include "Vector2.h"

class SpriteObject;
class TextObject;

class StateGameplay : public State
{
public:
    explicit StateGameplay(ObjectFactory& factory);
    virtual ~StateGameplay();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:
    std::shared_ptr<SpriteObject> m_player;
    std::shared_ptr<TextObject> m_text;

    Vector2 m_player_start;
    float m_player_speed;
};
