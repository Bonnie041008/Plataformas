
#ifndef __TP_H__
#define __TP_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class TP : public Entity
{
public:

	TP();
	virtual ~TP();

	bool Awake();

	bool Start();

	bool Update(float dt);

	
	bool CleanUp();

public:



private:

	PhysBody* pbody;
};

#endif // __COIN_H__
