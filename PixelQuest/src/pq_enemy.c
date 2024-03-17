#include <simple_logger.h>

#include <pq_player.h>
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

	enemy->type = ENEMY_ENTITY;
	enemy->current_state = IDLE;

	int frame_width, frame_height, frames_per_line;
	sj_object_get_value_as_int(enemy_json_data, "frame_width", &frame_width);
	sj_object_get_value_as_int(enemy_json_data, "frame_height", &frame_height);
	sj_object_get_value_as_int(enemy_json_data, "frames_per_line", &frames_per_line);
	enemy->idle_sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(enemy_json_data, "sprite"),
		frame_width, frame_height, frames_per_line, 0);

	enemy->attack_sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(enemy_json_data, "attack_sprite"),
		frame_width, frame_height, frames_per_line, 0);

	enemy->sprite = enemy->idle_sprite;

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

	float patrol_distance;
	sj_object_get_value_as_float(enemy_json_data, "patrol_distance", &patrol_distance);
	enemy->patrol_distance = patrol_distance;
	vector2d_add(enemy->patrol_point1, enemy->position, vector2d(-enemy->patrol_distance, 0));
	vector2d_add(enemy->patrol_point2, enemy->position, vector2d(enemy->patrol_distance, 0));
	enemy->current_state = IDLE;

	enemy->direction = -1;

	for (int i = 0; i < 5; i++) {
		enemy->active_effects[i] = NOT_BUFFED;
	}
	enemy->current_buffs_count = 0;

	pq_enemy_data* enemy_data = gfc_allocate_array(sizeof(pq_enemy_data), 1);
	if (enemy_data)
	{
		enemy->data = enemy_data;

		// Load Items
		SJson* enemy_items = sj_object_get_value(enemy_json_data, "items");
		if (!enemy_items)
		{
			slog("%s is missing the items list object.", enemy_json_data);
			sj_free(enemy_items);
		}
		else {
			for (int i = 0; i < sj_array_get_count(enemy_items); i++)
			{
				SJson* item_data = sj_array_get_nth(enemy_items, i);
				if (!item_data) {
					slog("item_data is NULL, Failed to init item %d for enemy.", i);
					break;
				}

				pq_entity* item = new_pq_item(item_data);
				if (!item){
					slog("item is NULL, Failed to init item %d for enemy.", i);
					break;
				}

				enemy_data->items[i] = item;
				slog("Loaded enemy item: %s", enemy_data->items[i]->display_name);
			}
		}

		// Load Abilities
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
				enemy_data->abilities->abilities[i] = load_nth_pq_ability(i, enemy);
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

	enemy->die = pq_enemy_die;

	return enemy;
}
// FIX
void pq_enemy_handle_actions(pq_entity* enemy)
{
	if (!enemy) {
		slog("enemy = NULL, Cannot handle enemy actions without a pq_enemy entity.");
		return;
	}
	pq_entity* player = get_pq_player();
	if (!player) return;

	float distance = fabs(player->position.x - enemy->position.x);

	if (distance <= ENEMY_ATTACK_RANGE)
	{
		//slog("Enemy Attacking");
		enemy->current_state = ATTACKING;
	}
	else
	{
		//slog("Enemy Patrolling");
		enemy->current_state = PATROLLING;
	}
}

// FIX
void pq_enemy_patrol(pq_entity* enemy)
{
	float distance_to_point1 = fabs(enemy->position.x - enemy->patrol_point1.x);
	float distance_to_point2 = fabs(enemy->position.x - enemy->patrol_point2.x);
	slog("enemy->position.x: %f", enemy->position.x);
	slog("distance_to_point1: %f, distance_to_point2: %f", distance_to_point1, distance_to_point2);
	Vector2D direction = { 0 };
	if (distance_to_point1 <= distance_to_point2)
	{
		direction.x = -1;
	}
	else {
		direction.x = 1;
	}
	
	vector2d_normalize(&direction);
	enemy->direction = direction.x;
	//slog("direction.x: %f", direction.x);
	// Update the enemy's velocity to move towards the patrol point
	vector2d_scale(enemy->velocity, direction, enemy->movement_speed);
}
// FIX
/*void pq_enemy_attack(pq_entity* enemy)
{
	pq_enemy_data* enemy_data = (pq_enemy_data*)enemy->data;
	if (!enemy_data) {
		slog("enemy_data = NULL, Failed to trigger any enemy ability.");
		return;
	}

	if (!enemy_data->abilities)
	{
		slog("enemy_data->abilities = NULL, Failed to trigger any enemy ability.");
		return;
	}

	if (!enemy_data->abilities->abilities[0])
	{
		slog("enemy_data->abilities->abilities[0] aka fireball = NULL.");
		return;
	}

	if (enemy_data->abilities->abilities[0]->duration != 0)
	{
		slog("enemy_data->abilities->abilities[0] aka fireball is on cooldown.");
		return;
	}

	pq_entity* player = get_pq_player();
	if (!player) {
		slog("Player is NULL, cannot perform enemy attack.");
		return;
	}

	float enemy_pos_x = enemy->position.x;
	float player_pos_x = player->position.x;
	// Player is on the left of enemy
	if (player_pos_x < enemy_pos_x)
	{

		enemy_data->abilities->abilities[0]->position = vector2d(enemy->position.x - 100, enemy->position.y + 50);
	}
	else {
		enemy_data->abilities->abilities[0]->position = vector2d(enemy->position.x + 100, enemy->position.y + 50);
	}
	
	enemy_data->abilities->abilities[0]->_is_active = 1;
	enemy->current_state = IDLE;
}
*/

