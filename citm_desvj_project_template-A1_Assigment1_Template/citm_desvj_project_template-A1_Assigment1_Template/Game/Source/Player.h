#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "EntityManager.h"

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

	void SetPosition(int x, int y);

public:
	PhysBody* pbody;
	float speed = 0.3f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* fireballtexture = NULL;
	SDL_Texture* currentTexture;
	SDL_Texture* fondobotonesPausa;
	int fireBalltoDestroy = -1;
	int enemyfireBalltoDestroy = -1;
	SDL_Texture* textureRun = NULL;

	float initialX;
	float initialY;
	int health = 1;
	int lives = 5;
	bool isalive = true;
	bool isFliped;
	int muriendo = 0;
	int cntfire = 0;
	int firelive = 0;
	bool godmode = false;
	float move_y = -10.0f;
	float move_x = 0.0f;
	float godspeed = 0.0f;

	bool pausa = false;
	bool fondoPausa = false;

	
	PhysBody* fireBall;
	bool hasToDestroyBall = false;
	List <PhysBody*> listOfFireballs;
	int pickCoinFxId;
	int coinCount=0;
	int timerFrames = 0;
	int timerGod = 0;
	bool Muerte_Mago = true;
	int MuerteMago;
	bool Salto_Mago = false;
	int SaltoMago;
	bool Ataque_Mago = true;
	int AtaqueMago;
	const float JUMP_FORCE = 50.0f; // Ajusta este valor seg�n tus necesidades
	const float MAX_FALL_SPEED = 10.0f;
	bool isjumping= false;
	bool isAttacking = false;
	int jumpcnt;
	iPoint lastCheckpoint = { 183,635 };
	iPoint currentposition;
	Animation idleAnim;
	Animation jumpAnim;
	Animation fallAnim;
	Animation rightAnim;
	Animation deadAnim;
	Animation attackAnim;
	Animation* currentAnimation = nullptr;
};

#endif // __PLAYER_H__