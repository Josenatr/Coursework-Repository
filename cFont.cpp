/*
==========================================================================
cFont.cpp
==========================================================================
*/
//#pragma warning ( disable : 4996 )
#include "cFont.h"
using namespace std;

/*
==========================================================================
Default Constructor
==========================================================================
*/
cFont::cFont()
{
	theFont = NULL;
}
/*
==========================================================================
Destructor
==========================================================================
*/
cFont::~cFont()
{
	TTF_CloseFont(theFont);
}
/*
==========================================================================
Load the desired font
==========================================================================
*/
bool cFont::loadFont(LPCSTR fontFileName, int fontSize)
{
	theFont = TTF_OpenFont(fontFileName, fontSize);

	if (theFont == NULL) //this if statement checks to see if a font is present
	{
		cout << " Failed to load font : " << SDL_GetError() << endl; //this error message shows when no font can be found
		delete theFont;
		return false;
	}
	cout << "Font '" << fontFileName << "' loaded successfully" << endl; //this message shows if the font selected has been loaded successfully
	return true;
}

/*
==========================================================================
get the pointer to the font
==========================================================================
*/
TTF_Font* cFont::getFont()
{
	return theFont;
}

/*
==========================================================================
Render the text using the desired font
==========================================================================
*/
SDL_Texture* cFont::createTextTexture(SDL_Renderer* theRenderer, LPCSTR text, textType txtType, SDL_Color txtColour, SDL_Color txtBkgd)
{
	SDL_Texture* theTxtTexture = NULL;
	SDL_Surface* theTxtSurface = NULL;

	switch (txtType)
	{
		case SOLID: //the option for Solid style font
		{
			theTxtSurface = TTF_RenderText_Solid(theFont, text, txtColour); //this looks for the font being used, the text to be written and the colour of the text
		}
		break;
		case BLENDED: //the option for Blender style font
		{
			theTxtSurface = TTF_RenderText_Blended(theFont, text, txtColour);
		}
		break;
		case SHADED: //the option for Shaded style font
		{
			theTxtSurface = TTF_RenderText_Shaded(theFont, text, txtColour, txtBkgd); //this also requires a text background to be declared
		}
		break;
		default:
			break;
	}

	theTxtTexture = SDL_CreateTextureFromSurface(theRenderer, theTxtSurface);
	SDL_FreeSurface(theTxtSurface);

	return theTxtTexture;
}