void pq_enemy_chase(pq_entity* enemy)
{
	if (!enemy) {
		slog("Enemy is NULL, cannot perform enemy attack.");
		return;
	}

	pq_entity* player = get_pq_player();
	if (!player) {
		slog("Player is NULL, cannot perform enemy attack.");
		return;
	}

	float enemy_pos_x = enemy->position.x;
	float player_pos_x = player->position.x;
	Vector2D direction = { 0 };
	// Player is on the left of enemy
	if (player_pos_x < enemy_pos_x)
	{
		direction.x = -1;
	}
	else {
		direction.x = 1;
	}
	enemy->direction = direction.x;
	vector2d_normalize(&direction);
	vector2d_scale(enemy->velocity, direction, enemy->movement_speed);
}

void pq_enemy_think(pq_entity* enemy)
{
	if (!enemy) return;

	//pq_enemy_handle_actions(enemy);
	pq_enemy_chase(enemy);

	if (enemy->current_state != GROUNDED) {
		enemy->velocity.y += GRAVITY;
	}

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

	static Uint32 playerCollisionCooldown = 0;
	static const Uint32 PLAYER_COLLISION_COOLDOWN_DURATION = 1000; // 1 second cooldown
	static Uint32 lastAttackTime = 0;
	static const Uint32 PLAYER_IDLE_COOLDOWN_DURATION = 5000; // 5 second cooldown for returning to idle state

	// Calculate enemy's bounding box after velocity is applied
	Rect enemyBox = {
		enemy->position.x + enemy->velocity.x,
		enemy->position.y + enemy->velocity.y,
		enemy->width,
		enemy->height
	};

	Uint8 isStillGrounded = 0;
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
				isStillGrounded = 1;
				enemy->current_state = GROUNDED;
				vector2d_clear(enemy->velocity);
			}
		}
	}

	if (isStillGrounded == 0) {
		enemy->current_state = IDLE;
	}

	// Get the current time
	Uint32 currentTime = SDL_GetTicks();

	// Check if the player collision cooldown has expired
	if (currentTime - playerCollisionCooldown >= PLAYER_COLLISION_COOLDOWN_DURATION) {
		pq_entity* player = get_pq_player();
		if (!player) return;

		Rect playerBox = {
			player->position.x + player->velocity.x,
			player->position.y + player->velocity.y,
			player->width,
			player->height
		};

		// Check for collision
		if (gfc_rect_overlap(enemyBox, playerBox))
		{
			enemy->current_state = ATTACKING;
			enemy->sprite = enemy->attack_sprite;
			slog("colliding with player");
			pq_entity_take_damage(player, enemy->damage);

			// Set the player collision cooldown start time
			playerCollisionCooldown = currentTime;

			// Update the last attack time
			lastAttackTime = currentTime;

			// Set player state to in combat
			player->current_state = IN_COMBAT;
		}
		else
		{
			// Player is not in range, switch back to idle sprite (TODO: FIX Cant Switch state to idle cuz grounded logic)
			enemy->sprite = enemy->idle_sprite;
		}
	}

	// Check if the player should return to idle state
	if (currentTime - lastAttackTime >= PLAYER_IDLE_COOLDOWN_DURATION) {
		pq_entity* player = get_pq_player();
		if (player && player->current_state == IN_COMBAT) {
			player->current_state = IDLE;
		}
	}
}

void pq_enemy_drop_items(pq_entity* enemy, pq_world* world)
{
	pq_enemy_data* enemy_data = (pq_enemy_data*)enemy->data;
	if (enemy_data && enemy_data->items)
	{
		// Iterate through the enemy's items and drop them in the game world
		for (int i = 0; i < MAX_ENEMY_ITEMS; i++)
		{
			pq_entity* item = enemy_data->items[i];
			if (item)
			{
				// Set the position of the dropped item
				item->position = enemy->position;
				// Add the item to the world items list
				world->items[world->items_count] = item;
				world->items_count++;
			}
		}
	}

	
}

void pq_enemy_die(pq_entity* enemy)
{
	if (!enemy) return;

	pq_world* world = get_pq_world();

	// Remove the enemy from the world's enemies list
	for (int i = 0; i < world->enemies_count; i++)
	{
		if (world->enemies[i] == enemy)
		{
			// Shift the remaining enemies in the list
			for (int j = i; j < world->enemies_count - 1; j++)
			{
				world->enemies[j] = world->enemies[j + 1];
			}
			// Decrement the enemies count
			world->enemies_count--;
			break;
		}
	}

	pq_enemy_drop_items(enemy, get_pq_world());

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
	gf2d_sprite_free(enemy->idle_sprite);
	gf2d_sprite_free(enemy->attack_sprite);

	free(enemy_data);
	free(enemy);
}