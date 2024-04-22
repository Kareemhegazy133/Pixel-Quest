#pragma once

#include <simple_json.h>
#include <pq_entity.h>

/**
* @brief: This function creates a new pixel quest item entity.
* @return: If there was any error in creating a new item entity, it returns NULL, otherwise a new pq_item entity pointer.
*/
pq_entity* new_pq_item(SJson* item_data);

/**
* @brief This function gets the pq_itemsList Sjson's pointer.
* @return A pointer to the pq_items_list Sjson.
*/
SJson* get_pq_items_list();

/**
* @brief: This function executes updates for the item entity.
* @param item: This is the pixel quest item entity in which we execute updates for.
*/
void pq_item_update(pq_entity* item);

/**
* @brief: This function frees up the passed in pixel quest item entity.
* @param item: This is the pixel quest player item to be freed.
*/
void pq_item_free(pq_entity* item);