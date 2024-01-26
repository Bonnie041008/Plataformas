#include "Checkpoint.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"
#include "Entity.h"


Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
	
	//actived
	actived.PushBack({ 0, 0, 96, 96 });
	actived.PushBack({ 96, 0, 96, 96 });
	actived.PushBack({ 192, 0, 96, 96 });
	actived.PushBack({ 288, 0, 96, 96 });
	actived.PushBack({ 384, 0, 96, 96 });
	actived.PushBack({ 480, 0, 96, 96 });
	actived.PushBack({ 576, 0, 96, 96 });
	actived.PushBack({ 672, 0, 96, 96 });
	actived.loop = true;
	actived.speed = 0.1f;

	//unactived
	inactive.PushBack({ 768, 0, 96, 96 });

	
}

Checkpoint::~Checkpoint() {}

bool Checkpoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &inactive;
	return true;
}

bool Checkpoint::Start() {

	//initilize textures
	
	texture = app->tex->Load("Assets/Textures/checkpoint-export.png");
	pbody = app->physics->CreateCircle(position.x , position.y , 16, bodyType::STATIC);
	pbody->ctype = ColliderType::CHECKPOINT;
	
	return true;
}

bool Checkpoint::Update(float dt)
{
	
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x-32, position.y-70, false, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (app->scene->player->position.x > METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 40 &&
		app->scene->player->position.x<METERS_TO_PIXELS(pbody->body->GetTransform().p.x) + 40 &&
		app->scene->player->position.y>METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 40 &&
		app->scene->player->position.y < METERS_TO_PIXELS(pbody->body->GetTransform().p.y) + 40
		&& !isPicked)
	{
		//efecto de sonido del portal
		
		isPicked = true;
		currentAnimation = &actived;
		app->scene->player->lastCheckpoint.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		app->scene->player->lastCheckpoint.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
		actived.Update();
	}
	if (isPicked) {
		currentAnimation = &actived;
	}


	return true;
}

bool Checkpoint::CleanUp()
{
	return true;
}