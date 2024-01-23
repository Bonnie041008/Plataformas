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


#include "GuiControl.h"
#include "GuiManager.h"
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
	if (config.child("checkpoint")) {
		checkpoint = (Checkpoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkpoint->parameters = config.child("checkpoint");
	}
	//Boss---------------------------------------------------------------------------
	if (config.child("boss")) {
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = config.child("boss");
	}
	//Esqueletos---------------------------------------------------------------------

	if (config.child("enemy2")) {
		enemy2 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy2->parameters = config.child("enemy2");
	}
	if (config.child("enemy")) {
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy->parameters = config.child("enemy");
	}
	if (config.child("enemy3")) {
		enemy3 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy3->parameters = config.child("enemy3");
	}
	if (config.child("enemy4")) {
		enemy4 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy4->parameters = config.child("enemy4");
	}
	if (config.child("enemy5")) {
		enemy5 = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		enemy5->parameters = config.child("enemy5");
	}

	//Fantasmas----------------------------------------------------------------------

	if (config.child("flyer")) {
		flyer = (Flyer*)app->entityManager->CreateEntity(EntityType::FLYER);
		flyer->parameters = config.child("flyer");
	}
	if (config.child("flyer2")) {
		flyer2 = (Flyer*)app->entityManager->CreateEntity(EntityType::FLYER);
		flyer2->parameters = config.child("flyer2");
	}
	if (config.child("flyer3")) {
		flyer3 = (Flyer*)app->entityManager->CreateEntity(EntityType::FLYER);
		flyer3->parameters = config.child("flyer3");
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//app->audio->PlayMusic("Assets/Audio/Music/Sonido-de-Fondo.wav");

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

	//Pantallas
	//pantallaInicio = app->tex->Load("Assets/Pantallas/SWMG_PantallaInicio.png");
	//pantallaWin = app->tex->Load("Assets/Pantallas/SWMG_PantallaWin.png");
	//Poner en el config.xml
	// <pantallaInicio texturepath="Assets/Pantallas/SWMG_PantallaInicio.png"/>
	//<pantallaWin texturepath = "Assets/Pantallas/SWMG_PantallaWin.png" / >

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Exit", btPos, this);
	exitButton->function = FunctionGUI::EXIT;
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
	iPoint origin = mouseTile;

	//app->render->DrawTexture(pantallaInicio, 0, 0, false);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (exitButton->exit == true) {
		ret = false;
	}
	return ret;
}


// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing Menu");

	return true;
}
bool Scene::LoadState(pugi::xml_node node) {
	
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	b2Vec2 newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	
	//Esqueletos---------------------------------------------------------------------------------------
	bool checkAlive = true;
	if (enemy->isalive == true) {
		enemy->position.x = node.child("enemy").attribute("x").as_int();
		enemy->position.y = node.child("enemy").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(enemy->position.x), PIXEL_TO_METERS(enemy->position.y));
		enemy->pbody->body->SetTransform(newPos, enemy->pbody->body->GetAngle());
		checkAlive = node.child("enemy").attribute("estavivo").as_bool();
		if (enemy->isalive && checkAlive == false) {

			enemy->health = 1;
			enemy->finalposition.x = METERS_TO_PIXELS(enemy->pbody->body->GetTransform().p.x);
			enemy->finalposition.y = METERS_TO_PIXELS(enemy->pbody->body->GetTransform().p.y);
			app->physics->DestroyObject(enemy->pbody);
			enemy->pbody->body->SetActive(false);
			enemy->muriendo = 0;
			enemy->isalive = false;

		}
	}
	
	if (enemy2->isalive == true) {
		enemy2->position.x = node.child("enemy2").attribute("x").as_int();
		enemy2->position.y = node.child("enemy2").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(enemy2->position.x), PIXEL_TO_METERS(enemy2->position.y));
		enemy2->pbody->body->SetTransform(newPos, enemy2->pbody->body->GetAngle());
		checkAlive = node.child("enemy2").attribute("estavivo").as_bool();
		if (enemy2->isalive && checkAlive == false) {

			enemy2->health = 1;
			enemy2->finalposition.x = METERS_TO_PIXELS(enemy2->pbody->body->GetTransform().p.x);
			enemy2->finalposition.y = METERS_TO_PIXELS(enemy2->pbody->body->GetTransform().p.y);
			enemy2->pbody->body->SetActive(false);
			enemy2->muriendo = 0;
			enemy2->isalive = false;

		}
	}
	
	if (enemy3->isalive == true) {
		enemy3->position.x = node.child("enemy3").attribute("x").as_int();
		enemy3->position.y = node.child("enemy3").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(enemy3->position.x), PIXEL_TO_METERS(enemy3->position.y));
		enemy3->pbody->body->SetTransform(newPos, enemy3->pbody->body->GetAngle());
		checkAlive = node.child("enemy3").attribute("estavivo").as_bool();
		if (enemy3->isalive && checkAlive == false) {

			enemy3->health = 1;
			enemy3->finalposition.x = METERS_TO_PIXELS(enemy3->pbody->body->GetTransform().p.x);
			enemy3->finalposition.y = METERS_TO_PIXELS(enemy3->pbody->body->GetTransform().p.y);
			enemy3->pbody->body->SetActive(false);
			enemy3->muriendo = 0;
			enemy3->isalive = false;

		}
	}
	
	if (enemy4->isalive == true) {
		enemy4->position.x = node.child("enemy4").attribute("x").as_int();
		enemy4->position.y = node.child("enemy4").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(enemy4->position.x), PIXEL_TO_METERS(enemy4->position.y));
		enemy4->pbody->body->SetTransform(newPos, enemy4->pbody->body->GetAngle());
		checkAlive = node.child("enemy4").attribute("estavivo").as_bool();
		if (enemy4->isalive && checkAlive == false) {

			enemy4->health = 1;
			enemy4->finalposition.x = METERS_TO_PIXELS(enemy4->pbody->body->GetTransform().p.x);
			enemy4->finalposition.y = METERS_TO_PIXELS(enemy4->pbody->body->GetTransform().p.y);
			enemy4->pbody->body->SetActive(false);
			enemy4->muriendo = 0;
			enemy4->isalive = false;

		}
	}
	
	if (enemy5->isalive == true) {
		enemy5->position.x = node.child("enemy5").attribute("x").as_int();
		enemy5->position.y = node.child("enemy5").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(enemy5->position.x), PIXEL_TO_METERS(enemy5->position.y));
		enemy5->pbody->body->SetTransform(newPos, enemy5->pbody->body->GetAngle());
		checkAlive = node.child("enemy5").attribute("estavivo").as_bool();
		if (enemy5->isalive && checkAlive == false) {

			enemy5->health = 1;
			enemy5->finalposition.x = METERS_TO_PIXELS(enemy5->pbody->body->GetTransform().p.x);
			enemy5->finalposition.y = METERS_TO_PIXELS(enemy5->pbody->body->GetTransform().p.y);
			enemy5->pbody->body->SetActive(false);
			enemy5->muriendo = 0;
			enemy5->isalive = false;
		}
	}

	//Fantasmas---------------------------------------------------------------------------------------
	if (flyer->isalive == true) {
		flyer->position.x = node.child("flyer").attribute("x").as_int();
		player->position.y = node.child("flyer").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(flyer->position.x), PIXEL_TO_METERS(flyer->position.y));
		flyer->pbody->body->SetTransform(newPos, flyer->pbody->body->GetAngle());
		checkAlive = node.child("flyer").attribute("estavivo").as_bool();
		if ( checkAlive == false) {

			flyer->health = 1;
			flyer->finalposition.x = METERS_TO_PIXELS(flyer->pbody->body->GetTransform().p.x) - 16;
			flyer->finalposition.y = METERS_TO_PIXELS(flyer->pbody->body->GetTransform().p.y) - 16;
			
			flyer->pbody->body->SetActive(false);
			flyer->muriendo = 0;
			flyer->isalive = false;
		}
	}
	
	if (flyer2->isalive == true) {
		flyer2->position.x = node.child("flyer2").attribute("x").as_int();
		flyer2->position.y = node.child("flyer2").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(flyer2->position.x), PIXEL_TO_METERS(flyer2->position.y));
		flyer2->pbody->body->SetTransform(newPos, flyer2->pbody->body->GetAngle());
		checkAlive = node.child("flyer2").attribute("estavivo").as_bool();
		if (checkAlive == false) {

			flyer2->health = 1;
			flyer2->finalposition.x = METERS_TO_PIXELS(flyer2->pbody->body->GetTransform().p.x) - 16;
			flyer2->finalposition.y = METERS_TO_PIXELS(flyer2->pbody->body->GetTransform().p.y) - 16;
			flyer2->pbody->body->SetActive(false);
			flyer2->muriendo = 0;
			flyer2->isalive = false;
		}
	}
	
	if (flyer3->isalive == true) {
		flyer3->position.x = node.child("flyer3").attribute("x").as_int();
		flyer3->position.y = node.child("flyer3").attribute("y").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(flyer3->position.x), PIXEL_TO_METERS(flyer3->position.y));
		flyer3->pbody->body->SetTransform(newPos, flyer3->pbody->body->GetAngle());
		checkAlive = node.child("flyer3").attribute("estavivo").as_bool();
		if (flyer3->isalive && checkAlive == false) {

			flyer3->health = 1;
			flyer3->finalposition.x = METERS_TO_PIXELS(flyer3->pbody->body->GetTransform().p.x) - 16;
			flyer3->finalposition.y = METERS_TO_PIXELS(flyer3->pbody->body->GetTransform().p.y) - 16;
			flyer3->pbody->body->SetActive(false);
			flyer3->muriendo = 0;
			flyer3->isalive = false;
		}
	}
	

	return true;
	
}


