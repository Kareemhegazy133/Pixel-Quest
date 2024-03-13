#pragma once

#include <gfc_shape.h>
#include <simple_json.h>
#include <pq_entity.h>
#include <pq_world.h>
#include <pq_inventory.h>
#include <pq_abilities.h>

#define MAX_ENEMY_ITEMS 1

typedef struct
{
	pq_entity* items[MAX_ENEMY_ITEMS];
	pq_abilities_list* abilities;
} pq_enemy_data;

/**
* @brief: This function creates a new pixel quest enemy entity.
* @return: If there was any error in creating a new enemy entity, it returns NULL, otherwise a new pq_enemy entity pointer.
*/
pq_entity* new_pq_enemy(SJson* enemy_data);

/**
* @brief: This function handles actions for the pixel quest enemy entity.
* @param enemy: This is the pixel quest enemy entity for which we handle actions.
*/
void pq_enemy_handle_actions(pq_entity* enemy);

/**
* @brief: This function makes decisions for the enemy entity which will then get executed in pq_enemy_update().
* @param enemy: This is the pixel quest enemy entity in which we will make decisions for.
*/
void pq_enemy_think(pq_entity* enemy);

/**
* @brief: This function executes the decisions for the enemy entity which was decided in pq_enemy_think().
* @param enemy: This is the pixel quest enemy entity in which we execute decisions for.
*/
void pq_enemy_update(pq_entity* enemy);

/**
* @brief: This function checks for collisions between the enemy entity and anything in the world entity.
* @param enemy: This is the pixel quest enemy entity.
* @param world: This is the pixel quest world entity.
*/
void pq_enemy_handle_collision(pq_entity* enemy, pq_world* world);

/**
* @brief: This function drops all items from the enemy.
* @param enemy: This is the pixel quest enemy entity.
* @param world: This is the pixel quest world entity.
*/
void pq_enemy_drop_items(pq_entity* enemy, pq_world* world);

/**
* @brief: This function kills for the pixel quest enemy.
* @param enemy: This is the pixel quest enemy that will die.
*/
void pq_enemy_die(pq_entity* enemy);

/**
* @brief: This function frees up the passed in pixel quest enemy entity.
* @param enemy: This is the pixel quest enemy entity to be freed.
*/
void pq_enemy_free(pq_entity* enemy);