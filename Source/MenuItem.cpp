#include "MenuItem.h"

MenuItem::MenuItem(std::unique_ptr<TextObject> _title, std::function<void()> _function)
    : title(std::move(_title))
    , function(_function)
{
}


