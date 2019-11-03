#ifndef __MODULEUI_H__
#define __MODULEUI_H__

#include "Module.h"
#include "p2Point.h"
#include "Globals.h"

struct SDL_Texture;
//struct Mix_Chunk;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	virtual ~ModuleUI();

	//LOOP
	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	SDL_Texture* UIgameover = nullptr;

	int UIballs = 5;
	int UIscore = 0;
	int UIprevious = 0;
	int UIhighscore = 0;
	int balls = 5;
	bool lose = false;

	char UItextscore[10];
	char UItextprevious[10];
	char UItexthighscore[10];
	char UItextballs[6];

	bool timer = true;
	int nowtimer = 0;
	int starttime = 0;

};

#endif // __MODULEUI_H__