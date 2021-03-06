#pragma once
#include <atomic>

#include <irrKlang.h>

#include "ObjectFactory.h"

/* All information that the whole game should have access to is included here.
 * This allows for important information to be passed through the StateHandler to
 * the current State without the need for additional member references.
 */
struct GameData
{
    GameData();

    irrklang::ISoundEngine* audio_engine;
    ObjectFactory* object_factory;
    unsigned int score;
    unsigned int highest_score_multiplier;
    std::atomic<bool> exit;
};
