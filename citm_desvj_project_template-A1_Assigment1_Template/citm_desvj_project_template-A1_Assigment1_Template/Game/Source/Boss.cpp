#include "Enemy.h"
#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Player.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
	//idle
	idleAnim.PushBack({ 0, 0, 160, 160 });
	idleAnim.PushBack({ 160, 0, 160, 160 });
	idleAnim.PushBack({ 320, 0, 160, 160 });
	idleAnim.PushBack({ 480, 0, 160, 160 });
	
	
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;


	//idle parry
	idleAnim2.PushBack({ 0, 640, 160, 160 });
	idleAnim2.PushBack({ 160, 640, 160, 160 });
	idleAnim2.PushBack({ 320, 640, 160, 160 });
	idleAnim2.PushBack({ 480, 640, 160, 160 });


	idleAnim2.loop = true;
	idleAnim2.speed = 0.1f;

	//ataque
	attackAnim.PushBack({ 0,160,160,160 });
	attackAnim.PushBack({ 160,160,160,160 });
	attackAnim.PushBack({ 320,160,160,160 });
	attackAnim.PushBack({ 480,160,160,160 });
	attackAnim.PushBack({ 640,160,160,160 });
	
	

	attackAnim.loop = false;
	attackAnim.speed = 0.1f;

	//ataque parry
	attackAnim2.PushBack({ 0,800,160,160 });
	attackAnim2.PushBack({ 160,800,160,160 });
	attackAnim2.PushBack({ 320,800,160,160 });
	attackAnim2.PushBack({ 480,800,160,160 });
	attackAnim2.PushBack({ 640,800,160,160 });



	attackAnim2.loop = false;
	attackAnim2.speed = 0.1f;

	//dead
	deadAnim.PushBack({ 0,480,160,160 });
	deadAnim.PushBack({ 160,480,160,160 });
	deadAnim.PushBack({ 320,480,160,160 });
	deadAnim.PushBack({ 480,480,160,160 });
	deadAnim.PushBack({ 640,480,160,160 });
	
	
	deadAnim.loop = false;
	deadAnim.speed = 0.2f;

	//correr
	rightAnim.PushBack({ 0,320,160,160 });
	rightAnim.PushBack({ 160,320,160,160 });
	rightAnim.PushBack({ 320,320,160,160 });
	rightAnim.PushBack({ 480,320,160,160 });
	

	rightAnim.loop = true;
	rightAnim.speed = 0.2f;

	//correr parry
	rightAnim2.PushBack({ 0,960,160,160 });
	rightAnim2.PushBack({ 160,960,160,160 });
	rightAnim2.PushBack({ 320,960,160,160 });
	rightAnim2.PushBack({ 480,960,160,160 });


	rightAnim2.loop = true;
	rightAnim2.speed = 0.2f;
}

Boss::~Boss() {

}

bool Boss::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	initialX = position.x;
	initialY = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	currentAnimation = &idleAnim;
	walkingRange = 1000;
	return true;
}

