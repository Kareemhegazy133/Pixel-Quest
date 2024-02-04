#include <simple_logger.h>
#include <gf2d_graphics.h>
#include <gf2d_sprite.h>

#include <pq_world.h>

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
			if (world->tile_map[index] == 0) continue;

			Vector2D position = vector2d(j * world->tile_set->frame_w, i * world->tile_set->frame_h);
			Uint32 frame = world->tile_map[index] - 1;
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
		world->tile_map[i * width] = 1;
		world->tile_map[i * width + (width - 1)] = 1;
	}

	for (int i = 0; i < width; i++)
	{
		world->tile_map[i] = 1;
		world->tile_map[i + (height - 1) * width] = 1;
	}

	pq_world_build_tile_layer(world);
	return world;
}

pq_world* new_pq_world(Uint32 width, Uint32 height)
{
	if (!width || !height)
	{
		slog("width and/or height = NULL, Cannot create a pq_world entity without a width and/or height.");
		return NULL;
	}

	pq_world* world;

	world = gfc_allocate_array(sizeof(pq_world), 1);
	if (!world) {
		slog("world = NULL, Failed to create a pq_world entity.");
		return NULL;
	}

	world->tile_map = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tile_height = height;
	world->tile_width = width;
	return world;
}

void free_pq_world(pq_world* world)
{
	if (!world) return;
	
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tile_set);
	gf2d_sprite_free(world->tile_layer);
	free(world->tile_map);
	free(world);
}

void draw_pq_world(pq_world* world)
{
	if (!world) return;

	gf2d_sprite_draw_image(world->background, vector2d(0, 0));

	if (!world->tile_layer)
	{
		slog("world->tile_layer = NULL, Cannot draw world with no tile_layer.");
		return;
	}

	gf2d_sprite_draw_image(world->tile_layer, vector2d(0, 0));
}
