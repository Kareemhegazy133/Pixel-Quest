#include <simple_logger.h>

#include <pq_item.h>

pq_entity* new_pq_item(SJson* item_data)
{
	pq_entity* item;

	item = new_pq_entity();

	if (!item) {
		slog("item = NULL, Failed to create a pq_item entity.");
		return NULL;
	}

	item->frame = 0;
	item->update = pq_item_update;
	item->free = pq_item_free;

	// Parse the rest of the properties from the json
	gfc_word_cpy(item->display_name, sj_object_get_value_as_string(item_data, "displayName"));

	item->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(item_data, "sprite"), 64, 64, 1, 0);

	int pos_x, pos_y;
	sj_object_get_value_as_int(item_data, "position_x", &pos_x);
	sj_object_get_value_as_int(item_data, "position_y", &pos_y);
	item->position = vector2d(pos_x, pos_y);

	sj_object_get_value_as_int(item_data, "count", &item->count);

	return item;
}

void pq_item_update(pq_entity* item)
{
	if (!item) return;

	item->frame += 0.125;
	if (item->frame >= 6) {
		item->frame = 0;
	}
}

void pq_item_free(pq_entity* item)
{
	if (!item) return;

	gf2d_sprite_free(item->sprite);
	free(item);
}
