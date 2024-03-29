#include <simple_logger.h>

#include <pq_camera.h>
#include <pq_world.h>
#include <pq_item.h>
#include <pq_enemy.h>

static pq_world* g_pq_world = NULL;  // Global variable to store the world pointer

void pq_world_build_tile_layer(pq_world* world)
{
	if (!world) return;

	if (world->tile_layer)
	{
		gf2d_sprite_free(world->tile_layer);
	}

	world->tile_layer = gf2d_sprite_new();

	world->tile_layer->surface = gf2d_graphics_create_surface(world->tile_width * world->tile_set->frame_w, world->tile_height * world->tile_set->frame_h);
	world->tile_layer->frame_w = world->tile_width * world->tile_set->frame_w;
	world->tile_layer->frame_h = world->tile_height * world->tile_set->frame_h;

	if (!world->tile_layer->surface)
	{
		slog("world->tile_layer->surface = NULL, Failed to create a pq_world tile layer surface.");
		return;
	}

	if (!world->tile_set)
	{
		slog("world->tile_set = NULL, Cannot build world tile layer without tile_set.");
		return;
	}

	for (int i = 0; i < world->tile_height; i++)
	{
		for (int j = 0; j < world->tile_width; j++)
		{
			Uint32 index = j + (i * world->tile_width);
			if (world->map[index] == 0) continue;

			Vector2D position = vector2d(j * world->tile_set->frame_w, i * world->tile_set->frame_h);
			Uint32 frame = world->map[index] - 1;
			gf2d_sprite_draw_to_surface(world->tile_set, position, NULL, NULL, frame, world->tile_layer->surface);
		}
	}

	world->tile_layer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tile_layer->surface);
	if (!world->tile_layer->texture)
	{
		slog("world->tile_layer->texture = NULL, Failed to create a pq_world tile layer texture.");
		return;
	}
}

void load_world_pq_items(SJson* world_json, pq_world* world)
{
	SJson* itemsList = sj_object_get_value(world_json, "items");
	if (!itemsList)
	{
		slog("This world_json is missing the items object.");
		return;
	}

	for (int i = 0; i < sj_array_get_count(itemsList); i++)
	{
		SJson* item_data = sj_array_get_nth(itemsList, i);
		if (!item_data) continue;

		pq_entity* item = new_pq_item(item_data);
		if (!item) continue;

		world->items[world->items_count] = item;
		world->items_count++;
		slog("Loaded an Item: %s", item->display_name);
	}
}

void load_world_pq_enemies(SJson* world_json, pq_world* world)
{
	SJson* enemiesList = sj_object_get_value(world_json, "enemies");
	if (!enemiesList)
	{
		slog("This world_json is missing the enemies object.");
		return;
	}

	for (int i = 0; i < sj_array_get_count(enemiesList); i++)
	{
		SJson* enemy_data = sj_array_get_nth(enemiesList, i);
		if (!enemy_data) continue;

		pq_entity* enemy = new_pq_enemy(enemy_data);
		if (!enemy) continue;

		world->enemies[world->enemies_count] = enemy;
		world->enemies_count++;
		slog("Loaded an Enemy: %s", enemy->name);
	}
}

