#pragma once
#include <irrKlang.h>

#include "ObjectFactory.h"

struct GameData
{
    GameData();

    irrklang::ISoundEngine* audio_engine;
    ObjectFactory* object_factory;
    int score;
    int highest_score_multiplier;
};
