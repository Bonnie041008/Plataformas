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
#include "Win.h"


#include "GuiControl.h"
#include "GuiManager.h"

Win::Win() : Module()
{
	name.Create("win");
}

// Destructor
Win::~Win()
{}

// Called before render is available
bool Win::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access


	

	return ret;
}

// Called before the first frame
bool Win::Start()
{
	
	
	pantallaWin = app->tex->Load("Assets/Pantallas/SWMG_PantallaYouWin.png");

	return true;
}

// Called each loop iteration
bool Win::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Win::Update(float dt)
{
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->DrawTexture(pantallaWin, 0, 0, false);

	return true;
}

// Called each loop iteration
bool Win::PostUpdate()
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
bool Win::CleanUp()
{
	LOG("Freeing Menu");

	return true;
}
