#pragma once
#include <string>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
static const int WINDOW_WIDTH                   = 620;
static const int WINDOW_HEIGHT                  = 600;
static const int WINDOW_MARGIN                  = 20;

static constexpr int WINDOW_LEFT_BOUNDARY       = WINDOW_MARGIN;
static constexpr int WINDOW_RIGHT_BOUNDARY      = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
static constexpr int WINDOW_BOTTOM_BOUNDARY     = WINDOW_HEIGHT - WINDOW_MARGIN;

// ======================================================================================
// STATE GAMEPLAY SETTINGS
// ======================================================================================
static const int PLAYER_START_LIVES             = 3;
static const int MAX_DISPLAYED_LIVES            = 8;
static const float PLAYER_SPEED                 = 200.0f;
static const float PLAYER_PROJECTILE_SPEED      = 500.0f;
static const float MEGA_MODE_DURATION           = 3.0f;
static const int MEGA_MODE_THRESHOLD            = 20;
static const int EXTRA_LIFE_THRESHOLD           = 100;

static const int ALIEN_ROWS_MAX                 = 5;
static const int ALIEN_COLUMNS_MAX              = 11;
static const float ALIEN_SIDE_SPEED             = 5.0f;
static const float ALIEN_DOWN_SPEED             = 20.0f;
static const float ALIEN_PROJECTILE_SPEED       = 250.0f;
static const float ALIEN_SHOOT_DELAY_MIN        = 0.05f;
static const float ALIEN_SHOOT_DELAY_MAX        = 1.0f;
static const float ALIEN_SHOOT_DELAY_GROWTH     = 0.035f;
static const float ALIEN_TICK_DELAY_MIN         = 0.1f;
static const float ALIEN_TICK_BLOCK_FACTOR      = 0.15f;

static const int TOP_ALIEN_VALUE                = 5;
static const int MIDDLE_ALIEN_VALUE             = 2;
static const int BOTTOM_ALIEN_VALUE             = 1;

static const int COLLISION_MANAGER_RESERVE      = 100;

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================
static const std::string TEXTURE_PATH           { "..\\..\\Resources\\Textures\\" };
static const std::string AUDIO_PATH             { "..\\..\\Resources\\Audio\\" };

// ======================================================================================
// TEXTURE STRINGS
// ======================================================================================
static const std::string LOGO_IMG               { "logo.png" };
static const std::string PLAYER_IMG             { "player.png" };
static const std::string PROJECTILE_IMG         { "projectile.png" };
static const std::string PROJECTILE_MEGA_IMG    { "projectile_mega.png" };
static const std::string TOP_ALIEN_IMG_0        { "top_alien_0.png" };
static const std::string TOP_ALIEN_IMG_1        { "top_alien_1.png" };
static const std::string MIDDLE_ALIEN_IMG_0     { "middle_alien_0.png" };
static const std::string MIDDLE_ALIEN_IMG_1     { "middle_alien_1.png" };
static const std::string BOTTOM_ALIEN_IMG_0     { "bottom_alien_0.png" };
static const std::string BOTTOM_ALIEN_IMG_1     { "bottom_alien_1.png" };
static const std::string EXPLOSION_IMG          { "explosion.png" };
static const std::string BARRIER_IMG            { "barrier.png" };

// ======================================================================================
// SOUND STRINGS
// ======================================================================================
static const std::string EXTRA_LIFE_CUE         { "extra_life.wav" };
static const std::string PLAYER_SHOT_CUE        { "player_shot.wav" };
static const std::string PLAYER_SHOT_MEGA_CUE   { "player_shot_mega.wav" };
static const std::string ALIEN_SHOT_CUE         { "alien_shot.wav" };
static const std::string PLAYER_DEATH_CUE       { "player_death.wav" };
static const std::string ALIEN_DEATH_CUE        { "alien_death.wav" };
static const std::string POWER_UP_CUE           { "power_up.wav" };
static const std::string POWER_DOWN_CUE         { "power_down.wav" };
static const std::string MENU_CLICK_CUE         { "menu_click.wav" };
static const std::string MENU_ACCEPT_CUE        { "menu_accept.wav" };