bool Scene::SaveState(pugi::xml_node node) {
	pugi::xml_node playerNode = node.append_child("player");
	pugi::xml_node enemyNode = node.append_child("enemy");
	pugi::xml_node enemy2Node = node.append_child("enemy2");
	pugi::xml_node enemy3Node = node.append_child("enemy3");
	pugi::xml_node enemy4Node = node.append_child("enemy4");
	pugi::xml_node enemy5Node = node.append_child("enemy5");
	pugi::xml_node flyerNode = node.append_child("flyer");
	pugi::xml_node flyer2Node = node.append_child("flyer2");
	pugi::xml_node flyer3Node = node.append_child("flyer3");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);

	//Fantasmas---------------------------------------------------------------------------------------

	enemyNode.append_attribute("x").set_value(enemy->position.x);
	enemyNode.append_attribute("y").set_value(enemy->position.y);
	enemyNode.append_attribute("estavivo").set_value(enemy->isalive);

	enemy2Node.append_attribute("x").set_value(enemy2->position.x);
	enemy2Node.append_attribute("y").set_value(enemy2->position.y);
	enemy2Node.append_attribute("estavivo").set_value(enemy2->isalive);

	enemy3Node.append_attribute("x").set_value(enemy3->position.x);
	enemy3Node.append_attribute("y").set_value(enemy3->position.y);
	enemy3Node.append_attribute("estavivo").set_value(enemy3->isalive);

	enemy4Node.append_attribute("x").set_value(enemy4->position.x);
	enemy4Node.append_attribute("y").set_value(enemy4->position.y);
	enemy4Node.append_attribute("estavivo").set_value(enemy4->isalive);

	enemy5Node.append_attribute("x").set_value(enemy5->position.x);
	enemy5Node.append_attribute("y").set_value(enemy5->position.y);
	enemy5Node.append_attribute("estavivo").set_value(enemy5->isalive);

	

	//Fantasmas---------------------------------------------------------------------------------------

	flyerNode.append_attribute("x").set_value(flyer->position.x);
	flyerNode.append_attribute("y").set_value(flyer->position.y);
	flyerNode.append_attribute("estavivo").set_value(flyer->isalive);

	flyer2Node.append_attribute("x").set_value(flyer2->position.x);
	flyer2Node.append_attribute("y").set_value(flyer2->position.y);
	flyer2Node.append_attribute("estavivo").set_value(flyer2->isalive);

	flyer3Node.append_attribute("x").set_value(flyer3->position.x);
	flyer3Node.append_attribute("y").set_value(flyer3->position.y);
	flyer3Node.append_attribute("estavivo").set_value(flyer3->isalive);


	return true;
}