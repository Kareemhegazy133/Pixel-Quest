#include <simple_json.h>
#include <simple_logger.h>
#include <gfc_shape.h>
#include <pq_abilities.h>
#include <pq_player.h>

static SJson* pq_abilities_list_json = NULL;  // Global variable to store the abilities list sjson pointer

pq_abilities_list* init_pq_abilities()
{
    pq_abilities_list* abilities_list = gfc_allocate_array(sizeof(pq_abilities_list), 1);
    if (!abilities_list) {
        slog("abilities_list = NULL, Failed to create a pq_abilities_list.");
        return NULL;
    }
    memset(abilities_list->abilities, 0, sizeof(pq_entity*) * MAX_ABILITIES);

    return abilities_list;
}

SJson* load_pq_abilities_json(const char* file_name)
{
	if (!file_name)
	{
		slog("file_name = NULL, Cannot load abilities without a file name.");
		return NULL;
	}

	SJson* abilities_file_json = sj_load(file_name);
	if (!abilities_file_json)
	{
		slog("abilities_file_json = NULL, Failed to load abilities def file.");
		return NULL;
	}

	SJson* abilitiesList = sj_object_get_value(abilities_file_json, "abilities");
	if (!abilitiesList)
	{
		slog("%s is missing the abilities list object.", file_name);
		sj_free(abilities_file_json);
		return NULL;
	}

	set_pq_abilities_list(abilitiesList);
	return abilitiesList;
}

SJson* get_pq_abilities_list()
{
	return pq_abilities_list_json;
}

void set_pq_abilities_list(SJson* abilitiesList)
{
	pq_abilities_list_json = abilitiesList;
}

pq_entity* load_nth_pq_ability(int n, pq_entity* caster)
{
	pq_entity* ability = new_pq_entity();
	SJson* abilitiesList = get_pq_abilities_list();
	if (!abilitiesList)
	{
		slog("abilitiesList = NULL, Cannot initialize fireball without the abilities list");
		return NULL;
	}

	SJson* ability_def = sj_array_get_nth(abilitiesList, n);
	if (!ability_def)
	{
		slog("Ability is missing from the abilities list object in pq_abilities def file.");
		return NULL;
	}

	ability->type = ABILITY_ENTITY;
	ability->_is_active = 0;
	ability->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(ability_def, "sprite"), 52, 40, 3, 0);
	ability->frame = 0;
	ability->width = 52;
	ability->height = 40;

	ability->think = pq_ability_think;
	ability->update = pq_ability_update;
	ability->free = pq_ability_free;

	pq_ability_data* ability_data = gfc_allocate_array(sizeof(pq_ability_data), 1);
	if (ability_data)
	{
		ability->data = ability_data;
		ability_data->owner = caster;
	}

	const char* name = sj_object_get_value_as_string(ability_def, "name");
	gfc_word_cpy(ability->name, name);

	int damage;
	sj_object_get_value_as_int(ability_def, "damage", &damage);
	ability->ability_damage = damage;

	float cooldown;
	sj_object_get_value_as_float(ability_def, "cooldown", &cooldown);
	ability->cooldown = cooldown;

	int effect;
	sj_object_get_value_as_int(ability_def, "effect", &effect);
	ability->ability_effect = effect;

	int type;
	sj_object_get_value_as_int(ability_def, "type", &type);
	ability->ability_type = type;

	int movement_speed;
	sj_object_get_value_as_int(ability_def, "movement_speed", &movement_speed);
	ability->movement_speed = movement_speed;

	ability->duration = 0.f;
	ability->max_duration = 12.f;
	ability->direction = 0;

	return ability;
}

void pq_ability_think(pq_entity* ability)
{
	if (!ability) return;

	pq_ability_data* ability_data = (pq_ability_data*)ability->data;
	if (!ability_data) return;

	Vector2D direction = { 0 };
	//slog("Owner direction: %d", ability_data->owner->direction);
	if (ability->ability_type == BUFF || ability->ability_type == DEBUFF)
	{
		vector2d_copy(ability->velocity, ability_data->owner->velocity);
	}
	else {
		// Take the owner's direction for the initial direction and keep it
		if (ability->direction == 0)
		{
			direction.x = ability_data->owner->direction;
			ability->direction = direction.x;
		}
		else {
			direction.x = ability->direction;
		}
		vector2d_normalize(&direction);
		vector2d_scale(ability->velocity, direction, ability->movement_speed);
	}

	// if its a defense buff, give the owner that buff
	if (ability->ability_effect == DEFENSE_BUFF && ability_data->owner->current_buffs_count < ability_data->owner->max_buffs_allowed) {
		ability_data->owner->defense += 10;
		ability_data->owner->current_buffs_count++;
	}
	// Check for ability collisions
	pq_ability_handle_collision(ability, get_pq_world(), get_pq_player());

}

void pq_ability_update(pq_entity* ability)
{
	if (!ability) return;

	ability->frame += 0.125;
	if (ability->frame >= 6) {
		ability->frame = 0;
	}

	ability->duration += 0.1;
	if (ability->duration >= ability->max_duration)
	{
		pq_ability_end(ability);
	}

	vector2d_add(ability->position, ability->position, ability->velocity);
}

void pq_ability_handle_collision(pq_entity* ability, pq_world* world, pq_entity* player)
{
	if (!ability || !world || !player) return;

	// Calculate ability's bounding box after velocity is applied
	Rect abilityBox = {
		ability->position.x + ability->velocity.x,
		ability->position.y + ability->velocity.y,
		ability->width,
		ability->height
	};

	pq_ability_data* ability_data = (pq_ability_data*)ability->data;
	if (!ability_data) return;

	pq_entity* caster = ability_data->owner;
	if (caster == player)
	{
		// Iterate through the world enemies and check for collision with any of them
		for (int i = 0; i < world->enemies_count; i++)
		{
			// Calculate enemy's bounding box
			Rect enemyBox = {
				world->enemies[i]->position.x,
				world->enemies[i]->position.y,
				world->enemies[i]->width,
				world->enemies[i]->height
			};

			// Check for collision
			if (gfc_rect_overlap(abilityBox, enemyBox))
			{
				int extra_damage = 0;
				if (ability->ability_effect == DAMAGE_BUFF) {
					extra_damage = 10;
				}
				pq_entity_take_damage(world->enemies[i], ability->ability_damage + extra_damage);
				pq_ability_end(ability);
			}
		}
	}
	else {
		// Calculate player's bounding box
		Rect playerBox = {
			player->position.x,
			player->position.y,
			player->width,
			player->height
		};

		// Check for collision
		if (gfc_rect_overlap(abilityBox, playerBox))
		{
			pq_entity_take_damage(player, ability->ability_damage);
			pq_ability_end(ability);
		}
	}
	
}

void pq_ability_end(pq_entity* ability)
{
	if (!ability) return;

	// Set defaults back
	vector2d_clear(ability->velocity);
	ability->duration = 0.f;
	ability->direction = 0;
	ability->_is_active = 0;
}

void pq_ability_free(pq_entity* ability)
{
    if (!ability) return;
	
	if (ability->sprite)
	{
		gf2d_sprite_free(ability->sprite);
	}

    free(ability);
}

void pq_abilities_free(pq_abilities_list* abilities_list)
{
    if (!abilities_list)
    {
        slog("abilities_list = NULL, Failed to free pq_abilities_list.");
        return;
    }

    // Free the memory of individual items
    for (int i = 0; i < abilities_list->count; i++)
    {
        pq_ability_free(abilities_list->abilities[i]);
    }

    free(abilities_list);
}
