#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ctime>

#include <Engine/Platform.h>

#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    srand(static_cast<unsigned int>(time(nullptr)));

	InvadersGame game;
	if (game.init())
	{
		return game.run();
	}

	// Game failed to initialise.
	return -1;
}