pq_world* load_pq_world(const char* file_name)
{
	if (!file_name)
	{
		slog("file_name = NULL, Cannot load world without a file name.");
		return NULL;
	}

	pq_world* world;
	SJson* map_file_json = sj_load(file_name);

	if (!map_file_json)
	{
		slog("map_file_json = NULL, Failed to load world map file.");
		return NULL;
	}

	SJson* world_json = sj_object_get_value(map_file_json, "world");
	if (!world_json)
	{
		slog("%s is missing the world object.", file_name);
		sj_free(map_file_json);
		return NULL;
	}

	SJson* vertical = sj_object_get_value(world_json, "map");
	if (!vertical)
	{
		slog("%s is missing the map object.", file_name);
		sj_free(map_file_json);
		return NULL;
	}
	int h = sj_array_get_count(vertical);

	SJson* horizontal = sj_array_get_nth(vertical, 0);
	if (!horizontal)
	{
		slog("Failed to get first horizontal row");
		sj_free(map_file_json);
		return NULL;
	}
	int w = sj_array_get_count(horizontal);

	world = new_pq_world(w, h);
	if (!world)
	{
		slog("Failed to allocate space for a new world for file: %s", file_name);
		sj_free(map_file_json);
		return NULL;
	}

	for (int i = 0; i < h; i++)
	{
		horizontal = sj_array_get_nth(vertical, i);
		if (!horizontal) continue;
		for (int j = 0; j < w; j++)
		{
			SJson* item = sj_array_get_nth(horizontal, j);
			if (!item) continue;
			int sprite_number = 0;
			sj_get_integer_value(item, &sprite_number);
			world->map[j + i * w] = sprite_number;
		}
	}
	const char* background = sj_object_get_value_as_string(world_json, "background");
	world->background = gf2d_sprite_load_image(background);
	const char* tile_set = sj_object_get_value_as_string(world_json, "tile_set");
	int frame_width;
	sj_object_get_value_as_int(world_json, "frame_width", &frame_width);
	int frame_height;
	sj_object_get_value_as_int(world_json, "frame_height", &frame_height);
	int frames_per_line;
	sj_object_get_value_as_int(world_json, "frames_per_line", &frames_per_line);

	world->tile_set = gf2d_sprite_load_all(tile_set, frame_width, frame_height, frames_per_line, 1);
	pq_world_build_tile_layer(world);

	load_world_pq_items(world_json, world);
	slog("Loaded all world items.");
	load_world_pq_enemies(world_json, world);
	slog("Loaded all world enemies.");

	sj_free(map_file_json);
	slog("Loaded World");
	return world;
}

pq_world* test_new_world()
{
	pq_world* world;
	int width = 80, height = 45;

	world = new_pq_world(width, height);

	if (!world) {
		slog("world = NULL, Failed to create a pq_world entity.");
		return NULL;
	}
	world->background = gf2d_sprite_load_image("images/backgrounds/Nature_1_1280x720.png");
	world->tile_set = gf2d_sprite_load_all("images/tilesets/green.png", 16, 16, 1, 1);

	for (int i = 0; i < height; i++)
	{
		world->map[i * width] = 1;
		world->map[i * width + (width - 1)] = 1;
	}

	for (int i = 0; i < width; i++)
	{
		world->map[i] = 1;
		world->map[i + (height - 1) * width] = 1;
	}

	pq_world_build_tile_layer(world);
	setup_camera_pq_world(world);
	return world;
}

pq_world* new_pq_world(Uint32 width, Uint32 height)
{
	if (!width || !height)
	{
		slog("width and/or height = NULL, Cannot create a pq_world entity without a width and/or height.");
		return NULL;
	}

	pq_world* world = gfc_allocate_array(sizeof(pq_world), 1);
	if (!world) {
		slog("world = NULL, Failed to create a pq_world entity.");
		return NULL;
	}

	world->map = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tile_height = height;
	world->tile_width = width;

	world->items_count = 0;
	memset(world->items, 0, sizeof(pq_entity*) * MAX_ITEMS);

	world->enemies_count = 0;
	memset(world->enemies, 0, sizeof(pq_entity*) * MAX_ENEMIES);

	set_pq_world(world);
	return world;
}

pq_world* get_pq_world()
{
	return g_pq_world;
}

void set_pq_world(pq_world* world) {
	g_pq_world = world;
}

void free_pq_world(pq_world* world)
{
	if (!world) return;
	
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tile_set);
	gf2d_sprite_free(world->tile_layer);
	free(world->map);
	free(world);
}

void draw_pq_world(pq_world* world)
{
	if (!world) return;

	Vector2D offset  = pq_camera_get_offset();

	gf2d_sprite_draw_image(world->background, vector2d(0, 0));

	if (!world->tile_layer)
	{
		slog("world->tile_layer = NULL, Cannot draw world with no tile_layer.");
		return;
	}

	gf2d_sprite_draw_image(world->tile_layer, offset);
}

void setup_camera_pq_world(pq_world* world)
{
	if (!world) return;

	if (!world->tile_layer || !world->tile_layer->surface)
	{
		slog("world->tile_layer or world->tile_layer->surface = NULL, Cannot setup camera with no tile_layer with surface.");
		return;
	}
	pq_camera_set_bounds(gfc_rect(0, 0, world->tile_layer->surface->w, world->tile_layer->surface->h));
	pq_camera_apply_bounds();
	pq_camera_enable_binding(1);
}
