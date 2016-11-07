#pragma once
#include <memory>

#include "State.h"

class SpriteObject;
class TextObject;

class StateStart : public State
{
public:
    explicit StateStart(ObjectFactory& _factory);
    virtual ~StateStart();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    std::shared_ptr<SpriteObject> logo;
    std::shared_ptr<TextObject> text;
};
