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

	rightAnim.loop = false;
	rightAnim.speed = 0.07f;

}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &idleAnim;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/spriteshee2t.png");
	//textureRun = app->tex->Load("Assets/Textures/SWMG-Sprite-Correr-Sheet.png");
	pbody = app->physics->CreateCircle(position.x + 10, position.y + 14, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	
	currentAnimation = &idleAnim;
	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed*dt, -GRAVITY_Y);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed*dt, -GRAVITY_Y);
		
		//texture = textureRun;
		currentAnimation = &rightAnim;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		//vel = b2Vec2(speed * dt, -GRAVITY_Y);

		health = 0;
		
		

	}
	if (health == 0) {
		currentAnimation = &deadAnim;
	}
	// Añadir la funcionalidad de saltar
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isjumping == false) {
		// Cambia la velocidad vertical para simular un salto
	
		isjumping = true;
		jumpcnt = 0;
	}
	if (isjumping) {

		if (GRAVITY_Y + jumpcnt - dt >= -GRAVITY_Y) {
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
		}

		jumpcnt++;
		
	}
	
	
	
	
	
	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x-19, position.y-20, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

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
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}