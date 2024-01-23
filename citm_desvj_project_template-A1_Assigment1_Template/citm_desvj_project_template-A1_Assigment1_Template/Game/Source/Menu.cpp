#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Menu.h"
#include "Map.h"
#include "Physics.h"
#include "Defs.h"
#include "Log.h"
#include "Scene.h"
#include "GuiControl.h"
#include "GuiManager.h"


Menu::Menu() : Module()
{
	name.Create("menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake(pugi::xml_node& config)
{
	bool ret = true;

	

	

	return ret;
}

// Called before the first frame
bool Menu::Start()
{
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;



	//app->audio->PlayMusic("Assets/Audio/Music/Sonido-de-Fondo.wav");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	//Pantallas
	pantallaInicio = app->tex->Load("Assets/Pantallas/SWMG_PantallaDeInicio.png");	


	SDL_Rect btPosStart = { windowW / 2 - 60, windowH / 2 - 100, 120,20 };
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START", btPosStart, this);
	

	SDL_Rect btPosExit = { windowW / 2 - 60, windowH / 2 + 100, 120,20 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "EXIT", btPosExit, this);
	

	SDL_Rect btPosSettings = { windowW / 2 - 60, windowH / 2 + 0, 120,20 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", btPosSettings, this);


	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	app->render->DrawTexture(pantallaInicio, 0, 0, false);
	
	

	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
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
bool Menu::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
bool Menu::LoadState(pugi::xml_node node) {
	
	

	return true;
	
}


bool Menu::SaveState(pugi::xml_node node) {
	


	return true;
}

bool  Menu:: OnGuiMouseClickEvent(GuiControl* control) {
	if (control->id == 1) {
		//app->audio->active = true;
		app->entityManager->active = true;
		app->map->active = true;
		app->scene->active = true;
		startButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
		
		active = false;
	}
	if (control->id == 2) {
		exitButton->function = FunctionGUI::EXIT;
	}
	return true;
}