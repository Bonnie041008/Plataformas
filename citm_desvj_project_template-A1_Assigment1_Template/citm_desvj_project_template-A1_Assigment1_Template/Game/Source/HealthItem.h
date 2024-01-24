#pragma once
#ifndef __HEALTHITEM_H__
#define __HEALTHITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class HealthItem : public Entity
{
public:

	HealthItem();
	virtual ~HealthItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation inactive;
	Animation actived;
	int cnt;
	Animation* currentAnimation = nullptr;
};

#endif // __HEALTHITEM_H__
