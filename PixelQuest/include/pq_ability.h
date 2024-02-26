#pragma once

#include <gf2d_sprite.h>
#include <pq_entity.h>
#include <pq_world.h>

#define MAX_ABILITIES 5

typedef struct
{
	pq_entity* caster;			// The ability's caster
} pq_ability_data;

/**
* @brief: This function loads the pixel quest abilities from a def file.
* @param file_name: This is the file name of the pixel quest abilities def file.
* @return: If there was any error in loading the abilities from the def file, it returns NULL, otherwise a SJson pointer to the abilities object.
*/
SJson* load_pq_abilities(const char* file_name);

/**
*@brief This function gets the pq_abilitiesList Sjson's pointer.
* @return A pointer to the pq_abilitiesList Sjson.
*/
SJson* get_pq_abilitiesList();

/**
 * @brief This function sets the pq_abilitiesList Sjson's pointer.
 * @param world: The abilitiesList Sjson pointer to be referenced globally.
 */
void set_pq_abilitiesList(SJson* abilitiesList);

/**
* @brief: Initialize the player's abilities.
* @return: A pointer to the created inventory.
*/
pq_entity* init_pq_ability_fireball();

/**
* @brief: This function makes decisions for the ability which will then get executed in pq_ability_update().
* @param ability: This is the pixel quest ability in which we will make decisions for.
* @param caster: This is the pixel quest ability's caster.
*/
void pq_ability_think(pq_entity* ability);

/**
* @brief: This function executes the decisions for the ability which was decided in pq_ability_think().
* @param ability: This is the pixel quest ability in which we execute decisions for.
* @param caster: This is the pixel quest ability's caster.
*/
void pq_ability_update(pq_entity* ability);

/**
* @brief: This function checks for collisions between the ability and anything in the world entity.
* @param ability: This is the pixel quest ability.
* @param world: This is the pixel quest world entity.
*/
void pq_ability_handle_collision(pq_entity* ability, pq_world* world);

/**
* @brief: This function frees up the passed in pixel quest ability.
* @param ability: This is the pixel quest ability to be freed.
*/
void pq_ability_free(pq_entity* ability);