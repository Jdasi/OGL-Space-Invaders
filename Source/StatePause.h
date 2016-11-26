#pragma once
#include <memory>

#include "State.h"

class SpriteObject;
class TextObject;
class ObjectBlock;

class StatePause : public State
{
public:
    explicit StatePause(ObjectFactory& _factory);
    virtual ~StatePause();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    std::unique_ptr<TextObject> pause_title;
    float blink_timer;
    float blink_delay;
};
