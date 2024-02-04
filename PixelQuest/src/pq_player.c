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

	player->sprite = gf2d_sprite_load_all("images/entities/Swordsman/Idle.png", 128, 128, 8, 0);
	player->frame = 0;
	player->position = vector2d(0, 400);

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

	// Jump when when 'W' or up arrow key or 'Space' key is pressed
	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_SPACE])
	{
		// TODO : Jump Logic
		direction.y = -1;
	}

	vector2d_normalize(&direction);
	vector2d_scale(player->velocity, direction, 2);
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
}

void pq_player_free(pq_entity* player)
{
	if (!player) return;
}