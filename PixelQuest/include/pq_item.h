#pragma once

#include <simple_json.h>

#include <gfc_text.h>
#include <gf2d_sprite.h>

typedef struct
{
	TextLine name;
	Sprite* sprite;
	int count;			// The amount of this item
} pq_item;

/**
* @brief: This function initializes and loads the items' definitions.
* @param fileName: The json file name that contains the items data.
*/
void pq_init_items(const char* fileName);

/**
* @brief: This function gets an item definition file by its name.
* @param name: The name of the item to retrieve.
* @return NULL if not found, otherwise the item's definition
*/
SJson* get_pq_item_def_by_name(const char* name);

pq_item* new_item(const char* name);

void free_pq_item(pq_item* item);