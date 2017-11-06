#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "EntityManager.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(const pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	pugi::xml_node node = config.child("map");
	while (node.attribute("source").as_string() != "") {
		p2SString* new_map = new p2SString;
		new_map->create(node.attribute("source").as_string());
		Map_list.add(new_map);
		node = node.next_sibling("map");
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;
	// LOAD MAPS AND MUSIC HERE
	if (ret == true) ret = App->map->Load(Map_list.start->data->GetString());
	if (ret == true) ret = App->audio->PlayMusic("audio/music/music_sadpiano.ogg");

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Get Mouse Position
	iPoint pos;
	App->input->GetMousePosition(pos.x, pos.y);
	pos = App->map->WorldToMap(pos.x - App->render->camera.x, pos.y - App->render->camera.y );

	// TODO 2.5: Call load / save methods when pressing l/s

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->Trigger_Load();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->Trigger_Save();

	// Extra inputs for assignment
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		App->map->debug_draw = !App->map->debug_draw;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		App->ChangeFPSLimit();
	// TODO 2.Homework Allow for change in volume
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_REPEAT) {
		App->audio->Increase_Master();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_REPEAT) {
		App->audio->Decrease_Master();
	}

	

	// Pathfinding Inputs
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		App->pathfinding->PropagateDijkstra();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		App->pathfinding->PropagateDijkstra();

	if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		App->pathfinding->CreateFPath(App->pathfinding->start, pos);

	if (App->input->GetMouseButtonDown(3) == KEY_DOWN) {
		App->pathfinding->SetStart(pos);
		App->pathfinding->ResetNav();
	}

	// Framerate change
	

	//App->render->Blit(img, 0, 0);
	App->map->Draw();
	App->entities->Draw(dt);
	App->collisions->DebugDraw();

	// TODO 3.7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	
	p2SString title("MapSize:%dx%d TileSize:%dx%d Tilesets:%d Layers:%d Tiles:%d Position:%d %d",
		App->map->Maps->width, App->map->Maps->height,
		App->map->Maps->tilewidth, App->map->Maps->tileheight,
		App->map->Maps->tilesets.count(),
		App->map->Maps->layers.count(),
		App->map->Maps->layers.start->data->size,
		pos.x,pos.y
		);
	
	//App->win->SetTitle(title.GetString());
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}