#pragma once
#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Checkpoint : public Entity
{
public:

	Checkpoint();
	virtual ~Checkpoint();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
	Animation inactive;
	Animation actived;
	int cnt;
	Animation* currentAnimation = nullptr;
};

#endif // __CHECKPOINT_H__
