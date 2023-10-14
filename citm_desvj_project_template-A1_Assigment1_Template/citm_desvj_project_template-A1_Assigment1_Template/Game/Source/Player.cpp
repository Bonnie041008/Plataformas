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
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 10, position.y + 14, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	

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
	}
	// A�adir la funcionalidad de saltar
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

	app->render->DrawTexture(texture, position.x-19, position.y-20);

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