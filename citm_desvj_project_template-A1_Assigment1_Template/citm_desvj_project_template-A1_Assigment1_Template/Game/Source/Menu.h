#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include "flyer.h"
#include "Checkpoint.h"
#include "Boss.h"

#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiManager.h"

struct SDL_Texture;

class Menu : public Module
{
public:

	Menu();

	// Destructor
	virtual ~Menu();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node node);

	void Drawline();

	bool SaveState(pugi::xml_node node);
public:
	Player* player;
	Enemy* enemy;
	Enemy* enemy2;
	Enemy* enemy3;
	Enemy* enemy4;
	Enemy* enemy5;
	Flyer* flyer;
	Flyer* flyer2;
	Flyer* flyer3;
	Boss* boss;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* pantallaInicio;
	Checkpoint* checkpoint;
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	
	bool debugcamera= false;

	GuiControlButton* exitButton;

};

#endif // __SCENE_H__