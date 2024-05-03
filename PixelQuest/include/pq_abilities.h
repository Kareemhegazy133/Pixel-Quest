#pragma once

#include <pq_entity.h>
#include <pq_world.h>

#define MAX_ABILITIES 10

typedef struct
{
    pq_entity* owner;
} pq_ability_data;

typedef struct
{
    pq_entity* abilities[MAX_ABILITIES];
    int count;
} pq_abilities_list;

/**
* @brief: Initialize the game's abilities.
* @return: A pointer to the game's abilities.
*/
pq_abilities_list* init_pq_abilities();

/**
* @brief: Load and initialize the nth ability from abilities def file.
* @param n: The nth index of the ability to be loaded.
* @param caster: The caster of the ability to be loaded.
* @return: A pointer to the created ability.
*/
pq_entity* load_nth_pq_ability(int n, pq_entity* caster);

/**
* @brief This function gets the pq_abilitiesList Sjson's pointer.
* @return A pointer to the pq_abilities_list Sjson.
*/
SJson* get_pq_abilities_list();

/**
 * @brief This function sets the pq_abilities_list Sjson's pointer.
 * @param world: The abilitiesList Sjson pointer to be referenced globally.
 */
void set_pq_abilities_list(SJson* abilitiesList);

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
void pq_ability_handle_collision(pq_entity* ability, pq_world* world, pq_entity* player);

/**
* @brief: This function applies the knockback effect to an entity.
* @param ability: This is the pixel quest ability.
* @param target: This is the pixel quest entity to be knockbacked.
*/
void pq_ability_knockback_effect(pq_entity* ability, pq_entity* target, int factor);

/**
* @brief: This function applies the slow effect to an entity.
* @param ability: This is the pixel quest ability.
* @param target: This is the pixel quest entity to be slowed.
*/
void pq_ability_slow_effect(pq_entity* ability, pq_entity* target, int factor);

/**
* @brief: This function applies the stun effect to an entity.
* @param ability: This is the pixel quest ability.
* @param target: This is the pixel quest entity to be stunned.
*/
void pq_ability_stun_effect(pq_entity* ability, pq_entity* target);

/**
* @brief: This function applies the effect of the ability.
* @param ability: This is the pixel quest ability.
*/
void pq_ability_apply_effects(pq_entity* ability);

/**
* @brief: This function is called when the pixel quest ability ends.
* @param ability: This is the pixel quest ability to end.
*/
void pq_ability_end(pq_entity* ability);

/**
* @brief: This function frees up the passed in pixel quest ability.
* @param ability: This is the pixel quest ability to be freed.
*/
void pq_ability_free(pq_entity* ability);

/**
* @brief: Free the memory allocated for the game's abilities list.
* @param abilities_list: The game's abilities list.
*/
void pq_abilities_free(pq_abilities_list* abilities_list);