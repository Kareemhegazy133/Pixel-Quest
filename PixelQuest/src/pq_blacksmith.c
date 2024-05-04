#include <simple_logger.h>

#include <pq_blacksmith.h>

pq_entity* init_pq_blacksmith(SJson* blacksmith_data)
{
	pq_entity* blacksmith;

	blacksmith = new_pq_entity();

	if (!blacksmith) {
		slog("blacksmith = NULL, Failed to create a pq_blacksmith entity.");
		return NULL;
	}

	blacksmith->type = SHOP_ENTITY;
	blacksmith->width = 128;
	blacksmith->height = 128;
	blacksmith->update = pq_blacksmith_update;
	blacksmith->free = pq_blacksmith_free;

	// Parse the rest of the properties from the json
	gfc_word_cpy(blacksmith->display_name, sj_object_get_value_as_string(blacksmith_data, "displayName"));

	blacksmith->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(blacksmith_data, "sprite"), 128, 128, 1, 0);

	int pos_x, pos_y;
	sj_object_get_value_as_int(blacksmith_data, "position_x", &pos_x);
	sj_object_get_value_as_int(blacksmith_data, "position_y", &pos_y);
	blacksmith->position = vector2d(pos_x, pos_y);

	return blacksmith;
}

void pq_blacksmith_update(pq_entity* blacksmith)
{
	if (!blacksmith) return;

}

void pq_blacksmith_free(pq_entity* blacksmith)
{
	if (!blacksmith) return;

	if (blacksmith->sprite)
	{
		gf2d_sprite_free(blacksmith->sprite);
	}

	free(blacksmith);
}
