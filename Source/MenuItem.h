#pragma once
#include <functional>
#include <memory>

#include "TextObject.h"

/* A struct which contains a TextObject and corresponding Function pointer to execute
 * when the player selects the item from a menu.
 */
struct MenuItem
{
    MenuItem(std::unique_ptr<TextObject> _title, std::function<void()> _function);

    std::unique_ptr<TextObject> title;
    std::function<void()> function;
};