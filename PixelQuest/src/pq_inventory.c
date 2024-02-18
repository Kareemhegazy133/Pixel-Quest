#include <simple_logger.h>
#include <pq_item.h>
#include <pq_inventory.h>

void init_pq_inventory(pq_inventory* inventory)
{
	if (!inventory) return;

	inventory->items_list = gfc_list_new();
}

pq_item* pq_inventory_get_item_by_name(pq_inventory* inventory, const char* name)
{
	if (!inventory) return;

	for(int i = 0; i < gfc_list_get_count; i++)
	{
		pq_item* item = gfc_list_get_nth(inventory->items_list, i);
		if (!item) continue;
		if (gfc_strlcmp(item->name, name) == 0)
		{
			return item;
		}
	}
	return NULL;
}

void pq_inventory_cleanup(pq_inventory* inventory)
{
	if (!inventory) return;

	gfc_list_foreach(inventory->items_list, (gfc_work_func*)free_pq_item);
	gfc_list_delete(inventory->items_list);
	inventory->items_list = NULL;
}

void pq_inventory_add_item(pq_inventory* inventory, const char* name)
{
	if (!inventory || !name) return;

	pq_item* item = pq_inventory_get_item_by_name(inventory, name);
	if (item)
	{
		item->count++;
		return;
	}

	item = new_item(name);
	if (!item) return;

	gfc_list_append(inventory->items_list, item);
}
