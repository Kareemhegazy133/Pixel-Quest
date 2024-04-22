#include <simple_logger.h>

#include <pq_item.h>

static SJson* pq_items_list_json = NULL;  // Global variable to store the items list sjson pointer
static SJson* mace_json = NULL;
static SJson* wand_json = NULL;
static SJson* staff_json = NULL;
static SJson* coin_json = NULL;
static SJson* food_json = NULL;

SJson* load_pq_items_json(const char* file_name)
{
	if (!file_name)
	{
		slog("file_name = NULL, Cannot load items without a file name.");
		return NULL;
	}

	SJson* items_file_json = sj_load(file_name);
	if (!items_file_json)
	{
		slog("items_file_json = NULL, Failed to load items def file.");
		return NULL;
	}

	SJson* itemsList = sj_object_get_value(items_file_json, "items");
	if (!itemsList)
	{
		slog("%s is missing the items list object.", file_name);
		sj_free(items_file_json);
		return NULL;
	}

	pq_items_list_json = itemsList;

	slog("Loaded pq_items_json.");
	return itemsList;
}

SJson* get_pq_items_list()
{
	return pq_items_list_json;
}

pq_entity* new_pq_item(SJson* item_data)
{
	pq_entity* item;

	item = new_pq_entity();

	if (!item) {
		slog("item = NULL, Failed to create a pq_item entity.");
		return NULL;
	}

	int item_type;
	sj_object_get_value_as_int(item_data, "type", &item_type);

	if (item_type == 1)
	{
		item->type = WEAPON_ENTITY;
		item->width = 128;
		item->height = 32;

		item->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "sprite"), 128, 32, 1, 0);
		item->sprite_2 = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "sprite_2"), 128, 32, 1, 0);
		item->sprite_3 = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "sprite_3"), 128, 32, 1, 0);
		item->inventory_sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "inventory_sprite"), 128, 32, 1, 0);
		item->inventory_sprite_2 = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "inventory_sprite_2"), 128, 32, 1, 0);
		item->inventory_sprite_3 = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "inventory_sprite_3"), 128, 32, 1, 0);
	}
	else
	{
		item->type = ITEM_ENTITY;
		item->width = 64;
		item->height = 64;

		item->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "sprite"), 64, 64, 1, 0);
		item->inventory_sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "inventory_sprite"), 64, 64, 1, 0);
	}

	item->frame = 0;
	item->update = pq_item_update;
	item->free = pq_item_free;

	// Parse the rest of the properties from the json
	gfc_word_cpy(item->display_name, sj_object_get_value_as_string(item_data, "displayName"));

	sj_object_get_value_as_int(item_data, "cost", &item->cost);

	int pos_x, pos_y;
	sj_object_get_value_as_int(item_data, "position_x", &pos_x);
	sj_object_get_value_as_int(item_data, "position_y", &pos_y);
	item->position = vector2d(pos_x, pos_y);

	sj_object_get_value_as_int(item_data, "count", &item->count);

	item->collected = 0;

	return item;
}

void pq_item_update(pq_entity* item)
{
	if (!item) return;

	if (item->type == ITEM_ENTITY)
	{
		item->frame += 0.125;
		if (item->frame >= 6) {
			item->frame = 0;
		}
	}
}

void pq_item_free(pq_entity* item)
{
	if (!item) return;

	if (item->sprite)
	{
		gf2d_sprite_free(item->sprite);
	}

	gf2d_sprite_free(item->inventory_sprite);

	if (item->type == WEAPON_ENTITY)
	{
		gf2d_sprite_free(item->sprite_2);
		gf2d_sprite_free(item->sprite_3);
		gf2d_sprite_free(item->inventory_sprite_2);
		gf2d_sprite_free(item->inventory_sprite_3);
	}
	free(item);
}
