#include <simple_json.h>
#include <simple_logger.h>
#include <gfc_shape.h>
#include <pq_ability.h>

static SJson* g_pq_abilitiesList = NULL;  // Global variable to store the abilities list sjson pointer

SJson* load_pq_abilities(const char* file_name)
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

	set_pq_abilitiesList(abilitiesList);
	return abilitiesList;
}

SJson* get_pq_abilitiesList()
{
	return g_pq_abilitiesList;
}

void set_pq_abilitiesList(SJson* abilitiesList)
{
	g_pq_abilitiesList = abilitiesList;
}

pq_entity* init_pq_ability_fireball()
{
	pq_entity* fireball = new_pq_entity();
	SJson* abilitiesList = get_pq_abilitiesList();
	if (!abilitiesList)
	{
		slog("abilitiesList = NULL, Cannot initialize fireball without the abilities list");
		return NULL;
	}

	SJson* fireball_def = sj_array_get_nth(abilitiesList, 0);
	if (!fireball_def)
	{
		slog("Fireball is missing from the abilities list object in pq_abilities def file.");
		return NULL;
	}
	fireball->_is_active = 0;
	fireball->sprite = gf2d_sprite_load_all(sj_object_get_value_as_string(fireball_def, "sprite"), 52, 40, 3, 0);
	fireball->frame = 0;
	fireball->width = 52;
	fireball->height = 40;

	fireball->think = pq_ability_think;
	fireball->update = pq_ability_update;
	fireball->free = pq_ability_free;

	const char* name = sj_object_get_value_as_string(fireball_def, "name");
	gfc_word_cpy(fireball->name, name);

	int damage;
	sj_object_get_value_as_int(fireball_def, "damage", &damage);
	fireball->ability_damage = damage;

	float cooldown;
	sj_object_get_value_as_float(fireball_def, "cooldown", &cooldown);
	fireball->cooldown = cooldown;

	fireball->duration = 0.f;
	fireball->max_duration = 12.f;

	pq_ability_data* fireball_data = gfc_allocate_array(sizeof(pq_ability_data), 1);
	if (fireball_data)
	{
		fireball->data = fireball_data;
		fireball_data->caster = NULL;
	}

	return fireball;
}

void pq_ability_think(pq_entity* ability)
{
	if (!ability) return;

	pq_ability_data* ability_data = (pq_ability_data*)ability->data;
	if (!ability_data)
	{
		slog("ability_data = NULL.");
		return;
	}

	Vector2D direction = { 0 };
	direction.x = 1;
	vector2d_normalize(&direction);
	vector2d_scale(ability->velocity, direction, 5);

	// Check for ability collisions
	//pq_ability_handle_collision(ability, get_pq_world());

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

void pq_ability_handle_collision(pq_entity* ability, pq_world* world)
{
	if (!ability || !world) return;

	// Calculate ability's bounding box after velocity is applied
	Rect abilityBox = {
		ability->position.x + ability->velocity.x,
		ability->position.y + ability->velocity.y,
		ability->width,
		ability->height
	};

}

void pq_ability_free(pq_entity* ability)
{
	if (!ability) return;

	gf2d_sprite_free(ability->sprite);

	pq_ability_data* ability_data = (pq_ability_data*)ability->data;
	free_pq_entity(ability_data->caster);
	free(ability_data);
	free(ability);
}
