#include "Globals.h"
#include <string>

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/Background.png");
	circle = App->textures->Load("pinball/ball.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	//bgm = App->audio->PlayMusic("pinball/PinballBG.ogg", 1);
	backgrounds = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);

	//circles.add(App->physics->CreateCircle(400, 500, 8.5));
	//circles.getLast()->data->listener = this;

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(circle);
	App->textures->Unload(box);
	App->textures->Unload(rick);
	App->audio->CleanUp();
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 8.5, b2_dynamicBody, 0.0f, 1.0f));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	App->renderer->Blit(background, 0, 0, &board_rect);

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[102] = {
	500/2, 800/2,
	500/2, 119/2,
	0/2, 120/2,
	0/2, 800/2,
	0/2, 800/2,
	0/2, 197/2,
	60/2, 146/2,
	60/2, 370/2,
	15/2, 422/2,
	15/2, 665/2,
	150/2, 749/2,
	150/2, 740/2,
	30/2, 663/2,
	20/2, 428/2,
	30/2, 429/2,
	25/2, 418/2,
	50/2, 377/2,
	70/2, 384/2,
	70/2, 377/2,
	60/2, 370/2,
	60/2, 135/2,
	90/2, 119/2,
	130/2, 99/2,
	280/2, 86/2,
	221/2, 81/2,
	260/2, 84/2,
	302/2, 93/2,
	341/2, 109/2,
	374/2, 131/2,
	405/2, 153/2,
	433/2, 181/2,
	455/2, 211/2,
	477/2, 249/2,
	486/2, 272/2,
	487/2, 730/2,
	476/2, 734/2,
	480/2, 740/2,
	480/2, 740/2,
	487/2, 80 / 2,
	450/2, 800/2,
	455/2, 740/2,
	466/2, 740/2,
	463/2, 732/2,
	455/2, 731/2,
	453/2, 402/2,
	443/2, 400/2,
	446/2, 643/2,
	329/2, 718/2,
	333/2, 723/2,
	445/2, 657/2,
	45 / 2, 800 / 2
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 12, b2_dynamicBody, 0.0f));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
