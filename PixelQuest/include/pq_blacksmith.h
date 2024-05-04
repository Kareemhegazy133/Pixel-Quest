#pragma once

#include <simple_json.h>
#include <pq_entity.h>

/**
* @brief: This function initializes the pixel quest blacksmith entity.
* @return: If there was any error in creating the blacksmith entity, it returns NULL, otherwise a new pq_blacksmith entity pointer.
*/
pq_entity* init_pq_blacksmith(SJson* blacksmith_data);

/**
* @brief: This function executes updates for the blacksmith entity.
* @param item: This is the pixel quest blacksmith entity in which we execute updates for.
*/
void pq_blacksmith_update(pq_entity* blacksmith);

/**
* @brief: This function frees up the passed in pixel quest blacksmith entity.
* @param item: This is the pixel quest blacksmith to be freed.
*/
void pq_blacksmith_free(pq_entity* blacksmith);
