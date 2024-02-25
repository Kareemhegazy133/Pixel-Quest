#pragma once

#include <pq_entity.h>
#include <pq_world.h>
#include <pq_camera.h>
#include <pq_inventory.h>
#include <pq_ability.h>

typedef struct
{
	pq_inventory* inventory;
	pq_ability* abilities[MAX_ABILITIES];
} pq_player_data;

/**
* @brief: This function creates a new pixel quest player entity.
* @return: If there was any error in creating a new player entity, it returns NULL, otherwise a new pq_player entity pointer.
*/
pq_entity* new_pq_player();

/**
* @brief: This function handles input for the pixel quest player entity.
* @param player: This is the pixel quest player entity for which we handle input.
*/
void pq_player_handle_input(pq_entity* player);

/**
* @brief: This function handles taking damage for the pixel quest player entity.
* @param player: This is the pixel quest player entity that takes damage.
* @param damage: The damage that the player will take.
*/
void pq_player_take_damage(pq_entity* player, int damage);

/**
* @brief: This function makes decisions for the player entity which will then get executed in pq_player_update().
* @param player: This is the pixel quest player entity in which we will make decisions for.
*/
void pq_player_think(pq_entity* player);

/**
* @brief: This function executes the decisions for the player entity which was decided in pq_player_think().
* @param player: This is the pixel quest player entity in which we execute decisions for.
*/
void pq_player_update(pq_entity* player);

/**
* @brief: This function checks for collisions between the player entity and anything in the world entity.
* @param player: This is the pixel quest player entity.
* @param world: This is the pixel quest world entity.
*/
void pq_player_handle_collision(pq_entity* player, pq_world* world);

/**
* @brief: This function collects an item from the world and adds it to the player's inventory.
* @param player: This is the pixel quest player entity.
* @param world: This is the pixel quest world entity.
* @param itemIndex: This is the index of the pixel quest item entity to be collected.
*/
void pq_player_collect_item(pq_entity* player, pq_world* world, int itemIndex);

/**
* @brief: This function frees up the passed in pixel quest player entity.
* @param player: This is the pixel quest player entity to be freed.
*/
void pq_player_free(pq_entity* player);