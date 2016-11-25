#pragma once
#include <memory>

#include "State.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

class StateGameOver : public State
{
public:
    explicit StateGameOver(ObjectFactory& _factory);
    virtual ~StateGameOver();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    std::unique_ptr<TextObject> gameover_title;
};
