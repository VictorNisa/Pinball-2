#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	//p2List<PhysBody*> bouncer;
	//p2List<PhysBody*> flipper;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	PhysBody* backgrounds;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	//SDL_Texture* Lflipper;
	//SDL_Texture* Rflipper;
	//SDL_Texture* bouncer;

	SDL_Rect board_rect = { 0,0,480,800 };

	uint bgm;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
