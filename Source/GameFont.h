#pragma once

/**
**   A font that's loaded in the game engine.
*/

struct GameFont
{
	GameFont(int _id, int _size, char* _name);

	int id = 0;             /**< The ID assigned to the font from the graphics engine. */
	int size = 0;           /**< The size of the font that was loaded. */
	char* name = "";        /**< The name of the font. */
	static GameFont* fonts[5]; /**< Nasty way of globalising five loaded fonts. */
};
