#include <simple_logger.h>

#include <pq_item.h>

static SJson* _itemJson = NULL;
static SJson* _itemDefs = NULL;

void close_items();

void pq_init_items(const char* fileName)
{
	if (!fileName)
	{
		slog("No file name was provided for initializing items.");
		return;
	}
	
	_itemJson = sj_load(fileName);
	if (_itemJson)
	{
		slog("Failed to load the json for the item definition.");
		return;
	}

	_itemDefs = sj_object_get_value(_itemJson, "items");
	if (!_itemDefs)
	{
		slog("Item definition json file %s does not contain items list", fileName);
		sj_free(_itemJson);
		_itemJson = NULL;
		return;
	}

	atexit(close_items);
}

void close_items()
{
	if (_itemJson)
	{
		sj_free(_itemJson);
	}
	_itemJson = NULL;
	_itemDefs = NULL;
}

SJson* get_pq_item_def_by_name(const char* name)
{
	if (!name)
	{
		slog("Cannot find an item without providing a name.", name);
		return NULL;
	}

	if (!_itemDefs)
	{
		slog("No Item definitions loaded.");
		return NULL;
	}

	SJson* item;
	const char* item_name = NULL;
	for (int i = 0; i < sj_array_get_count(_itemDefs); i++)
	{
		item = sj_array_get_nth(_itemDefs, i);
		if (!item) continue;
		item_name = sj_object_get_value_as_string(item, "name");
		if (!item_name) continue;
		if (gfc_strlcmp(name, item_name) == 0)
		{
			return item;
		}

	}
	slog("No item found with the name: %s", name);
	return NULL;
}

pq_item* new_item(const char* name)
{
	SJson* itemDef = get_pq_item_def_by_name(name);
	if (!itemDef) return NULL;

	pq_item* item = gfc_allocate_array(sizeof(pq_item), 1);
	if (!item) return NULL;

	gfc_line_cpy(item->name, name);

	// Set Item's Properties
	//sj_object_get_value_as_int(itemDef, "");
	item->count = 1;

	return item;
}

void free_pq_item(pq_item* item)
{
	if (!item) return;

	gf2d_sprite_free(item->sprite);
	free(item);
}
