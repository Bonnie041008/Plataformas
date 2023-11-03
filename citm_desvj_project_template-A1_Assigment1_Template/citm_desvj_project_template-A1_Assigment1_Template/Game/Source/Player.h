#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* currentTexture;
	SDL_Texture* textureRun = NULL;

	int health = 1;
	bool isalive = true;
	PhysBody* pbody;
	int pickCoinFxId;
	const float JUMP_FORCE = 50.0f; // Ajusta este valor según tus necesidades
	const float MAX_FALL_SPEED = 10.0f;
	bool isjumping= false;
	int jumpcnt;
	Animation idleAnim;
	Animation leftAnim;
	Animation rightAnim;
	Animation deadAnim;
	Animation* currentAnimation = nullptr;
};

#endif // __PLAYER_H__