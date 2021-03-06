#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Engine/Platform.h>

#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	InvadersGame game;
	if (game.init())
	{
		return game.run();
	}

	// Game failed to initialise.
	return -1;
}