#pragma once

#include <gf2d_sprite.h>

#define MAX_ABILITIES 5

typedef struct {
    TextWord name;              // Ability Name
    int hotkey;                 // The hotkey for the ability
    int cooldown;               // Cooldown in frames
    int damage;                 // Damage caused by the ability
} pq_ability;

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
pq_ability* init_pq_ability_fireball();

/**
* @brief: This function frees up the passed in pixel quest ability.
* @param ability: This is the pixel quest ability to be freed.
*/
void pq_ability_free(pq_ability* ability);