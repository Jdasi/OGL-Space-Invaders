#pragma once
#include <memory>
#include <vector>
#include <atomic>
#include <functional>

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
    void initMenuTitles();
    void initMenuFunctions();
    void updateMenuSelection() const;

    void cycleIndexUp();
    void cycleIndexDown();
    void executeMenuFunction();

    std::unique_ptr<SpriteObject> logo;

    std::vector<std::unique_ptr<TextObject>> menu_titles;
    std::vector<std::function<void()>> menu_functions;
    std::atomic<int> menu_index;
};
