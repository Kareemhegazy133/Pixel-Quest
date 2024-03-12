#include <simple_logger.h>

#include <pq_enemy.h>

pq_entity* new_pq_enemy(SJson* enemy_json_data)
{
	pq_entity* enemy;

	enemy = new_pq_entity();

	if (!enemy) {
		slog("enemy = NULL, Failed to spawn a pq_enemy entity.");
		return NULL;
	}

	if (!enemy_json_data) {
		slog("enemy_json_data = NULL, Failed to spawn a pq_enemy entity.");
		return NULL;
	}

	gfc_word_cpy(enemy->name, sj_object_get_value_as_string(enemy_json_data, "name"));

	int frame_width, frame_height, frames_per_line;
	sj_object_get_value_as_int(enemy_json_data, "frame_width", &frame_width);
	sj_object_get_value_as_int(enemy_json_data, "frame_height", &frame_height);
	sj_object_get_value_as_int(enemy_json_data, "frames_per_line", &frames_per_line);
	enemy->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(enemy_json_data, "sprite"),
		frame_width, frame_height, frames_per_line, 0);

	int width, height;
	sj_object_get_value_as_int(enemy_json_data, "width", &width);
	enemy->width = width;
	sj_object_get_value_as_int(enemy_json_data, "height", &height);
	enemy->height = height;

	enemy->frame = 0;

	int pos_x, pos_y;
	sj_object_get_value_as_int(enemy_json_data, "position_x", &pos_x);
	sj_object_get_value_as_int(enemy_json_data, "position_y", &pos_y);
	enemy->position = vector2d(pos_x, pos_y);

	// Initialize enemy stats
	int health, max_health, damage, defense, movement_speed;
	sj_object_get_value_as_int(enemy_json_data, "health", &health);
	enemy->health = health;

	sj_object_get_value_as_int(enemy_json_data, "max_health", &max_health);
	enemy->max_health = max_health;

	sj_object_get_value_as_int(enemy_json_data, "damage", &damage);
	enemy->damage = damage;

	sj_object_get_value_as_int(enemy_json_data, "defense", &defense);
	enemy->defense = defense;

	sj_object_get_value_as_int(enemy_json_data, "movement_speed", &movement_speed);
	enemy->movement_speed = movement_speed;

	Vector2D direction = { 0 };
	vector2d_normalize(&direction);
	vector2d_scale(enemy->velocity, direction, enemy->movement_speed);

	enemy->take_damage = pq_enemy_take_damage;

	pq_enemy_data* enemy_data = gfc_allocate_array(sizeof(pq_enemy_data), 1);
	if (enemy_data)
	{
		enemy->data = enemy_data;
		/* Items initialize here
		enemy_data->inventory = init_pq_inventory();
		enemy_data->inventory->count = 0;
		*/
		enemy_data->abilities = init_pq_abilities();
		enemy_data->abilities->count = 0;

		SJson* enemy_abilities = sj_object_get_value(enemy_json_data, "abilities");
		if (!enemy_abilities)
		{
			slog("%s is missing the abilities list object.", enemy_json_data);
			sj_free(enemy_abilities);
		}
		else {
			for (int i = 0; i < sj_array_get_count(enemy_abilities); i++)
			{
				enemy_data->abilities->abilities[i] = load_nth_pq_ability(i);
				enemy_data->abilities->count++;
				slog("Loaded enemy ability: %s", enemy_data->abilities->abilities[i]->name);
			}
		}
	}

	// Initialize the action handling function for the enemy
	enemy->handle_actions = pq_enemy_handle_actions;

	enemy->think = pq_enemy_think;
	enemy->update = pq_enemy_update;
	enemy->free = pq_enemy_free;

	return enemy;
}

void pq_enemy_handle_actions(pq_entity* enemy)
{
	if (!enemy) {
		slog("enemy = NULL, Cannot handle enemy actions without a pq_enemy entity.");
		return;
	}

	
}

void pq_enemy_take_damage(pq_entity* enemy, int damage)
{
	if (!enemy)
	{
		slog("enemy = NULL, Cannot take damage without a pq_enemy entity.");
		return;
	}

	// Calculate actual damage after considering enemy's defense
	int actual_damage = damage - enemy->defense;
	if (actual_damage < 0)
	{
		actual_damage = 0;
	}

	// Update enemy's health
	enemy->health -= actual_damage;

	// Check if enemy's health is below or equal to zero (indicating death)
	if (enemy->health <= 0)
	{
		// Enemy is defeated, handle game over or respawn logic
		// pq_enemy_die(enemy) or pq_enemy_respawn(enemy);
	}
}

void pq_enemy_think(pq_entity* enemy)
{
	if (!enemy) return;

	pq_enemy_handle_actions(enemy);

	// Check for collisions
	pq_enemy_handle_collision(enemy, get_pq_world());
}

void pq_enemy_update(pq_entity* enemy)
{
	if (!enemy) return;

	enemy->frame += 0.125;
	if (enemy->frame >= 9) {
		enemy->frame = 0;
	}

	vector2d_add(enemy->position, enemy->position, enemy->velocity);
}

void pq_enemy_handle_collision(pq_entity* enemy, pq_world* world)
{
	if (!enemy || !world) return;

	// Calculate enemy's bounding box after velocity is applied
	Rect enemyBox = {
		enemy->position.x + enemy->velocity.x,
		enemy->position.y + enemy->velocity.y,
		enemy->width,
		enemy->height
	};

	// Iterate through the tiles and check for collision with the tiles
	for (int i = 0; i < world->tile_height; i++)
	{
		for (int j = 0; j < world->tile_width; j++)
		{
			Uint32 index = j + (i * world->tile_width);
			if (world->map[index] == 0) continue; // Skip empty tiles

			// Calculate tile's bounding box
			Rect tileBox = {
				j * world->tile_set->frame_w,
				i * world->tile_set->frame_h,
				world->tile_set->frame_w,
				world->tile_set->frame_h
			};

			// Check for collision
			if (gfc_rect_overlap(enemyBox, tileBox))
			{
				// Handle collision, for example, stop enemy's movement
				//slog("Enemy Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", enemyBox.x, enemyBox.y, enemyBox.w, enemyBox.h);
				//slog("Tile Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f", tileBox.x, tileBox.y, tileBox.w, tileBox.h);
				vector2d_clear(enemy->velocity);
			}
		}
	}

}

void pq_enemy_drop_items(pq_entity* enemy, pq_world* world)
{
	pq_enemy_data* enemy_data = (pq_enemy_data*)enemy->data;
	if (enemy_data && enemy_data->items)
	{
		
	}

	
}

void pq_enemy_free(pq_entity* enemy)
{
	if (!enemy || !enemy->data) return;

	pq_enemy_data* enemy_data = (pq_enemy_data*)enemy->data;

	// Free the memory of enemy items
	for (int i = 0; i < MAX_ENEMY_ITEMS; i++)
	{
		pq_item_free(enemy_data->items[i]);
	}

	pq_abilities_free(enemy_data->abilities);

	free(enemy_data);
	free(enemy);
}