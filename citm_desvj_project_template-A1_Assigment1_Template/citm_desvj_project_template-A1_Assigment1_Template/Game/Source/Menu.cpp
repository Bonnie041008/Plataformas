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
	
	
	if (config.child("player")) {
		
		int x  = config.child("player").attribute("x").as_int();
	}
	
	
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
	fondobotonesMenu = app->tex->Load("Assets/Textures/FondosBotones/FondoBotones-Menu.png");
	pantallaInicio = app->tex->Load("Assets/Pantallas/SWMG_PantallaDeInicio.png");	
	pantallaCredits = app->tex->Load("Assets/Pantallas/SWMG-Credits.png");
	

	SDL_Rect btPosStart = { windowW / 2 - 70, windowH / 2 - 80, 180,40 };
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START", btPosStart, this);
	
	SDL_Rect btPosContinue = { windowW / 2 - 70, windowH / 2 - 10, 140,40 };
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "CONTINUE", btPosContinue, this);

	SDL_Rect btPosExit = { windowW / 2 - 70, windowH / 2 + 250, 140,40 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "EXIT", btPosExit, this);
	

	SDL_Rect btPosSettings = { windowW / 2 - 70, windowH / 2 + 90, 140,40 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", btPosSettings, this);

	SDL_Rect btPosCredits = { windowW / 2 - 70, windowH / 2 + 170, 140,40 };
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "CREDITS", btPosCredits, this);

	
	
	
	SDL_Rect btPosMusicBar = { windowW / 2 - 60, windowH / 2 - 50, 120,20 };
	musicSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 5, "MUSIC", btPosMusicBar, this);
	musicSlider->function = FunctionGUI::MUSIC;
	musicSlider->state = GuiControlState::DISABLED;

	SDL_Rect btPosFxBar = { windowW / 2 - 60, windowH / 2 + 50 , 120,20 };
	FxSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Fx", btPosFxBar, this);
	FxSlider->function = FunctionGUI::MUSIC;
	FxSlider->state = GuiControlState::DISABLED;

	SDL_Rect btPosBack = { windowW / 2 - 70, windowH / 2 + 250, 140,40 };
	GoBackButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "GO BACK", btPosBack, this);
	GoBackButton->state = GuiControlState::DISABLED;

	SDL_Rect btPosFull = { windowW / 2 + 70, windowH / 2 +130 , 20,20 };
	FullscreenCheckBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "FULLSCREEN", btPosFull, this);
	FullscreenCheckBox->state = GuiControlState::DISABLED;

	SDL_Rect btPosVsync = { windowW / 2 + 70, windowH / 2 + 180 , 20,20 };
	VsyncCheckBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "V-SYNC", btPosVsync, this);
	VsyncCheckBox->state = GuiControlState::DISABLED;



	SDL_Rect btPosBackCredits = { windowW / 2 - 60, windowH / 2 + 200 , 120,20 };
	GoBackCreditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "GO BACK", btPosBack, this);
	GoBackCreditsButton->state = GuiControlState::DISABLED;



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
	app->render->DrawTexture(fondobotonesMenu, windowW / 2 - 295 / 2, windowH / 2 - 300 / 2, false);

	
	app->scene->player->SetPosition(163, 665);
	
	if (credits == true)
	{
		app->render->DrawTexture(pantallaCredits, 0, 0, false);
	}

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
		continueButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;
		
		active = false;
	}
	if (control->id == 2) {
		exitButton->function = FunctionGUI::EXIT;
	}
	if (control->id == 3) {
		
		musicSlider->state = GuiControlState::NORMAL;
		FxSlider->state = GuiControlState::NORMAL;
		GoBackButton->state = GuiControlState::NORMAL;
		FullscreenCheckBox->state = GuiControlState::NORMAL;
		VsyncCheckBox->state = GuiControlState::NORMAL;

		startButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
		continueButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;
	

		

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
		GoBackCreditsButton->state = GuiControlState::DISABLED;

		startButton->state = GuiControlState::NORMAL;
		exitButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::NORMAL;
		continueButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;

		credits = false;


	}
	if (control->id == 8) {
		if (fullscreen == false) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			fullscreen = true;
		}
		else {
			uint heigth;
			uint width;

			app->win->GetWindowSize(width, heigth);

			SDL_SetWindowFullscreen(app->win->window, 0);
			SDL_SetWindowSize(app->win->window, width, heigth);
			fullscreen = false;
		}
	
	

	}
	if (control->id == 9) {

		app->entityManager->active = true;
		app->map->active = true;
		app->scene->active = true;
		startButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
		continueButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;
		active = false;
		app->LoadRequest();


	}
	if (control->id == 10) {

		


	}
	if (control->id == 11) {

		startButton->state = GuiControlState::DISABLED;
		exitButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
		continueButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;

		GoBackCreditsButton->state = GuiControlState::NORMAL;

		credits = true;
		
		


	}

	return true;
}