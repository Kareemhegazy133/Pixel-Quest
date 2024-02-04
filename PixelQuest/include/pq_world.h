#pragma once

typedef struct
{
	Sprite* background;			// This is the background image for the world
	Sprite* tile_layer;			// This is the sprite containing a prerendered layer of the tiles for the world
	Sprite* tile_set;			// This is the sprite containing tiles for the world
	Uint8*	tile_map;			// This is the tiles that make up the world
	Uint32	tile_height;		// This is how many tiles tall the map is
	Uint32	tile_width;			// This is how many tiles wide the map is
} pq_world;

/**
* @brief: This function tests a pixel quest world.
* @return: If there was any error in creating a new world entity, it returns NULL, otherwise a new pq_world pointer.
*/
pq_world* test_new_world();

/**
* @brief: This function creates a new pixel quest world.
* @param width: This is the width of the pixel quest world.
* @param height: This is the height of pixel quest world.
* @return: If there was any error in creating a new world entity, it returns NULL, otherwise a new pq_world pointer.
*/

pq_world* new_pq_world(Uint32 width, Uint32 height);

/**
* @brief: This function frees up the passed in pixel quest world.
* @param world: This is the pixel quest world to be freed.
*/
void free_pq_world(pq_world* world);

/**
* @brief: This function draws the pixel quest world.
* @param world: This is the pixel quest world to be drawn.
*/
void draw_pq_world(pq_world* world);