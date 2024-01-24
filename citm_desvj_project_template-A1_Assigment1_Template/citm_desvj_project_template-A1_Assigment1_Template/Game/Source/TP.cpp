#include "TP.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

TP::TP() : Entity(EntityType::TP)
{
	name.Create("TP");
}

TP::~TP() {}

bool TP::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	return true;
}

bool TP::Start() {

	//initilize textures
	
	pbody = app->physics->CreateCircle(position.x + 16, position.y - 16, 16, bodyType::STATIC);
	pbody->ctype = ColliderType::TP;

	return true;
}

bool TP::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	return true;
}

bool TP::CleanUp()
{
	return true;
}

