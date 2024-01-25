#include "HealthItem.h"
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


HealthItem::HealthItem() : Entity(EntityType::HEALTHITEM)
{
	name.Create("healthItem");

	//actived
	actived.PushBack({ 0, 0, 192, 192 });
	actived.PushBack({ 192, 0, 192, 192 });
	actived.PushBack({ 384, 0, 192, 192 });
	actived.PushBack({ 576, 0, 192, 192 });
	actived.PushBack({ 768, 0, 192, 192 });
	actived.PushBack({ 960, 0, 192, 192 });
	actived.loop = true;
	actived.speed = 0.1f;
}

HealthItem::~HealthItem() {}

bool HealthItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &actived;
	return true;
}

bool HealthItem::Start() {

	//initilize textures

	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::STATIC);
	pbody->ctype = ColliderType::HEALTHITEM;

	return true;
}

bool HealthItem::Update(float dt)
{
	//currentAnimation = &actived;
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	currentAnimation = &actived;
	app->render->DrawTexture(texture, position.x - 80, position.y - 80, false, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if ((app->scene->player->currentposition.x > position.x - 40 &&
		app->scene->player->currentposition.x<position.x + 40 &&
		app->scene->player->currentposition.y>position.y - 40 &&
		app->scene->player->currentposition.y < position.y + 40
		&& !isPicked))
	{
		//efecto de sonido del portal

		isPicked = true;
		app->scene->player->lives = app->scene->player->lives + 1;
		app->physics->DestroyObject(pbody);
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	return true;
}

bool HealthItem::CleanUp()
{
	return true;
}