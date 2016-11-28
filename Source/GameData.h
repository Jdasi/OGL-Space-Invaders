#pragma once
#include <irrKlang.h>

#include "ObjectFactory.h"

struct GameData
{
    irrklang::ISoundEngine* audio_engine = nullptr;
    ObjectFactory* object_factory = nullptr;
    int score = 0;
    int highest_score_multiplier = 0;
};
