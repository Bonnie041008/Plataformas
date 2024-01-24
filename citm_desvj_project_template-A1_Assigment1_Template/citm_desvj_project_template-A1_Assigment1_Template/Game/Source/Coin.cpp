#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Coin::Coin() : Entity(EntityType::COIN)
{
	name.Create("coin");
}

Coin::~Coin() {}

bool Coin::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Coin::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::STATIC);
	
	pbody->ctype = ColliderType::COIN;

	return true;
}

bool Coin::Update(float dt)
{

	if (app->scene->player->currentposition.x > position.x - 40 &&
		app->scene->player->currentposition.x<position.x + 40 &&
		app->scene->player->currentposition.y>position.y - 40 &&
		app->scene->player->currentposition.y < position.y + 40
		&& !isPicked)
	{
		//efecto de sonido de moneda

		isPicked = true;
		app->physics->DestroyObject(pbody);
		
	}
	if (isPicked) {
		
	}
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y, false);

	
	return true;
}

bool Coin::CleanUp()
{
	return true;
}

