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
#include "ModuleFadeToBlack.h"
#include "Menu.h"
#include "Coin.h"
#include "GameOver.h"
#include "Win.h"

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


	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	//Checkpoints------------------------------------------------------------------------

	for (pugi::xml_node CheckpointNode = config.child("checkpoint"); CheckpointNode; CheckpointNode = CheckpointNode.next_sibling("checkpoint"))
	{
		Checkpoint* checkPoint = (Checkpoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkPoint->parameters = CheckpointNode;
		listOfCheckpoints.Add(checkPoint);
	}
	//Coins--------------------------------------------------------------------------------
	for (pugi::xml_node CoinNode = config.child("coin"); CoinNode; CoinNode = CoinNode.next_sibling("coin"))
	{
		Coin* coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
		coin->parameters = CoinNode;
		listOfCoins.Add(coin);
	}
	//Healthitem--------------------------------------------------------------------------------
	for (pugi::xml_node HealthNode = config.child("healthItem"); HealthNode; HealthNode = HealthNode.next_sibling("healthItem"))
	{
		HealthItem* healthItem = (HealthItem*)app->entityManager->CreateEntity(EntityType::HEALTHITEM);
		healthItem->parameters = HealthNode;
		listOfHealtthitems.Add(healthItem);
	}
	//TP---------------------------------------------------------------------------
	if (config.child("tp")) {
		tp = (TP*)app->entityManager->CreateEntity(EntityType::TP);
		tp->parameters = config.child("tp");
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
	SDL_Rect btPosResume = { windowW / 2 - 60 + 480, windowH / 2 + 250, 190,50 };
	SDL_Rect btPosSettings2 = { windowW / 2 - 60 + 480, windowH / 2 + 350, 190,50 };
	SDL_Rect btPosBackToTitle = { windowW / 2 - 60 + 380, windowH / 2 + 450, 410,60 };
	SDL_Rect btPosExit = { windowW / 2 - 60 + 500, windowH / 2 + 550, 150,50 };

	SDL_Rect btPosMusicBar = { windowW / 2 - 60 + 480, windowH / 2 + 250, 190,50 };
	SDL_Rect btPosFxBar = { windowW / 2 - 60 + 480, windowH / 2 + 350, 190,50 };
	SDL_Rect btPosFull = { windowW / 2 - 60 + 650, windowH / 2 + 450 , 20,20 };
	SDL_Rect btPosVsync = { windowW / 2 - 60 + 650, windowH / 2 + 550 , 20,20 };
	SDL_Rect btPosBack = { windowW / 2 - 60 + 480, windowH / 2 + 650 , 190,50 };



	SDL_Rect livesBox = { windowW / 2 - 60 + 120, windowH / 2 + 50 , 40,25 };
	SDL_Rect coinsBox = { windowW / 2 - 60 + 120, windowH / 2 + 150 , 40,25 };
	SDL_Rect timerBoox = { windowW / 2 - 60 + 1000, windowH / 2 + 70 , 40,40 };



	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", btPosExit, this);
	exitButton->state = GuiControlState::DISABLED;
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESUME", btPosResume, this);
	resumeButton->state = GuiControlState::DISABLED;
	backToTitleButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "BACK TO TITLE", btPosBackToTitle, this);
	backToTitleButton->state = GuiControlState::DISABLED;

	settingsButton2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", btPosSettings2, this);
	settingsButton2->state = GuiControlState::DISABLED;


	musicSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 5, "MUSIC", btPosMusicBar, this);
	musicSlider->function = FunctionGUI::MUSIC;
	musicSlider->state = GuiControlState::DISABLED;



	FxSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Fx     ", btPosFxBar, this);
	FxSlider->function = FunctionGUI::MUSIC;
	FxSlider->state = GuiControlState::DISABLED;


	GoBackButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "GO BACK", btPosBack, this);
	GoBackButton->state = GuiControlState::DISABLED;


	FullscreenCheckBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "FULLSCREEN", btPosFull, this);
	FullscreenCheckBox->state = GuiControlState::DISABLED;


	VsyncCheckBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "V-SYNC", btPosVsync, this);
	VsyncCheckBox->state = GuiControlState::DISABLED;

	playerLifesBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 11, " ", livesBox, this);
	playerLifesBox->state = GuiControlState::DISABLED;

	scoreBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 12, " ", coinsBox, this);
	scoreBox->state = GuiControlState::DISABLED;

	timerBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 13, " ", timerBoox, this);
	timerBox->state = GuiControlState::DISABLED;


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
	std::string strPlayerLifes = std::to_string(player->lives);
	playerLifesBox->SetValue(strPlayerLifes);
	std::string coins = std::to_string(player->coinCount);
	scoreBox->SetValue(coins);
	std::string seconds = std::to_string(player->timerGod);
	timerBox->SetValue(seconds);


	app->scene->player->timerFrames++;
	if (app->scene->player->timerFrames == 60) {

		app->scene->player->timerGod= app->scene->player->timerGod+1;

		app->scene->player->timerFrames = 0;
	}

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

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
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

	float timeSinceLivesZero = 0.0f;
	bool isWaitingAfterLivesZero = false;
	float delayAfterLivesZero = 25.0f;

	if (app->scene->player->lives == 0 && !isWaitingAfterLivesZero)
	{
		isWaitingAfterLivesZero = true;
		timeSinceLivesZero = 0.0f;
	}

	if (isWaitingAfterLivesZero)
	{
		timeSinceLivesZero += dt;

		if (timeSinceLivesZero >= delayAfterLivesZero)
		{
			
			active = false;
			app->map->active = false;
			app->entityManager->active = false;
			app->guiManager->active = false;
			app->gameover->active = true;

			
			isWaitingAfterLivesZero = false;
			timeSinceLivesZero = 0.0f;
		}
	}

	float timeSinceBossDefeated = 0.0f;
	bool isWaitingAfterBossDefeated = false;
	float delayAfterBossDefeated = 20.0f;

	if (boss->health == 0 && !isWaitingAfterBossDefeated)
	{
		isWaitingAfterBossDefeated = true;
		timeSinceBossDefeated = 0.0f;
	}

	if (isWaitingAfterBossDefeated)
	{
		timeSinceBossDefeated += dt;

		if (timeSinceBossDefeated >= delayAfterBossDefeated)
		{
			
			active = false;
			app->map->active = false;
			app->entityManager->active = false;
			app->guiManager->active = false;
			app->winFinal->active = true;

			
			isWaitingAfterBossDefeated = false;
			timeSinceBossDefeated = 0.0f;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if (ext == true) {
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

	player->position.x = node.child("player").attribute("lastCheckpointX").as_int();
	player->position.y = node.child("player").attribute("lastCheckpointY").as_int();
	player->lives = node.child("player").attribute("lives").as_int();
	b2Vec2 newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	player->lastCheckpoint.x = newPos.x;
	player->lastCheckpoint.y = newPos.y;

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
		if (checkAlive == false) {

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
	//boss---------------------------------------------------------------------------------------
	if (boss->isalive == true) {
		boss->position.x = node.child("boss").attribute("x").as_int();
		boss->position.y = node.child("boss").attribute("y").as_int();
		boss->health = node.child("boss").attribute("health").as_int();
		newPos = b2Vec2(PIXEL_TO_METERS(boss->position.x), PIXEL_TO_METERS(boss->position.y));
		boss->pbody->body->SetTransform(newPos, boss->pbody->body->GetAngle());
		checkAlive = node.child("boss").attribute("estavivo").as_bool();
		if (boss->isalive && checkAlive == false) {

			boss->health = 10;
			boss->finalposition.x = METERS_TO_PIXELS(boss->pbody->body->GetTransform().p.x) - 16;
			boss->finalposition.y = METERS_TO_PIXELS(boss->pbody->body->GetTransform().p.y) - 16;
			boss->pbody->body->SetActive(false);
			boss->muriendo = 0;
			boss->isalive = false;
		}
	}
	//checkpoint---------------------------------------------------------------------------------
	int i = 0;
	for (pugi::xml_node CheckpointNode = node.child("checkpoint"); CheckpointNode; CheckpointNode = CheckpointNode.next_sibling("chekpoint"))
	{
		listOfCheckpoints[i]->isPicked = CheckpointNode.attribute("isPicked").as_bool();
		i++;
	}

	//coin----------------------------------------------------------------------------------------
	i = 0;
	for (pugi::xml_node CoinNode = node.child("coin"); CoinNode; CoinNode = CoinNode.next_sibling("coin"))
	{
		bool aux = listOfCoins[i]->isPicked;

		listOfCoins[i]->isPicked = CoinNode.attribute("isPicked").as_bool();
		if (!aux && listOfCoins[i]->isPicked) {
			app->physics->DestroyObject(listOfCoins[i]->pbody);
		}
		i++;
	}
	//healthitem--------------------------------------------------------------------------------------------
	i = 0;
	for (pugi::xml_node healthNode = node.child("healthitem"); healthNode; healthNode = healthNode.next_sibling("healthitem"))
	{
		bool aux = listOfHealtthitems[i]->isPicked;

		listOfHealtthitems[i]->isPicked = healthNode.attribute("isPicked").as_bool();
		if (!aux && listOfHealtthitems[i]->isPicked) {
			app->physics->DestroyObject(listOfHealtthitems[i]->pbody);
		}
		i++;
	}
	return true;

}


bool Scene::SaveState(pugi::xml_node node) {

	pugi::xml_node enemyNode = node.append_child("enemy");
	pugi::xml_node enemy2Node = node.append_child("enemy2");
	pugi::xml_node enemy3Node = node.append_child("enemy3");
	pugi::xml_node enemy4Node = node.append_child("enemy4");
	pugi::xml_node enemy5Node = node.append_child("enemy5");
	pugi::xml_node flyerNode = node.append_child("flyer");
	pugi::xml_node flyer2Node = node.append_child("flyer2");
	pugi::xml_node flyer3Node = node.append_child("flyer3");
	pugi::xml_node playerNode = node.append_child("player");
	pugi::xml_node bossNode = node.append_child("boss");

	//player-----------------------------------------------------------------
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("lives").set_value(player->lives);
	playerNode.append_attribute("lastCheckpointX").set_value(player->lastCheckpoint.x);
	playerNode.append_attribute("lastCheckpointY").set_value(player->lastCheckpoint.y);

	//esqueletos---------------------------------------------------------------------------------------

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
	//boss---------------------------------------------------------------------------------------------
	bossNode.append_attribute("x").set_value(boss->position.x);
	bossNode.append_attribute("y").set_value(boss->position.y);
	bossNode.append_attribute("health").set_value(boss->health);
	bossNode.append_attribute("estavivo").set_value(boss->isalive);
	//checkpoints----------------------------------------------------------------
	for (int i = 0; i < listOfCheckpoints.Count(); i++) {
		pugi::xml_node CheckpointNode = node.append_child("checkpoint");
		CheckpointNode.append_attribute("x").set_value(listOfCheckpoints[i]->position.x);
		CheckpointNode.append_attribute("y").set_value(listOfCheckpoints[i]->position.y);
		CheckpointNode.append_attribute("x").set_value(player->lastCheckpoint.x);
		CheckpointNode.append_attribute("y").set_value(player->lastCheckpoint.y);
		CheckpointNode.append_attribute("isPicked").set_value(listOfCheckpoints[i]->isPicked);
	}

	//coin---------------------------------------------------------------------------------------------

	for (int i = 0; i < listOfCoins.Count(); i++) {
		pugi::xml_node CoinNode = node.append_child("coin");
		CoinNode.append_attribute("x").set_value(listOfCoins[i]->position.x);
		CoinNode.append_attribute("y").set_value(listOfCoins[i]->position.y);
		CoinNode.append_attribute("isPicked").set_value(listOfCoins[i]->isPicked);
	}
	//healthitem--------------------------------------------------------------------------------------------

	for (int i = 0; i < listOfHealtthitems.Count(); i++) {
		pugi::xml_node HealthNode = node.append_child("healtitem");
		HealthNode.append_attribute("x").set_value(listOfHealtthitems[i]->position.x);
		HealthNode.append_attribute("y").set_value(listOfHealtthitems[i]->position.y);
		HealthNode.append_attribute("isPicked").set_value(listOfHealtthitems[i]->isPicked);
	}
	return true;
}
bool  Scene::OnGuiMouseClickEvent(GuiControl* control) {
	if (control->id == 1) {
		//app->audio->active = true;

		ext = true;

	}
	//resume 2
	if (control->id == 2) {
		app->scene->player->pausa = false;
		app->scene->resumeButton->state = GuiControlState::DISABLED;
		app->scene->exitButton->state = GuiControlState::DISABLED;
		app->scene->backToTitleButton->state = GuiControlState::DISABLED;
		app->scene->settingsButton2->state = GuiControlState::DISABLED;
		app->scene->playerLifesBox->state = GuiControlState::NORMAL;
		app->scene->scoreBox->state = GuiControlState::NORMAL;
		app->scene->timerBox->state = GuiControlState::NORMAL;
	}
	//back to title screen 3
	if (control->id == 9) {

		app->scene->player->pausa = false;
		active = false;
		app->menu->active = true;
		app->entityManager->active = false;
		app->map->active = false;
		app->scene->player->SetPosition(163, 665);

		app->menu->startButton->state = GuiControlState::NORMAL;
		app->menu->exitButton->state = GuiControlState::NORMAL;
		app->menu->settingsButton->state = GuiControlState::NORMAL;
		app->menu->continueButton->state = GuiControlState::NORMAL;
		app->menu->creditsButton->state = GuiControlState::NORMAL;
		app->scene->resumeButton->state = GuiControlState::DISABLED;
		app->scene->exitButton->state = GuiControlState::DISABLED;
		app->scene->backToTitleButton->state = GuiControlState::DISABLED;



	}
	if (control->id == 3) {

		musicSlider->state = GuiControlState::NORMAL;
		FxSlider->state = GuiControlState::NORMAL;
		GoBackButton->state = GuiControlState::NORMAL;
		FullscreenCheckBox->state = GuiControlState::NORMAL;
		VsyncCheckBox->state = GuiControlState::NORMAL;


		exitButton->state = GuiControlState::DISABLED;
		app->scene->resumeButton->state = GuiControlState::DISABLED;
		app->scene->exitButton->state = GuiControlState::DISABLED;
		app->scene->backToTitleButton->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;





	}
	if (control->id == 5) {

		app->audio->ChangeMusicVolume(musicSlider->newValue);

	}
	if (control->id == 6) {

		app->audio->ChangeFxVolume(FxSlider->newValue);

	}
	if (control->id == 7) {

		musicSlider->state = GuiControlState::DISABLED;
		FxSlider->state = GuiControlState::DISABLED;
		GoBackButton->state = GuiControlState::DISABLED;
		FullscreenCheckBox->state = GuiControlState::DISABLED;
		VsyncCheckBox->state = GuiControlState::DISABLED;


		exitButton->state = GuiControlState::NORMAL;
		settingsButton2->state = GuiControlState::NORMAL;
		resumeButton->state = GuiControlState::NORMAL;
		backToTitleButton->state = GuiControlState::NORMAL;



	}
	if (control->id == 8) {
		if (app->menu->fullscreen == false) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			app->menu->fullscreen = true;
		}
		else {
			uint heigth;
			uint width;

			app->win->GetWindowSize(width, heigth);

			SDL_SetWindowFullscreen(app->win->window, 0);
			SDL_SetWindowSize(app->win->window, width, heigth);
			app->menu->fullscreen = false;
		}



	}
	if (control->id == 9) {

		app->entityManager->active = false;
		app->map->active = false;
		app->scene->active = false;
		//app->physics->active = false;
		exitButton->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;

		active = false;

	}
	if (control->id == 10)
	{
		if (app->maxFrameDuration == 1000 / 60)
		{
			app->maxFrameDuration = 1000 / 30;
		}
		else
		{
			app->maxFrameDuration = 1000 / 60;
		}
	}
	return true;
}