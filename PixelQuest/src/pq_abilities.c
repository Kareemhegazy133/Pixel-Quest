#include <simple_json.h>
#include <simple_logger.h>
#include <gfc_shape.h>
#include <pq_abilities.h>

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

pq_entity* load_nth_pq_ability(int n)
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

	const char* name = sj_object_get_value_as_string(ability_def, "name");
	gfc_word_cpy(ability->name, name);

	int damage;
	sj_object_get_value_as_int(ability_def, "damage", &damage);
	ability->ability_damage = damage;

	float cooldown;
	sj_object_get_value_as_float(ability_def, "cooldown", &cooldown);
	ability->cooldown = cooldown;

	ability->duration = 0.f;
	ability->max_duration = 12.f;

	return ability;
}

void pq_ability_think(pq_entity* ability)
{
	if (!ability) return;

	Vector2D direction = { 0 };
	direction.x = 1;
	vector2d_normalize(&direction);
	vector2d_scale(ability->velocity, direction, 5);

	// Check for ability collisions
	pq_ability_handle_collision(ability, get_pq_world());

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
		// Set defaults back
		ability->duration = 0.f;
		ability->_is_active = 0;
	}

	vector2d_add(ability->position, ability->position, ability->velocity);
}

void pq_ability_handle_collision(pq_entity* ability)
{
	// Access the entities list

	if (!ability) return;

	// Calculate ability's bounding box after velocity is applied
	Rect abilityBox = {
		ability->position.x + ability->velocity.x,
		ability->position.y + ability->velocity.y,
		ability->width,
		ability->height
	};

	/*// Iterate through the world entities and check for collision with any of them
	for (int i = 0; i < MAX_ENTITIES; i++)
	{

		// Skip anything that is not the player or an enemy
		//if (!currentEntity || currentEntity->type != PLAYER_ENTITY || currentEntity->type != ENEMY_ENTITY) continue;

		// Calculate entity's bounding box
		Rect entityBox = {
			currentEntity->position.x,
			currentEntity->position.y,
			currentEntity->width,
			currentEntity->height
		};

		// Check for collision
		if (gfc_rect_overlap(abilityBox, entityBox))
		{
			vector2d_clear(ability->velocity);
			// Apply Damage
		}
	}*/
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
