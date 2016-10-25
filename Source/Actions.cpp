#include "Actions.h"

/**< The next action to be processed as determined by user input. */
std::atomic<GameAction> game_action = GameAction::NONE;