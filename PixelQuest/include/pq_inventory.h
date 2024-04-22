#pragma once

#include <pq_entity.h>
#include <pq_item.h>

#define MAX_INVENTORY_SIZE 100

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
* @param item_display_name: The item's display name to remove from the inventory.
* @param amount: the amount of the item to remove.
* @return: 1 if successful, 0 if the item is not in the inventory.
*/
int pq_inventory_remove_item(pq_inventory* inventory, TextWord item_display_name, int amount);

/**
* @brief: Display the player's inventory.
* @param inventory: The player's inventory.
*/
void pq_inventory_display(pq_inventory* inventory);

/**
* @brief: Get the player's item's count in inventory.
* @param inventory: The player's inventory.
* @param display_name: The display name of the item to get the amount of from the inventory.
*/
int pq_inventory_get_item_amount(pq_inventory* inventory, const char* display_name);

/**
* @brief: Free the memory allocated for the player's inventory.
* @param inventory: The player's inventory.
*/
void pq_inventory_free(pq_inventory* inventory);
