#include <simple_logger.h>

#include <pq_player.h>

pq_entity* new_pq_player()
{
	pq_entity* player;

	player = new_pq_entity();

	if (!player) {
		slog("player = NULL, Failed to spawn a pq_player entity.");
		return NULL;
	}

	player->sprite = gf2d_sprite_load_all("images/entities/Player/Player_Idle.png", 128, 128, 9, 0);
	player->width = 90;
	player->height = 120;
	player->frame = 0;
	player->position = vector2d(640, 580);

	// Initialize player stats
	player->health = 100;
	player->max_health = 100;
	player->damage = 10;
	player->defense = 5;

	player->take_damage = pq_player_take_damage;

	pq_player_data* player_data = gfc_allocate_array(sizeof(pq_player_data), 1);
	if (player_data)
	{
		player->data = player_data;
		player_data->inventory = init_pq_inventory();
	}

	// Initialize the input handling function for the player
	player->handle_input = pq_player_handle_input;

	player->think = pq_player_think;
	player->update = pq_player_update;
	player->free = pq_player_free;

	return player;
}

void pq_player_handle_input(pq_entity* player)
{
	if (!player) {
		slog("player = NULL, Cannot handle player input without a pq_player entity.");
		return;
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	///////// <Movement> /////////

	Vector2D direction = { 0 };

	// Move left when 'A' or left arrow key is pressed
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
	{
		direction.x = -1;
	}
	// Move right when 'D' or right arrow key is pressed
	else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
	{
		direction.x = 1;
	}

	// Jump when 'W' or up arrow key or 'Space' key is pressed
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_SPACE])
	{
		// TODO : Jump Logic
		direction.y = -1;
	}

	// Move down when 'S' or down arrow key is pressed (testing only)
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
	{
		direction.y = 1;
	}

	vector2d_normalize(&direction);
	vector2d_scale(player->velocity, direction, 2);

	// 'I' key to display inventory
	if (keys[SDL_SCANCODE_I])
	{
		// Cast data to pq_player_data
		pq_player_data* player_data = (pq_player_data*)player->data;
		if (!player_data || !player_data->inventory)
		{
			slog("player_data or player_data->inventory = NULL, Failed to display player inventory.");
			return;
		}
		pq_inventory_display(player_data->inventory);
	}
}

void pq_player_take_damage(pq_entity* player, int damage)
{
	if (!player)
	{
		slog("player = NULL, Cannot take damage without a pq_player entity.");
		return;
	}

	// Calculate actual damage after considering player's defense
	int actual_damage = damage - player->defense;
	if (actual_damage < 0)
	{
		actual_damage = 0;
	}

	// Update player's health
	player->health -= actual_damage;

	// Check if player's health is below or equal to zero (indicating death)
	if (player->health <= 0)
	{
		// Player is defeated, handle game over or respawn logic
		// pq_player_die(player) or pq_player_respawn(player);
	}
}

void pq_player_think(pq_entity* player)
{
	if (!player) return;

	pq_player_handle_input(player);

	// Check for collisions
	pq_player_handle_collision(player, get_pq_world());

}

void pq_player_update(pq_entity* player)
{
	if (!player) return;

	player->frame += 0.125;
	if (player->frame >= 9) {
		player->frame = 0;
	}

	vector2d_add(player->position, player->position, player->velocity);
	//pq_camera_follow(player->position);
	pq_camera_center_on(player->position);
}

void pq_player_handle_collision(pq_entity* player, pq_world* world)
{
	if (!player || !world) return;

	// Calculate player's bounding box after velocity is applied
	Rect playerBox = {
		player->position.x + player->velocity.x,
		player->position.y + player->velocity.y,
		player->width,
		player->height
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
			if (gfc_rect_overlap(playerBox, tileBox))
			{
				// Handle collision, for example, stop player's movement
				//slog("Player Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", playerBox.x, playerBox.y, playerBox.w, playerBox.h);
				//slog("Tile Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f", tileBox.x, tileBox.y, tileBox.w, tileBox.h);
				vector2d_clear(player->velocity);
			}
		}
	}

	// Iterate through the world items and check for collision with any of the items
	for (int i = 0; i < world->items_count; i++)
	{
		// Skip collected items
		if (world->items[i]->collected == 1) continue;
		// Calculate item's bounding box
		Rect itemBox = {
			world->items[i]->position.x,
			world->items[i]->position.y,
			world->items[i]->width,
			world->items[i]->height
		};

		// Check for collision
		if (gfc_rect_overlap(playerBox, itemBox))
		{
			//slog("Player Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", playerBox.x, playerBox.y, playerBox.w, playerBox.h);
			//slog("Item Box: x=%.2f, y=%.2f, w=%.2f, h=%.2f", itemBox.x, itemBox.y, itemBox.w, itemBox.h);

			// Add the item to the player's inventory
			pq_player_collect_item(player, world, i);
		}
	}
}

void pq_player_collect_item(pq_entity* player, pq_world* world, int itemIndex)
{
	pq_player_data* player_data = (pq_player_data*)player->data;
	if (player_data && player_data->inventory)
	{
		pq_inventory_add_item(player_data->inventory, world->items[itemIndex]);
	}

	// Set item status to collected
	world->items[itemIndex]->collected = 1;
	world->items[itemIndex]->sprite = NULL;
	slog("Collected %s", world->items[itemIndex]->display_name);
	pq_inventory_display(player_data->inventory);
}

void pq_player_free(pq_entity* player)
{
	if (!player || !player->data) return;

	pq_player_data* player_data = (pq_player_data*)player->data;

	pq_inventory_free(player_data->inventory);
	free(player_data);
}