#include <simple_logger.h>

#include <pq_shop.h>

pq_entity* init_pq_shop(SJson* shop_data)
{
	pq_entity* shop;

	shop = new_pq_entity();

	if (!shop) {
		slog("shop = NULL, Failed to create a pq_shop entity.");
		return NULL;
	}

	shop->type = SHOP_ENTITY;
	shop->width = 128;
	shop->height = 128;
	shop->update = pq_shop_update;
	shop->free = pq_shop_free;

	// Parse the rest of the properties from the json
	gfc_word_cpy(shop->display_name, sj_object_get_value_as_string(shop_data, "displayName"));

	shop->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(shop_data, "sprite"), 128, 128, 1, 0);

	int pos_x, pos_y;
	sj_object_get_value_as_int(shop_data, "position_x", &pos_x);
	sj_object_get_value_as_int(shop_data, "position_y", &pos_y);
	shop->position = vector2d(pos_x, pos_y);

	return shop;
}

void pq_shop_update(pq_entity* shop)
{
	if (!shop) return;

}

void pq_shop_free(pq_entity* shop)
{
	if (!shop) return;

	if (shop->sprite)
	{
		gf2d_sprite_free(shop->sprite);
	}

	free(shop);
}
