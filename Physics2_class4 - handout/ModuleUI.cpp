#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"


//CONSTRUCTOR
ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


//DESTRUCTOR
ModuleUI::~ModuleUI()
{}

//BODY
bool ModuleUI::Start()
{
	LOG("Loading UI...");

	UIgameover = App->textures->Load("pinball/gameover.png");

	UIscore = App->fonts->Load("pinball/fonts.png", "0123456789", 1);
	UIhighscore = App->fonts->Load("pinball/fonts.png", "0123456789", 1);
	UIprevious = App->fonts->Load("pinball/fonts.png", "0123456789", 1);
	UIballs = App->fonts->Load("pinball/fonts.png", "012345", 1);

	return true;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading UI...");
	App->fonts->UnLoad(UIscore);
	App->fonts->UnLoad(UIhighscore);
	App->fonts->UnLoad(UIprevious);
	App->fonts->UnLoad(UIballs);
	
	return true;
}

update_status ModuleUI::Update()
{
	sprintf_s(UItextscore, 10, "%1d", UIscore);
	App->fonts->BlitText(200, 20, UIscore, UItextscore);

	sprintf_s(UItexthighscore, 10, "%1d", UIhighscore);
	App->fonts->BlitText(100, 20, UIscore, UItexthighscore);

	sprintf_s(UItextballs, 6, "%1d", balls);
	App->fonts->BlitText(100, 30, UIballs, UItextballs);

	if (lose = true)
	{
		SDL_Rect rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		App->renderer->DrawQuad(rect, 255, 255, 255, 100);
		App->renderer->Blit(UIgameover, 65, 200, NULL);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate()
{
	if (balls == 0)
	{
		lose = true;
		if (UIscore > UIhighscore)
		{
			UIhighscore = UIscore;
		}
		UIscore = 0;
		balls = 5;
	}

	return UPDATE_CONTINUE;

}
