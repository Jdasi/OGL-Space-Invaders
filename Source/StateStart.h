#pragma once
#include <memory>
#include <vector>
#include <atomic>
#include <functional>

#include "State.h"

class SpriteObject;
class TextObject;
struct GameData;
struct MenuItem;

/* Start State is initial State of the game, and contains the Main Menu.
 * This State uses a vector of MenuItems to provide options to the player.
 *
 * In this version of the game, only the "Play Game" and "Quit Options" are available.
 *
 * The player returns to this State after proceeding from the Game Over State.
 */
class StateStart : public State
{
public:
    explicit StateStart(GameData& _game_data);
    virtual ~StateStart();

    void onStateEnter() override;
    void onStateLeave() override;

    void tick(float _dt) override;
    void onCommand(const Command _command, const CommandState _command_state) override;

private:
    void initMenuItems();
    void updateMenuSelection() const;

    void cycleIndexUp();
    void cycleIndexDown();
    void executeMenuFunction();

    std::unique_ptr<SpriteObject> logo;
    std::vector<MenuItem> menu_items;
    std::atomic<int> menu_index;
};
