#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

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
#include "GuiControlSlider.h"
#include "GuiManager.h"
#include "GuiControlCheckBox.h"

#include <list>
struct SDL_Texture;

class GO : public Module
{
public:

	GO();

	// Destructor
	virtual ~GO();

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


public:

	SDL_Texture* pantallaGameOver;
	bool ext;
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	bool debugcamera = false;



};

#endif // __SCENE_H__