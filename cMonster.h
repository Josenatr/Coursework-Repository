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
	cMonster();
	void update(double deltaTime);		// Rocket update method
	void setMonsterVelocity(SDL_Point rocketVel);   // Sets the velocity for the rocket
	SDL_Point getMonsterVelocity();
};
#endif