bool Boss::Start() {

	//initilize textures
	texture = app->tex->Load("Assets/Textures/Boss_Spritesheet2-export.png");
	bossRip = app->tex->Load("Assets/Textures/bossRip.png");
	pbody = app->physics->CreateCircle(position.x , position.y , 17, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::BOSS;
	b2Vec2 vel = b2Vec2(move_x, move_y);
	move_x = 0;
	move_y = -GRAVITY_Y;

	pantallaWin = app->tex->Load("Assets/Pantallas/SWMG_PantallaYouWin.png");
	
	MuerteBoss = app->audio->LoadFx("Assets/Audio/Fx/BossMuerte.wav");
	AtaqueBoss = app->audio->LoadFx("Assets/Audio/Fx/BossAtaqueEspada.wav");
	


	return true;
}

void Boss::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	b2Vec2 newPos(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	pbody->body->SetTransform(newPos, pbody->body->GetAngle());
}

bool Boss::Update(float dt)
{
	
	b2Vec2 vel = b2Vec2(move_x, move_y);
	move_x = 0;
	move_y = -GRAVITY_Y;
	
	// Check if 5 seconds have elapsed
	if (parryToggleTimer >= 150.0f)
	{
		// Toggle ParryMode
		parryMode = !parryMode;

		// Reset the timer
		parryToggleTimer = 0.0f;
	}

	
	if (health == 0 && isalive== true) {
		
		if(Muerte_Boss == true){
			app->audio->PlayFx(MuerteBoss);	
		}
		Muerte_Boss = false;
		currentAnimation = &deadAnim;
		muriendo++;
		movX = 0;
		vel.x = 0;
		pbody->ctype = ColliderType::ITEM;
		
		if (muriendo > 70) {
			isalive = false;
			
			
			finalposition.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			finalposition.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
			app->physics->DestroyObject(pbody);
			
			muriendo = 0;
			
			
		}


	}
	
	if (isalive == true) {
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		
		if (isAttacking == false) {
			app->render->DrawTexture(texture, position.x - 60, position.y - 85, isFliped, &currentAnimation->GetCurrentFrame());
		}
		else {
			if (parryMode == true) {
				app->render->DrawTexture(texture, position.x - 40, position.y - 85, isFliped, &attackAnim2.GetCurrentFrame());
			}
			else {
				app->render->DrawTexture(texture, position.x - 40, position.y - 85, isFliped, &attackAnim.GetCurrentFrame());
			}
			
		}
		
		currentAnimation->Update();

		if (isAttacking == true) {
			if(Ataque_Boss == true){
				app->audio->PlayFx(AtaqueBoss);	
			}
			Ataque_Boss = false;
			
			if (parryMode == true) {
				currentAnimation = &attackAnim2;
			}

			else {
				currentAnimation = &attackAnim;
			}
			
			cntatt++;
			attackAnim.Update();
			if (cntatt > 20 && isAttacking == true) {

				if (parryMode == true) {
					attackAnim2.Reset();
					Ataque_Boss = true;
					isAttacking = false;
					if (parryMode == true) {
						currentAnimation = &idleAnim2;
					}
					else {
						currentAnimation = &idleAnim;
					}
					
					cntatt = 0;
				}
				else {
					attackAnim.Reset();


					Ataque_Boss = true;
					isAttacking = false;
					if (parryMode == true) {
						currentAnimation = &idleAnim2;
					}
					else {
						currentAnimation = &idleAnim;
					}
					cntatt = 0;
				}
				
			}
		}
	}
	else {
		app->render->DrawTexture(bossRip, finalposition.x - 60, finalposition.y - 85, isFliped);
	}
	parryToggleTimer += 1;


	//Set the velocity of the pbody of the enemy

	if (isalive == true) {
		
		if (app->scene->player->position.x + 20 > initialX - walkingRange && app->scene->player->position.x - 20 < initialX + walkingRange) {
			app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y));
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

			if (app->physics->debug == true)
			{
				for (uint i = 0; i < path->Count(); i++)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y, false);


				}

			}

			for (uint i = 0; i < path->Count(); i++) {
				if (isalive == true && health != 0) {
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					movX = (pos.x - this->position.x);

					if (movX < 0) {
						vel.x = -3;
					}
					else if (movX > 0) {
						vel.x = 3;
					}
					else {
						vel.x = 0;
					}
					if (isAttacking == false) {
						if (currentAnimation == &deadAnim) {
							
							vel.x = 0;

						}
						if (vel.x != 0) {
							if (parryMode == true) {
								currentAnimation = &rightAnim2;
							}
							else {
								currentAnimation = &rightAnim;
							}
							
						}

						else {
							if (parryMode == true) {
								currentAnimation = &idleAnim2;
							}
							else {
								currentAnimation = &idleAnim;
							}
							
						}
					}
					else {
						vel.x = 0;
					}

					


					if (position.x > pos.x)
					{
						isFliped = true;
					}
					else
					{
						isFliped = false;
					}



				}
				else {
					movX = 0;
					vel.x = movX;
				}

			}
				
			
				

			
			pbody->body->SetLinearVelocity(vel);
		}
		else if (position.x != initialX ) {
			app->map->pathfinding->ClearLastPath();
			app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y), app->map->WorldToMap(initialX, initialY));
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

			for (uint i = 0; i < path->Count(); i++) {
				if (isalive == true && health != 0) {
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					//app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y, false);
					if (app->physics->debug == true)
					{
						for (uint i = 0; i < path->Count(); i++)
						{
							iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
							app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y, false);


						}

					}
					if (isAttacking == false) {
						movX = (pos.x - this->position.x);
						if (movX < 0) {
							vel.x = -3;
						}
						else if (movX > 0) {
							vel.x = 3;
						}
						else {
							vel.x = 0;
						}
					}
					else {
						vel.x = 0;
					}











					if (position.x > pos.x)
					{
						isFliped = true;
					}
					else
					{
						isFliped = false;
					}
				}
				else {
					movX = 0;
					vel.x = movX;
				}
			}
			

				
			
				


			
			pbody->body->SetLinearVelocity(vel);
		}
		else {

		   if (position.x < initialX - walkingRange) {
			   speed = -3;
		   }
		   else if (position.x > initialX + walkingRange) {
			   speed = +3;
		   }
		   pbody->body->SetLinearVelocity(vel);
		}



	}
	
	/*if (health == 0) {

		app->render->DrawTexture(pantallaWin,0,0,false);

	}*/
	
	

	

	
	
	

	return true;
}

bool Boss::CleanUp()
{

	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (physA->ctype == ColliderType::BOSS) {
		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			
			break;
		case ColliderType::PLAYER:
			LOG("Collision PLAYER");
			isAttacking = true;
			
			
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			
			break;
		case ColliderType::FIREBALL:
			LOG("Collision FIREBALL");
			if (parryMode == true) {
				isAttacking = true;
				b2Vec2 vel = b2Vec2(move_x, move_y);
				move_x = 0;
				move_y = -GRAVITY_Y;
				vel.x = 0;
				physB->body->SetLinearVelocity(-physB->body->GetLinearVelocity());
				physB->ctype = ColliderType::FIREENEMY;
			}
			else {
				health = health-1;

				if (health == 0 && isalive) {
					muriendo++;

					//currentAnimation = &deadAnim;
					//SetPosition(400, 352);
				}
			}
			

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
	
	



}