/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Set filename
	theFile.setFileName("Data/usermap.dat");
	// Check file is available
	if (!theFile.openFile(ios::in)) //open file for input output
	{
		cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
		fileAvailable = false;
	}
	else
	{
		cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
		fileAvailable = true;
	}

	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "tile1", "tile2", "tile3", "tile4", "tile5","tile6", "tile7", "tile8", "tile9", "tile10", "tile11", "tile12", "theBackground"};
	texturesToUse = { "Images/Tiles/1.png", "Images/Tiles/2.png", "Images/Tiles/3.png", "Images/Tiles/4.png", "Images/Tiles/5.png", "Images/Tiles/6.png","Images/Tiles/7.png", "Images/Tiles/8.png", "Images/Tiles/9.png", "Images/Tiles/10.png", "Images/Tiles/11.png", "Images/Tiles/12.png", "Images/Bkg/bkg.jpg"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	tempTextTexture = theTextureMgr->getTexture("tile1");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 0, 0, 0, 255 };
	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "submit_btn", "settings_btn", "giveup_btn", "credits_btn", "retry_button"};
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png",  "Images/Buttons/button_giveup.png",  "Images/Buttons/button_credits.png", "Images/Buttons/button_retry.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;
	// Create textures for Game Dialogue (text)
	fontList = { "marker", "ice" };
	fontsToUse = { "Fonts/marker.ttf", "Fonts/ice_sticks.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "CreateTxt", "RandomTxt", "DragDropTxt", "ThanksTxt", "SeeYouTxt", "DrawHereTxt", "DrawHouseTxt", "SelectTxt", "NiceTryTxt", "RetryTxt"};
	gameTextList = { "Sketcher", "Create your own designs", "or try the random challenge", "", "Thanks for playing!", "See you again soon!", "^ Draw here! ^", "Draw a house!", "Select your colour here!", "Nice Try! Better luck next time!", "Try Again or return to the menu"};
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("marker")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
	}
	// Load game sounds
	soundList = { "theme", "click" };
	soundTypes = { MUSIC, SFX };
	soundsToUse = { "Audio/Theme/212797__genghis-attenborough__keyboard-intro-1.wav", "Audio/SFX/ClickOn.wav"};
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theTileMap.setMapStartXY({ 100, 100 });
	theTilePicker.setTileListStartXY({ 740, 100 });
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 30, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("CreateTxt");
		pos = { 300, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("RandomTxt");
		pos = { 300, 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("DragDropTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 425 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("load_btn")->setSpritePos({ 740, 500 });
		theButtonMgr->getBtn("load_btn")->render(theRenderer, &theButtonMgr->getBtn("load_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("load_btn")->getSpritePos(), theButtonMgr->getBtn("load_btn")->getSpriteScale());
		theSoundMgr->getSnd("theme")->play(-1);
	}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("DrawHereTxt");
		pos = { 50, 650, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("DrawHouseTxt");
		pos = { 450, 650, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SelectTxt");
		pos = { 600, 30, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTilePicker.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour);
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 740, 650 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("giveup_btn")->setSpritePos({ 740, 500 });
		theButtonMgr->getBtn("giveup_btn")->render(theRenderer, &theButtonMgr->getBtn("giveup_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("giveup_btn")->getSpritePos(), theButtonMgr->getBtn("giveup_btn")->getSpriteScale());
		theButtonMgr->getBtn("submit_btn")->setSpritePos({ 740, 500 });
		theButtonMgr->getBtn("submit_btn")->render(theRenderer, &theButtonMgr->getBtn("submit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("submit_btn")->getSpritePos(), theButtonMgr->getBtn("submit_btn")->getSpriteScale());
		if (theTilePicker.getTilePicked() > -1)
		{
			dragTile.render(theRenderer, &dragTile.getSpriteDimensions(), &dragTile.getSpritePos(), spriteBkgd.getSpriteScale());
		}
	}
	break;
	case SUBMIT:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 300, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}

	case GIVEUP:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("NiceTryTxt");
		pos = { 300, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("RetryTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("retry_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("retry_btn")->render(theRenderer, &theButtonMgr->getBtn("retry_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("retry_btn")->getSpritePos(), theButtonMgr->getBtn("retry_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 650 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// CHeck Button clicked and change state
	switch (theGameState)
	{
		case MENU:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
			theGameState = theButtonMgr->getBtn("laod_btn")->update(theGameState, SAVEMAP, theAreaClicked);
		}
		break;
		case PLAYING:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, SUBMIT, theAreaClicked);
			//theGameState = theButtonMgr->getBtn("load_btn")->update(theGameState, LOADMAP, theAreaClicked);
			//if (fileAvailable && theGameState == LOADMAP)
			//{
			//	theTileMap.initialiseMapFromFile(&theFile);
			//	theGameState = PLAYING;
			//	theAreaClicked = { 0, 0 };
			//}
			theGameState = theButtonMgr->getBtn("submit_btn")->update(theGameState, SAVEMAP, theAreaClicked);
			if (theGameState == SAVEMAP)
			{
				// Check file is available
				if (!theFile.openFile(ios::out)) //open file for output
				{
					cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				}
				else
				{
					cout << "File '" << theFile.getFileName() << "' opened for output!" << endl;
					theTileMap.writeMapDataToFile(&theFile);
				}

				theTileMap.writeMapDataToFile(&theFile);
				theGameState = PLAYING;
				theAreaClicked = { 0, 0 };
			}
		}
		break;
		case SUBMIT:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
			theGameState = theButtonMgr->getBtn("credits_btn")->update(theGameState, CREDITS, theAreaClicked);
		}
		case GIVEUP:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("retry_btn")->update(theGameState, PLAYING, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
			theGameState = theButtonMgr->getBtn("credits_btn")->update(theGameState, CREDITS, theAreaClicked);
		}
		break;
		case QUIT:
		{
		}
		break;
		default:
			break;
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
					if (theGameState == PLAYING)
					{
						theTilePicker.update(theAreaClicked);
						if (theTilePicker.getTilePicked() > -1)
						{
							theSoundMgr->getSnd("click")->play(0);
							dragTile.setSpritePos(theAreaClicked);
							dragTile.setTexture(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1]));
							dragTile.setSpriteDimensions(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTWidth(), theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTHeight());
						}
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					if (theGameState == PLAYING)
					{
						theAreaClicked = { event.motion.x, event.motion.y };
						theTileMap.update(theAreaClicked, theTilePicker.getTilePicked());
						theTilePicker.setTilePicked(-1);
						theSoundMgr->getSnd("click")->play(0);
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				dragTile.setSpritePos({ event.motion.x, event.motion.y });
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
				}
				break;

				case SDLK_UP:
				{
				}
				break;
				case SDLK_RIGHT:
				{
				}
				break;

				case SDLK_LEFT:
				{
				}
				break;
				case SDLK_SPACE:
				{
				}
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

