/*
=================
cmonster.h
- Header file for class definition - SPECIFICATION
- Header file for the mONSTER class which is a child of cSprite class
=================
*/
#ifndef _CMONSTER_H
#define _CMONSTER_H
#include "cSprite.h"

class cMonster : public cSprite
{
private:
	SDL_Point monsterVelocity;
	double monsterRotAngle;

public:
	void initialise();
	//void render();		// Default render function
	void update(float deltaTime);		// monster update method
	void setMonsterVelocity(SDL_Point monsterVel);   // Sets the velocity for the monster
	SDL_Point getMonsterVelocity();				 // Gets the monster velocity
	void setMonsterRotation(double theRotAngle);   // Sets the rotation angle for the monster
	double getMonsterRotation();				 // Gets the rotation angle for the monster velocity
};
#endif