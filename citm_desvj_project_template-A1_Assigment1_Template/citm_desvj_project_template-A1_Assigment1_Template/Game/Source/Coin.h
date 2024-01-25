
#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Coin : public Entity
{
public:

	Coin();
	virtual ~Coin();

	bool Awake();

	bool Start();

	bool Update(float dt);

	
	bool CleanUp();

public:

	bool isPicked = false;
	PhysBody* pbody;
	
private:

	SDL_Texture* texture;
	Animation actived;
	Animation* currentAnimation = nullptr;
	const char* texturePath;

};

#endif // __COIN_H__
