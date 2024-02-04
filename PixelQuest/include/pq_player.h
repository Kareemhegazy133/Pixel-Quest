#pragma once

#include <pq_entity.h>

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
* @brief: This function frees up the passed in pixel quest player entity.
* @param player: This is the pixel quest player entity to be freed.
*/
void pq_player_free(pq_entity* player);