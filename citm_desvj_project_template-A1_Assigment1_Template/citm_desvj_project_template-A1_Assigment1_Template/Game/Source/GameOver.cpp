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
#include "Player.h"


#include "GuiControl.h"
#include "GuiManager.h"

GO::GO() : Module()
{
	name.Create("gameover");
}

// Destructor
GO::~GO()
{}

// Called before render is available
bool GO::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access


	

	return ret;
}

// Called before the first frame
bool GO::Start()
{

	pantallaGameOver = app->tex->Load("Assets/Pantallas/SWMG_PantallaGameOver.png");

	return true;
}

// Called each loop iteration
bool GO::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool GO::Update(float dt)
{
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->DrawTexture(pantallaGameOver, 0, 0, false);

	return true;
}

// Called each loop iteration
bool GO::PostUpdate()
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
bool GO::CleanUp()
{
	LOG("Freeing Menu");

	return true;
}
