#pragma once
#include <memory>

#include "State.h"

class SpriteObject;
class TextObject;

class StateStart : public State
{
public:
    explicit StateStart(ObjectFactory& factory);
    virtual ~StateStart();

    void onStateEnter() override;
    void onStateLeave() override;
    void tick() override;
    void onCommand(const Command c, const CommandState s) override;

private:
    std::shared_ptr<SpriteObject> m_logo;
    std::shared_ptr<TextObject> m_text;
};
