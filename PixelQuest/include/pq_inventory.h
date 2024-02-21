#pragma once

#include <pq_entity.h>
#include <pq_item.h>

#define MAX_INVENTORY_SIZE 10

typedef struct
{
    pq_entity* items[MAX_INVENTORY_SIZE];
    int count;
} pq_inventory;

/**
* @brief: Initialize the player's inventory.
* @return: A pointer to the created inventory.
*/
pq_inventory* init_pq_inventory();

/**
* @brief: Add an item to the player's inventory.
* @param inventory: The player's inventory.
* @param item: The item to add to the inventory.
* @return: 1 if successful, 0 if the inventory is full.
*/
int pq_inventory_add_item(pq_inventory* inventory, pq_entity* item);

/**
* @brief: Remove an item from the player's inventory.
* @param inventory: The player's inventory.
* @param item: The item to remove from the inventory.
* @return: 1 if successful, 0 if the item is not in the inventory.
*/
int pq_inventory_remove_item(pq_inventory* inventory, pq_entity* item);

/**
* @brief: Display the player's inventory.
* @param inventory: The player's inventory.
*/
void pq_inventory_display(pq_inventory* inventory);

/**
* @brief: Free the memory allocated for the player's inventory.
* @param inventory: The player's inventory.
*/
void pq_inventory_free(pq_inventory* inventory);
