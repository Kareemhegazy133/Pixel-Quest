#include <simple_logger.h>
#include <pq_item.h>
#include <pq_inventory.h>

void init_pq_inventory(pq_inventory* inventory)
{
	if (!inventory) return;

	inventory->items_list = gfc_list_new();
}

pq_entity* pq_inventory_get_item_by_name(pq_inventory* inventory, const char* name)
{
	if (!inventory) return NULL;

	return NULL;
}

void pq_inventory_cleanup(pq_inventory* inventory)
{
	if (!inventory) return;

	gfc_list_foreach(inventory->items_list, (gfc_work_func*)pq_item_free);
	gfc_list_delete(inventory->items_list);
	inventory->items_list = NULL;
}