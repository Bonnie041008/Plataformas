#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include "flyer.h"
#include "Checkpoint.h"
#include "Boss.h"
#include"Coin.h"
#include "HealthItem.h"
#include "TP.h"

#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiManager.h"

#include <list>
struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

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
	Checkpoint* checkpoint;
	Checkpoint* checkpoint2;
	GuiControlButton* exitButton;
	GuiControlButton* resumeButton;
	GuiControlButton* backToTitleButton;
	Coin* coin;
	TP* tp;
	List<Checkpoint*> listOfCheckpoints;
	List<Coin*> listOfCoins;
	List<HealthItem*> listOfHealtthitems;
	bool ext;
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	
	bool debugcamera= false;

	

};

#endif // __SCENE_H__