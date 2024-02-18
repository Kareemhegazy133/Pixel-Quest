#include <simple_logger.h>

#include <pq_camera.h>
#include <pq_player.h>
#include <pq_inventory.h>

typedef struct {
	int level;
	int experience;
	TextLine class_name;
	pq_inventory inventory;
}pq_player_data;

pq_entity* new_pq_player()
{
	pq_entity* player;

	player = new_pq_entity();

	if (!player) {
		slog("player = NULL, Failed to spawn a pq_player entity.");
		return NULL;
	}

	player->sprite = gf2d_sprite_load_all("images/entities/Swordsman/Idle.png", 128, 128, 8, 0);
	player->frame = 0;
	player->position = vector2d(0, 400);

	// Initialize player stats
	player->health = 100;
	player->max_health = 100;
	player->damage = 10;
	player->defense = 5;

	player->take_damage = pq_player_take_damage;

	// Initialize the input handling function for the player
	player->handle_input = pq_player_handle_input;

	player->think = pq_player_think;
	player->update = pq_player_update;
	player->free = pq_player_free;

	pq_player_data* data = gfc_allocate_array(sizeof(pq_player_data), 1);
	if (data)
	{
		player->data = data;
		init_pq_inventory(&data->inventory);
	}

	return player;
}

void pq_player_handle_input(pq_entity* player)
{
	if (!player) {
		slog("player = NULL, Cannot handle player input without a pq_player entity.");
		return;
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

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

}

void pq_player_update(pq_entity* player)
{
	if (!player) return;

	player->frame += 0.1;
	if (player->frame >= 8) {
		player->frame = 0;
	}

	vector2d_add(player->position, player->position, player->velocity);
	pq_camera_center_on(player->position);
}

void pq_player_free(pq_entity* player)
{
	if (!player || !player->data) return;

	pq_player_data* data = (pq_player_data*)player->data;
	pq_inventory_cleanup(&data->inventory);
	free(data);
}