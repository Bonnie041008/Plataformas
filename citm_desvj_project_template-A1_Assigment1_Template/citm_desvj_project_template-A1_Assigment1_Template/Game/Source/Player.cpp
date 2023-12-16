#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	//idle
	idleAnim.PushBack({ 0, 0, 64, 64 });
	idleAnim.PushBack({ 64, 0, 64, 64 });
	idleAnim.PushBack({ 128, 0, 64, 64 });
	idleAnim.PushBack({ 192, 0, 64, 64 });
	idleAnim.PushBack({ 256, 0, 64, 64 });
	idleAnim.PushBack({ 320, 0, 64, 64 }); 
	idleAnim.loop = true;
	idleAnim.speed = 0.07f;

	//correr
	rightAnim.PushBack({ 0,64,64,64 });
	rightAnim.PushBack({ 64,64,64,64 });
	rightAnim.PushBack({ 128,64,64,64 });
	rightAnim.PushBack({ 192,64,64,64 });
	rightAnim.PushBack({ 256,64,64,64 });
	rightAnim.PushBack({ 320,64,64,64 });
	rightAnim.PushBack({ 384,64,64,64 });
	rightAnim.PushBack({ 448,64,64,64 });

	rightAnim.loop = true;
	rightAnim.speed = 0.2f;

	//dead
	deadAnim.PushBack({ 0,128,64,64 });
	deadAnim.PushBack({ 64,128,64,64 });
	deadAnim.PushBack({ 128,128,64,64 });
	deadAnim.PushBack({ 192,128,64,64 });
	deadAnim.PushBack({ 256,128,64,64 });
	deadAnim.PushBack({ 320,128,64,64 });
	deadAnim.PushBack({ 384,128,64,64 });
	deadAnim.PushBack({ 448,128,64,64 });
	deadAnim.PushBack({ 512,128,64,64 });
	deadAnim.PushBack({ 576,128,64,64 });
	deadAnim.PushBack({ 640,128,64,64 });
	deadAnim.PushBack({ 0,0,0,0 });
	deadAnim.loop = false;
	deadAnim.speed = 0.2f;

	//salto

	jumpAnim.PushBack({ 0,192,64,64 });
	jumpAnim.PushBack({ 64,192,64,64 });
	jumpAnim.PushBack({ 128,192,64,64 });
	jumpAnim.PushBack({ 192,192,64,64 });
	jumpAnim.PushBack({ 256,192,64,64 });
	jumpAnim.PushBack({ 320,192,64,64 });
	/*jumpAnim.PushBack({ 384,192,64,64 });
	jumpAnim.PushBack({ 448,192,64,64 });
	jumpAnim.PushBack({ 512,192,64,64 });
	jumpAnim.PushBack({ 576,192,64,64 });*/
	




	jumpAnim.loop = false;
	jumpAnim.speed = 0.2f;




	//caida
	
	
	fallAnim.PushBack({ 0,0,0,0 });





	fallAnim.loop = false;
	fallAnim.speed = 0.2f;


}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialX = position.x;
	initialY = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &idleAnim;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/spriteshee2t.png");
	pbody = app->physics->CreateCircle(position.x + 10, position.y -200, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

void Player::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

bool Player::Update(float dt)
{
	if (fireBalltoDestroy != -1) {
		app->physics->DestroyObject(listOfFireballs[fireBalltoDestroy]);
		fireBalltoDestroy = -1;
		
	}
	b2Vec2 vel = b2Vec2(move_x, move_y);
	move_x = 0;
	move_y = -GRAVITY_Y;
	
	currentAnimation = &idleAnim;
	

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		
		
		b2Vec2 fireBallvel = b2Vec2(10, 0);

		if (isFliped) {
			fireBallvel= b2Vec2(-10, 0);
			fireBall = app->physics->CreateCircle(position.x  -30, position.y + 10, 20, bodyType::DYNAMIC);
		}
		else {
			fireBall = app->physics->CreateCircle(position.x + 70, position.y + 10, 20, bodyType::DYNAMIC);
			
		}
		fireBall->listener = this;
		fireBall->ctype = ColliderType::FIREBALL;
		fireBall->body->SetLinearVelocity(fireBallvel);
		listOfFireballs.Add(fireBall);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		move_x = -speed*dt;
		isFliped = true;

		if (godmode == true)
		{
			move_x = -speed * 5 *dt;
		}

		currentAnimation = &rightAnim;

	}
	
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		move_x = +speed * dt;
		isFliped = false;
		//texture = textureRun;
		currentAnimation = &rightAnim;
		if (godmode == true)
		{
			move_x = +speed * 5 * dt;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		SetPosition(400, 352);
		health = 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		SetPosition(400, 352);
		health = 1;
	}


	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		//vel = b2Vec2(speed * dt, -GRAVITY_Y);

		health = 0;
		
		

	}
	//godmode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (godmode == false) {
			godmode = true;
		}
		else if (godmode == true) {
			godmode = false;
		}
	}

	if (godmode == true)
	{
		move_y = 0;

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			move_y = -10;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			move_y = 10;
		}
		godspeed = speed * 10;

	}
	
	//muerte
	if (health == 0 && isalive) {
		currentAnimation = &deadAnim;
		muriendo++;
		
		speed = 0;
		if (muriendo > 70) {
			SetPosition(400, 352);
			speed = 0.3f;
			currentAnimation = &idleAnim;
			deadAnim.Reset();
			health = 1;
			
			muriendo = 0;
		}
	
	}
	// Añadir la funcionalidad de saltar
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isjumping == false) {
		// Cambia la velocidad vertical para simular un salto
	
		isjumping = true;
		jumpcnt = 0;
		
	}
	if (isjumping) {
		currentAnimation = &jumpAnim;
		/*if (GRAVITY_Y + jumpcnt - dt >= -GRAVITY_Y) {
			vel = b2Vec2(0 * dt, -GRAVITY_Y);
		}
		else {
			vel = b2Vec2(0 * dt, GRAVITY_Y + jumpcnt - dt+5);
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			
			if (GRAVITY_Y + jumpcnt - dt >= -GRAVITY_Y) {
				vel = b2Vec2(-speed * dt, -GRAVITY_Y);
			}
			else {
				vel = b2Vec2(-speed * dt, GRAVITY_Y + jumpcnt - dt+5);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speed * dt, GRAVITY_Y + jumpcnt - dt);

			if (GRAVITY_Y + jumpcnt - dt >= -GRAVITY_Y) {
				vel = b2Vec2(speed * dt, -GRAVITY_Y);
			}
			else {
				vel = b2Vec2(speed * dt, GRAVITY_Y + jumpcnt - dt+5);
			}
		}*/

		move_y = GRAVITY_Y + jumpcnt - dt + 5;
		currentAnimation = &jumpAnim;

		if(health == 0 && currentAnimation == &jumpAnim){
			currentAnimation = &deadAnim;
		}
		
		jumpcnt++;
		
	}
	else {
		jumpAnim.Reset();
		jumpcnt = 0;
	}
	
	
	vel = b2Vec2(move_x, move_y);
	
	
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x-15, position.y-27,isFliped, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (physA->ctype == ColliderType::PLAYER) {
		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if (isjumping) {
				isjumping = false;
			}
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if (godmode == false)
			{
				health = 0;
			}
			if (health == 0 && isalive) {
				muriendo++;
				currentAnimation = &deadAnim;
				//SetPosition(400, 352);
			}
		case ColliderType::ENEMY:
			LOG("Collision ENEMY");
			if (godmode == false)
			{
				health = 0;
			}
			if (health == 0 && isalive) {
				muriendo++;
				currentAnimation = &deadAnim;
				//SetPosition(400, 352);
			}

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
	else if(physA->ctype == ColliderType::FIREBALL){
		switch (physB->ctype)
		{
		case ColliderType::ENEMY:
			LOG("Collision ENEMY");
			fireBalltoDestroy = listOfFireballs.Find(physA);
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			fireBalltoDestroy = listOfFireballs.Find(physA);
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if (godmode == false)
			{
				health = 0;
			}
			if (health == 0 && isalive) {
				muriendo++;
				currentAnimation = &deadAnim;
				//SetPosition(400, 352);
			}

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
	
	

}