#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "EntityManager.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPosition(int x, int y);

public:
	PhysBody* pbody;
	float speed = 0.3f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* currentTexture;
	int fireBalltoDestroy = -1;
	SDL_Texture* textureRun = NULL;

	float initialX;
	float initialY;
	int health = 1;
	bool isalive = true;
	bool isFliped;
	int muriendo = 0;

	bool godmode;
	float move_y = -10.0f;
	float move_x = 0.0f;
	float godspeed = 0.0f;
	iPoint finalposition;

	PhysBody* fireBall;
	bool hasToDestroyBall = false;
	List <PhysBody*> listOfFireballs;
	int pickCoinFxId;
	const float JUMP_FORCE = 50.0f; // Ajusta este valor según tus necesidades
	const float MAX_FALL_SPEED = 10.0f;
	
	Animation idleAnim;
	
	Animation rightAnim;
	Animation deadAnim;
	Animation attackAnim;
	Animation* currentAnimation = nullptr;

	float movX;
	float movY;
	int counter;
	
};

#endif // __ENEMY_H__
