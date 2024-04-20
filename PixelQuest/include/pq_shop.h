#pragma once

#include <simple_json.h>
#include <pq_entity.h>

/**
* @brief: This function initializes the pixel quest shop entity.
* @return: If there was any error in creating the shop entity, it returns NULL, otherwise a new pq_shop entity pointer.
*/
pq_entity* init_pq_shop(SJson* shop_data);

/**
* @brief: This function executes updates for the shop entity.
* @param item: This is the pixel quest shop entity in which we execute updates for.
*/
void pq_shop_update(pq_entity* shop);

/**
* @brief: This function frees up the passed in pixel quest shop entity.
* @param item: This is the pixel quest shop to be freed.
*/
void pq_shop_free(pq_entity* shop);
