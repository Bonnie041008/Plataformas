#include "Flyer.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"
#include "Map.h"

Flyer::Flyer() : Entity(EntityType::FLYER)
{
	name.Create("Flyer");
	//idle
	flyingAnim.PushBack({ 0, 64, 64, 64 });
	flyingAnim.PushBack({ 70, 64, 64, 64 });
	flyingAnim.PushBack({ 140, 64, 64, 64 });
	flyingAnim.PushBack({ 210, 64, 64, 64 });
	flyingAnim.PushBack({ 280, 64, 64, 64 });
	flyingAnim.PushBack({ 350, 64, 64, 64 });
	flyingAnim.PushBack({ 420, 64, 64, 64 });
	
	flyingAnim.loop = true;
	flyingAnim.speed = 0.1f;

	

	//dead
	deadAnim.PushBack({ 0, 0, 64, 64 });
	deadAnim.PushBack({ 70, 0, 64, 64 });
	deadAnim.PushBack({ 140, 0, 64, 64 });
	deadAnim.PushBack({ 210, 0, 64, 64 });
	deadAnim.PushBack({ 280, 0, 64, 64 });
	deadAnim.PushBack({ 350, 0, 64, 64 });
	deadAnim.PushBack({ 420, 0, 64, 64 });
	deadAnim.PushBack({ 490, 0, 64, 64 });
	
	deadAnim.loop = false;
	deadAnim.speed = 0.2f;



}

Flyer::~Flyer() {

}

bool Flyer::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialX = position.x;
	initialY = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &flyingAnim;

	return true;
}

bool Flyer::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/spritesheetflyer.png");
	pbody = app->physics->CreateCircle(position.x +100, position.y - 190, 25, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;


	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

void Flyer::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

bool Flyer::Update(float dt)
{
	
	b2Vec2 vel = b2Vec2(move_x, move_y);
	move_x = 0;
	//move_y = -GRAVITY_Y;
	move_y = 0;
	if (isalive == true) {
		currentAnimation = &flyingAnim;
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

		app->render->DrawTexture(texture, position.x - 15, position.y - 20, isFliped, &currentAnimation->GetCurrentFrame());
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
		movY = (pos.y - this->position.y) / 50;
		vel.y = movY;

		if (position.x > pos.x)
		{
			isFliped = false;
		}
		else
		{
			isFliped = true;
		}

	}

	pbody->body->SetLinearVelocity(vel);


	

	
	

	return true;
}

bool Flyer::CleanUp()
{

	return true;
}

void Flyer::OnCollision(PhysBody* physA, PhysBody* physB) {

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