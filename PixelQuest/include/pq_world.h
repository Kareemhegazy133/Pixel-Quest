#pragma once

#include <gf2d_graphics.h>
#include <gf2d_sprite.h>
#include <pq_item.h>

#define MAX_ITEMS 128
#define MAX_ENEMIES 20

typedef struct
{
	Sprite* background;						// This is the background image for the world
	Sprite* tile_layer;						// This is the sprite containing a prerendered layer of the tiles for the world
	Sprite* tile_set;						// This is the sprite containing tiles for the world
	Uint8*	map;							// This is the tiles that make up the world
	Uint32	tile_height;					// This is how many tiles tall the map is
	Uint32	tile_width;						// This is how many tiles wide the map is
	pq_entity* items[MAX_ITEMS];			// This is the list of items in the world
	Uint16	items_count;					// This is how many items are in the world
	pq_entity* enemies[MAX_ENEMIES];		// This is the list of enemies in the world
	Uint16	enemies_count;					// This is how many enemies are in the world
} pq_world;

/**
* @brief: This function tests a pixel quest world.
* @return: If there was any error in creating a new world entity, it returns NULL, otherwise a new pq_world pointer.
*/
pq_world* test_new_world();

/**
* @brief: This function loads a pixel quest world from a map file.
* @param file_name: This is the file name of the pixel quest world map.
* @return: If there was any error in loading the world from the map file, it returns NULL, otherwise a new pq_world pointer.
*/
pq_world* load_pq_world(const char* file_name);

/**
* @brief: This function creates a new pixel quest world.
* @param width: This is the width of the pixel quest world.
* @param height: This is the height of pixel quest world.
* @return: If there was any error in creating a new world entity, it returns NULL, otherwise a new pq_world pointer.
*/
pq_world* new_pq_world(Uint32 width, Uint32 height);

/**
 * @brief This function gets the pq_world pointer.
 * @return A pointer to the pq_world.
 */
pq_world* get_pq_world();

/**
 * @brief This function sets the pq_world pointer.
 * @param world: The world pointer to be referenced globally.
 */
void set_pq_world(pq_world* world);

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

/**
* @brief: This function sets up the camera's bounds to the pixel quest world size.
* @param world: This is the pixel quest world.
*/
void setup_camera_pq_world(pq_world* world);