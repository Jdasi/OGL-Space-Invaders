#pragma once
#include <functional>
#include <memory>

#include "TextObject.h"

struct MenuItem
{
    MenuItem(std::unique_ptr<TextObject> _title, std::function<void()> _function);

    std::unique_ptr<TextObject> title;
    std::function<void()> function;
};