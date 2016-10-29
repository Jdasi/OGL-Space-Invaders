#pragma once

enum class Command
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    SHOOT,
    PAUSE,
    QUIT
};

enum class CommandState
{
    RELEASED,
    PRESSED
};
