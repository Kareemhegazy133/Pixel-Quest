#pragma once

#include <gfc_list.h>

typedef enum
{
	II_ShortSword,
	II_Shield,
	II_HealthPotion,
	II_MAX
}pq_item_indices;

typedef struct
{
	//int items_list[II_MAX];
	List* items_list;
	int weight;
	int weight_limit;

}pq_inventory;

void init_pq_inventory(pq_inventory* inventory);

void pq_inventory_cleanup(pq_inventory* inventory);

void pq_inventory_add_item(pq_inventory* inventory, const char* name);