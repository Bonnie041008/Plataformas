#ifndef __Boss_H__
#define __Boss_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "EntityManager.h"

struct SDL_Texture;

class Boss : public Entity
{
public:

	Boss();

	virtual ~Boss();

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
	SDL_Texture* bossRip = NULL;
	int fireBalltoDestroy = -1;
	SDL_Texture* textureRun = NULL;
	int cntAnim = 0;
	bool isAttacking = false;
	bool parryMode = true;
	int cntfire;
	float initialX;
	float initialY;
	int health = 5;
	bool isalive = true;
	bool isFliped;
	int muriendo = 0;
	int walkingRange = 0;

	float parryToggleTimer = 0.0f;
	bool godmode;
	float move_y = -10.0f;
	float move_x = 0.0f;
	float godspeed = 0.0f;
	iPoint finalposition;
	int cntatt = 0;

	SDL_Texture* pantallaWin;
	
	bool Muerte_Boss = true;
	int MuerteBoss;
	bool Ataque_Boss = true;
	int AtaqueBoss;
	int ParryBoss;
	const float JUMP_FORCE = 50.0f; // Ajusta este valor según tus necesidades
	const float MAX_FALL_SPEED = 10.0f;
	
	Animation idleAnim;
	
	Animation rightAnim;
	Animation deadAnim;
	Animation attackAnim;
	Animation idleAnim2;

	Animation rightAnim2;
	Animation deadAnim2;
	Animation attackAnim2;
	Animation* currentAnimation = nullptr;

	float movX;
	float movY;
	int counter;
	
};

#endif // __Boss_H__
