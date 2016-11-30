#pragma once

/* The available commands in the game.
 * InvadersGame translates specific key presses into one of these Commands, which are
 * then passed through the StateHandler to the current State.
 */
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

/* CommandState isolates the key states from the larger list of keys InvadersGame
 * can detect. This is purely here for convenience.
 */
enum class CommandState : int
{
    RELEASED = 0,
    PRESSED,
    REPEATING
};
