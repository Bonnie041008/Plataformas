#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Defs.h"
#include "Log.h"



Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	if (config.child("enemy")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy");
	}
	if (config.child("flyer")) {
		flyer = (Flyer*)app->entityManager->CreateEntity(EntityType::FLYER);
		flyer->parameters = config.child("flyer");
	}
	if (config.child("flyer2")) {
		flyer2 = (Flyer*)app->entityManager->CreateEntity(EntityType::FLYER);
		flyer2->parameters = config.child("flyer2");
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	
	app->audio->PlayMusic("Assets/Audio/Music/Sonido-de-Fondo.wav");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT) {
		debugcamera = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		debugcamera = false;
	}
	if (debugcamera == false) {
		if (player->position.x < app->win->screenSurface->w / 2) {
			app->render->camera.x = 0;
		}
		else {
			app->render->camera.x = -player->position.x + app->win->screenSurface->w / 2;
		}

		if (-player->position.y < app->win->screenSurface->h / 2) {
			app->render->camera.y = 0;
		}
		else {
			app->render->camera.y = -player->position.y + app->win->screenSurface->h / 2;
		}
		//app->render->camera.x = -player->position.x + app->win->screenSurface->w / 2;
		//app->render->camera.y = -player->position.y + app->win->screenSurface->h / 2;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);
		

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);
		

	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	// Get the mouse position and obtain the map coordinate
	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x,
		mousePos.y - app->render->camera.y);

	// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	//app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y, false);

	iPoint origin = mouseTile;

	//If mouse button is pressed modify player position
	/*if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		player->position = iPoint(highlightedTileWorld.x, highlightedTileWorld.y);*/
	//app->map->pathfinding->CreatePath(origin, app->map->WorldToMap(player->position.x, player->position.y));
	/*}*/

	/*const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y, false);
	}*/
	
	//Drawline();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

//void Scene::Drawline() {
//
//	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
//	for (uint i = 0; i < path->Count(); ++i)
//	{
//		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
//		app->render->DrawTexture(mouseTileTex, pos.x, pos.y, false);
//	}
//
//}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
bool Scene::LoadState(pugi::xml_node node) {
	
	

	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	b2Vec2 newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	
	enemy->position.x = node.child("enemy").attribute("x").as_int();
	enemy->position.y = node.child("enemy").attribute("y").as_int();
	newPos = b2Vec2(PIXEL_TO_METERS(enemy->position.x), PIXEL_TO_METERS(enemy->position.y));
	enemy->pbody->body->SetTransform(newPos, enemy->pbody->body->GetAngle());
	bool checkAlive = node.child("enemy").attribute("estavivo").as_bool();
	if (enemy->isalive && checkAlive == false) {

		enemy->health = 1;
		enemy->finalposition.x = METERS_TO_PIXELS(enemy->pbody->body->GetTransform().p.x) - 16;
		enemy->finalposition.y = METERS_TO_PIXELS(enemy->pbody->body->GetTransform().p.y) - 16;
		app->physics->DestroyObject(enemy->pbody);
		enemy->muriendo = 0;
		enemy->isalive = false;
		
	}

	flyer->position.x = node.child("flyer").attribute("x").as_int();
	player->position.y = node.child("flyer").attribute("y").as_int();
	newPos = b2Vec2(PIXEL_TO_METERS(flyer->position.x), PIXEL_TO_METERS(flyer->position.y));
	flyer->pbody->body->SetTransform(newPos, flyer->pbody->body->GetAngle());
	checkAlive = node.child("flyer").attribute("estavivo").as_bool();
	if (flyer->isalive && checkAlive == false) {

		flyer->health = 1;
		flyer->finalposition.x = METERS_TO_PIXELS(flyer->pbody->body->GetTransform().p.x) - 16;
		flyer->finalposition.y = METERS_TO_PIXELS(flyer->pbody->body->GetTransform().p.y) - 16;
		app->physics->DestroyObject(flyer->pbody);
		flyer->muriendo = 0;
		flyer->isalive = false;
	}

	flyer2->position.x = node.child("flyer2").attribute("x").as_int();
	flyer2->position.y = node.child("flyer2").attribute("y").as_int();
	newPos= b2Vec2(PIXEL_TO_METERS(flyer2->position.x), PIXEL_TO_METERS(flyer2->position.y));
	flyer2->pbody->body->SetTransform(newPos, flyer2->pbody->body->GetAngle());
	checkAlive = node.child("flyer2").attribute("estavivo").as_bool();
	if (flyer2->isalive && checkAlive == false) {
		flyer2->health = 1;
		flyer2->finalposition.x = METERS_TO_PIXELS(flyer2->pbody->body->GetTransform().p.x) - 16;
		flyer2->finalposition.y = METERS_TO_PIXELS(flyer2->pbody->body->GetTransform().p.y) - 16;
		app->physics->DestroyObject(flyer2->pbody);
		flyer2->muriendo = 0;
		flyer2->isalive = false;
	}

	return true;
	
}


bool Scene::SaveState(pugi::xml_node node) {
	pugi::xml_node playerNode = node.append_child("player");
	pugi::xml_node enemyNode = node.append_child("enemy");
	pugi::xml_node flyerNode = node.append_child("flyer");
	pugi::xml_node flyer2Node = node.append_child("flyer2");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);

	enemyNode.append_attribute("x").set_value(enemy->position.x);
	enemyNode.append_attribute("y").set_value(enemy->position.y);
	enemyNode.append_attribute("estavivo").set_value(enemy->isalive);

	flyerNode.append_attribute("x").set_value(flyer->position.x);
	flyerNode.append_attribute("y").set_value(flyer->position.y);
	flyerNode.append_attribute("estavivo").set_value(flyer->isalive);

	flyer2Node.append_attribute("x").set_value(flyer2->position.x);
	flyer2Node.append_attribute("y").set_value(flyer2->position.y);
	flyer2Node.append_attribute("estavivo").set_value(flyer2->isalive);


	return true;
}