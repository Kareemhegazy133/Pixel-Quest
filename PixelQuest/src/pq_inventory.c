#include <simple_logger.h>

#include <pq_inventory.h>

pq_inventory* init_pq_inventory()
{
    pq_inventory* inventory = gfc_allocate_array(sizeof(pq_inventory), 1);
    if (!inventory) {
        slog("inventory = NULL, Failed to create a pq_inventory.");
        return NULL;
    }
    memset(inventory->items, 0, sizeof(pq_entity*) * MAX_INVENTORY_SIZE);

    return inventory;
}

int pq_inventory_add_item(pq_inventory* inventory, pq_entity* item)
{
    if (!inventory || !item)
    {
        slog("inventory or item = NULL, failed to add item to the inventory.");
        return 0;
    }

    if (inventory->count >= MAX_INVENTORY_SIZE)
    {
        slog("Inventory is full. Cannot add more items.");
        return 0;
    }

    // Add the item to the inventory
    inventory->items[inventory->count++] = item;
    slog("Added %s to player's inventory", item->display_name);
    return 1;
}

int pq_inventory_remove_item(pq_inventory* inventory, pq_entity* item)
{
    if (!inventory || !item)
    {
        slog("inventory or item = NULL, failed to remove item to the inventory.");
        return 0;
    }

    // Find the item in the inventory
    for (int i = 0; i < inventory->count; i++)
    {
        if (inventory->items[i] == item)
        {
            // Shift items to fill the gap
            for (int j = i; j < inventory->count - 1; j++)
            {
                inventory->items[j] = inventory->items[j + 1];
            }

            // Clear the last slot and update the count
            inventory->items[inventory->count - 1] = NULL;
            inventory->count--;

            return 1; // Item removed successfully
        }
    }

    // Item not found in the inventory
    slog("Item not found in the inventory.");
    return 0;
}

void pq_inventory_display(pq_inventory* inventory)
{
    if (!inventory)
    {
        slog("inventory = NULL, Failed to display a pq_inventory.");
        return;
    }

    slog("Inventory:");

    if (inventory->count == 0)
    {
        slog("Empty");
        return;
    }

    for (int i = 0; i < inventory->count; i++)
    {
        slog("Item %d: %s", i + 1, inventory->items[i]->display_name);
    }
}

int pq_inventory_get_item_amount(pq_inventory* inventory, const char* display_name)
{
    if (!inventory || !display_name)
    {
        slog("inventory or display_name = NULL, Failed to get item amount in the inventory.");
        return 0;
    }

    int amount = 0;

    for (int i = 0; i < inventory->count; i++)
    {
        if (strcmp(inventory->items[i]->display_name, display_name) == 0)
        {
            amount++;
        }
    }

    slog("Amount of %s in inventory: %d", display_name, amount);
    return amount;
}

void pq_inventory_free(pq_inventory* inventory)
{
    if (!inventory)
    {
        slog("inventory = NULL, Failed to free pq_inventory.");
        return;
    }

    // Free the memory of individual items
    for (int i = 0; i < inventory->count; i++)
    {
        pq_item_free(inventory->items[i]);
    }

    free(inventory);
}
