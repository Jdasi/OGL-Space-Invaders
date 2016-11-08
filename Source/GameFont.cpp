#include "GameFont.h"

/**
*   @brief   Constructor.
*   @details Sets the font index, it's size and description. 
*            Make sure the IDX is valid otherwise the font will 
*            fail to render. 
*	@param   idx is The font index in the Graphics Engine.
*   @param   n is the name or description of the font.
*   @param   sz is the size of the loaded font.
*   @return  void
*/
GameFont::GameFont(int _id, int _size, char* _name)
    : id(_id)
    , size(_size)
    , name(_name)
{
}

// Static variables for fonts.
GameFont* GameFont::fonts[5];
