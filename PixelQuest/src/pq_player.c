#include <simple_logger.h>

#include <pq_player.h>

void pq_player_think(pq_entity* player);
void pq_player_update(pq_entity* player);
void pq_player_free(pq_entity* player);

pq_entity* new_pq_player()
{
	pq_entity* player;

	player = new_pq_entity();

	if (!player) {
		slog("player = NULL, Failed to spawn a pq_player entity.");
		return NULL;
	}

	player->sprite = gf2d_sprite_load_all("images/entities/Men Pack/Swordsman/Idle.png", 128, 128, 8, 0);
	player->frame = 0;
	player->position = vector2d(0, 0);

	player->think = pq_player_think;
	player->update = pq_player_update;
	player->free = pq_player_free;

	return player;
}

void pq_player_think(pq_entity* player)
{
	if (!player) return;

	Vector2D direction = { 0 };
	Sint32 mx = 0, my = 0;
	SDL_GetMouseState(&mx, &my);

	if (player->position.x < mx) direction.x = 1;
	if (player->position.y < my) direction.y = 1;
	if (player->position.x > mx) direction.x = -1;
	if (player->position.y > my) direction.y = -1;


	vector2d_normalize(&direction);
	vector2d_scale(player->velocity, direction, 2);
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