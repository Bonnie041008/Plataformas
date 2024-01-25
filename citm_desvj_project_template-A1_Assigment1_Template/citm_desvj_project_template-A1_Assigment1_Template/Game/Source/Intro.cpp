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
#include "Intro.h"


#include "GuiControl.h"
#include "GuiManager.h"
Intro::Intro() : Module()
{
	name.Create("scene");
}

// Destructor
Intro::~Intro()
{}

// Called before render is available
bool Intro::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access


	

	return ret;
}

// Called before the first frame
bool Intro::Start()
{

	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;
	app->menu->active = false;
	app->guiManager->active = false;

	pantallaIntro = app->tex->Load("Assets/Pantallas/SWMG-Intro.png");

	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Intro::Update(float dt)
{
	
	app->render->DrawTexture(pantallaIntro, 0, 0, false);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		active = false;
		app->menu->active = true;
		app->guiManager->active = true;

	}


	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{
	bool ret = true;
	if (ext == true) {
		ret = false;
	}
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (app->menu->exitButton->exit == true) {
		ret = false;
	}
	return ret;
}


// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing Menu");

	return true;
}
