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
    if (item->type != WEAPON_ENTITY)
    {
        for (int i = 0; i < item->count; i++)
        {
            inventory->items[inventory->count++] = item;
        }
    }
    else {
        inventory->items[inventory->count++] = item;

        if (gfc_word_cmp(item->display_name, "Mace") == 0)
        {
            inventory->mace_count++;
        }
        else if (gfc_word_cmp(item->display_name, "Wand") == 0)
        {
            inventory->wand_count++;
        }
        else if (gfc_word_cmp(item->display_name, "Staff") == 0)
        {
            inventory->staff_count++;
        }
        else if (gfc_word_cmp(item->display_name, "Bat") == 0)
        {
            inventory->bat_count++;
        }
        else if (gfc_word_cmp(item->display_name, "Sickle") == 0)
        {
            inventory->sickle_count++;
        }
    }
    
    slog("Added %d %s to player's inventory", item->count, item->display_name);
    return 1;
}

int pq_inventory_remove_item(pq_inventory* inventory, TextWord item_display_name, int amount)
{
    if (!inventory || !item_display_name)
    {
        slog("inventory or item_display_name = NULL, failed to remove item to the inventory.");
        return 0;
    }

    int removed = 0;

    // Find the item in the inventory and remove it x amount of times
    for (int i = 0; i < inventory->count; i++)
    {
        if (gfc_word_cmp(inventory->items[i]->display_name, item_display_name) == 0)
        {
            if (gfc_word_cmp(inventory->items[i]->display_name, "Mace") == 0)
            {
                inventory->mace_count--;
            }
            else if (gfc_word_cmp(inventory->items[i]->display_name, "Wand") == 0)
            {
                inventory->wand_count--;
            }
            else if (gfc_word_cmp(inventory->items[i]->display_name, "Staff") == 0)
            {
                inventory->staff_count--;
            }
            else if (gfc_word_cmp(inventory->items[i]->display_name, "Bat") == 0)
            {
                inventory->bat_count--;
            }
            else if (gfc_word_cmp(inventory->items[i]->display_name, "Sickle") == 0)
            {
                inventory->sickle_count--;
            }

            // Shift items to fill the gap
            for (int j = i; j < inventory->count - 1; j++)
            {
                inventory->items[j] = inventory->items[j + 1];
            }

            // Clear the last slot and update the count
            inventory->items[inventory->count - 1] = NULL;
            inventory->count--;
            removed++;

            // Check if we have removed the desired amount
            if (removed == amount)
            {
                return 1; // Removed the desired amount
            }

            // After removing an item, we need to check the current index 'i' again
            // because inventory->items[i] has been replaced by the next item.
            i--;
        }
    }

    if (removed > 0)
    {
        slog("Removed %d %s from Inventory", removed, item_display_name);
        return 1;
    }

    slog("Item not found in Inventory");
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

    //slog("Amount of %s in inventory: %d", display_name, amount);
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
