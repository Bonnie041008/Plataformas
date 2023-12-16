#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
	//idle
	idleAnim.PushBack({ 0, 0, 64, 64 });
	idleAnim.PushBack({ 64, 0, 64, 64 });
	idleAnim.PushBack({ 128, 0, 64, 64 });
	idleAnim.PushBack({ 192, 0, 64, 64 });
	idleAnim.PushBack({ 256, 0, 64, 64 });
	idleAnim.PushBack({ 320, 0, 64, 64 });
	idleAnim.PushBack({ 384, 0, 64, 64 });
	idleAnim.PushBack({ 448, 0, 64, 64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	//correr
	rightAnim.PushBack({ 0,64,64,64 });
	rightAnim.PushBack({ 64,64,64,64 });
	rightAnim.PushBack({ 128,64,64,64 });
	rightAnim.PushBack({ 192,64,64,64 });
	rightAnim.PushBack({ 256,64,64,64 });
	rightAnim.PushBack({ 320,64,64,64 });
	rightAnim.PushBack({ 384,64,64,64 });
	rightAnim.PushBack({ 448,64,64,64 });
	rightAnim.PushBack({ 512,64,64,64 });
	rightAnim.PushBack({ 576,64,64,64 });
	rightAnim.PushBack({ 640,64,64,64 });
	rightAnim.PushBack({ 704,64,64,64 });
	rightAnim.PushBack({ 768,64,64,64 });

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
	deadAnim.PushBack({ 704,128,64,64 });
	deadAnim.PushBack({ 768,128,64,64 });
	deadAnim.PushBack({ 832,128,64,64 });
	
	deadAnim.loop = false;
	deadAnim.speed = 0.2f;

	//attack
	attackAnim.PushBack({ 0,192,104,64 });
	attackAnim.PushBack({ 104,192,104,64 });
	attackAnim.PushBack({ 208,192,104,64 });
	attackAnim.PushBack({ 312,192,104,64 });
	attackAnim.PushBack({ 416,192,104,64 });
	attackAnim.PushBack({ 520,192,104,64 });
	attackAnim.PushBack({ 624,192,104,64 });

	attackAnim.loop = true;
	attackAnim.speed = 0.2f;
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialX = position.x;
	initialY = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &idleAnim;

	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/spritesheetskeleton2.png");
	pbody = app->physics->CreateCircle(position.x + 10, position.y - 190, 17, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

void Enemy::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

bool Enemy::Update(float dt)
{
	
	b2Vec2 vel = b2Vec2(move_x, move_y);
	move_x = 0;
	move_y = -GRAVITY_Y;
	if (isalive == true) {
		currentAnimation = &idleAnim;
	}
	




	vel = b2Vec2(move_x, move_y);

	if (health == 0 && isalive) {
		currentAnimation = &deadAnim;
		muriendo++;
		
		speed = 0;
		if (muriendo > 70) {
			
			
			health = 1;
			finalposition.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			finalposition.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
			app->physics->DestroyObject(pbody);
			muriendo = 0;
			isalive = false;
		}

	}
	if (isalive) {
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		
		
		app->render->DrawTexture(texture, position.x - 15, position.y - 30, isFliped, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	else {
		app->render->DrawTexture(texture, finalposition.x - 15, finalposition.y - 30, isFliped, &currentAnimation->GetCurrentFrame());
	}
	//Set the velocity of the pbody of the player

	app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y));
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	
	if (app->physics->debug == true)
	{
		for (uint i = 0; i < path->Count(); i++)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y, false);

			movX = (pos.x - this->position.x) / 50;
			vel.x = movX;

			if (position.x > pos.x)
			{
				isFliped = true;
			}
			else
			{
				isFliped = false;
			}
		}
		
	}

	for (uint i = 0; i < path->Count(); i++)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		
		movX = (pos.x - this->position.x) / 50;
		vel.x = movX;
		
		if (position.x > pos.x)
		{
			isFliped = true;
		}
		else
		{
			isFliped = false;
		}

	}

	pbody->body->SetLinearVelocity(vel);

	
	
	

	return true;
}

bool Enemy::CleanUp()
{

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (physA->ctype == ColliderType::ENEMY) {
		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			
			break;
		case ColliderType::FIREBALL:
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
	else if (physA->ctype == ColliderType::FIREBALL) {
		switch (physB->ctype)
		{

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
		case ColliderType::ENEMY:
			LOG("Collision ENEMY");
			fireBalltoDestroy = listOfFireballs.Find(physA);
		case ColliderType::FIREBALL:
			LOG("Collision FIREBALL");
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