/*
=================
cMonster.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cMonster.h"

/*
=================================================================
Initialise the sprite variables
=================================================================
*/

void cMonster::initialise()
{
	monsterVelocity = { 0, 0 };
	monsterRotAngle = 0;
}

//void cMonster::render()
//{
//}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cMonster::update(float deltaTime)
{

	SDL_Rect currentSpritePos = this->getSpritePos();

	currentSpritePos.x += monsterVelocity.x;
	currentSpritePos.y += monsterVelocity.y;

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y });
	this->monsterVelocity = {0,0};
}
/*
=================================================================
  Sets the velocity for the monster
=================================================================
*/
void cMonster::setMonsterVelocity(SDL_Point monsterVel)
{
	monsterVelocity = monsterVel;
}
/*
=================================================================
  Gets the monster velocity
=================================================================
*/
SDL_Point cMonster::getMonsterVelocity()
{
	return monsterVelocity;
}
/*
=================================================================
Sets the rotation angle for the monster
=================================================================
*/
void cMonster::setMonsterRotation(double theRotAngle)
{
	monsterRotAngle = theRotAngle;
}
/*
=================================================================
Gets the rotation angle for the monster
=================================================================
*/
double cMonster::getMonsterRotation()
{
	return monsterRotAngle;
}
