#include <simple_json.h>
#include <simple_logger.h>
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

pq_ability* init_pq_ability_fireball()
{
	pq_ability* fireball = gfc_allocate_array(sizeof(pq_ability), 1);
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

	const char* name = sj_object_get_value_as_string(fireball_def, "name");
	gfc_word_cpy(fireball->name, name);
	int cooldown, damage;
	sj_object_get_value_as_int(fireball_def, "cooldown", &cooldown);
	sj_object_get_value_as_int(fireball_def, "damage", &damage);
	fireball->cooldown = cooldown;
	fireball->damage = damage;

	return fireball;
}

void pq_ability_free(pq_ability* ability)
{
	if (!ability) return;
	free(ability);
}
