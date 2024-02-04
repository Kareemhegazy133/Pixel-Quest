#include <simple_logger.h>

#include <pq_entity.h>

typedef struct
{
	pq_entity* entities_list;
	Uint32 max_entities;
} pq_entity_manager;

void close_pq_entity_system();

static pq_entity_manager _pq_entity_manager = { 0 }; // Initialize a LOCAL global pixel quest entity manager.

void init_pq_entity_system(Uint32 max_amount)
{
	if (_pq_entity_manager.entities_list)
	{
		slog("Cannot have two instances of pq_entity_manager.");
		return;
	}
	if (max_amount == 0)
	{
		slog("max_amount = 0, Cannot allocate 0 pixel quest entities.");
		return;
	}

	_pq_entity_manager.entities_list = gfc_allocate_array(sizeof(pq_entity), max_amount);
	if (!_pq_entity_manager.entities_list)
	{
		slog("_pq_entity_manager.entities_list = NULL, Failed to allocate global entities list.");
		return;
	}
	_pq_entity_manager.max_entities = max_amount;
	atexit(close_pq_entity_system);

}

void close_pq_entity_system()
{
	clear_all_pq_entities(NULL);
	if (_pq_entity_manager.entities_list)
	{
		free(_pq_entity_manager.entities_list);
	}
	memset(&_pq_entity_manager, 0, sizeof(pq_entity_manager));
}

void clear_all_pq_entities(pq_entity* ignored_entity)
{
	int i;
	for (i = 0; i < _pq_entity_manager.max_entities; i++)
	{
		if (&_pq_entity_manager.entities_list[i] == ignored_entity) continue;
		if (!_pq_entity_manager.entities_list[i]._is_active) continue;

		free_pq_entity(&_pq_entity_manager.entities_list[i]);
	}
}

pq_entity* new_pq_entity()
{
	int i;
	for (i = 0; i < _pq_entity_manager.max_entities; i++)
	{
		if (_pq_entity_manager.entities_list[i]._is_active) continue;

		memset(&_pq_entity_manager.entities_list[i], 0, sizeof(pq_entity));
		
		// Set the defaults
		_pq_entity_manager.entities_list[i]._is_active = 1;
		_pq_entity_manager.entities_list[i].handle_input = NULL;

		return &_pq_entity_manager.entities_list[i];
	}
	slog("There are no more available pixel quest entities");
	return NULL;
}

void free_pq_entity(pq_entity* entity)
{
	if (!entity) return;

	gf2d_sprite_free(entity->sprite);

	if (entity->free)
	{
		entity->free(entity->data);
	}
}

void pq_entity_think(pq_entity* entity)
{
	if (!entity) return;

	if (entity->think) {
		entity->think(entity);
	}
}

void pq_entity_system_think()
{
	int i;
	for (i = 0; i < _pq_entity_manager.max_entities; i++)
	{
		if (!_pq_entity_manager.entities_list[i]._is_active) continue;
		pq_entity_think(&_pq_entity_manager.entities_list[i]);
	}
}

void pq_entity_update(pq_entity* entity)
{
	if (!entity) return;

	if (entity->update) {
		entity->update(entity);
	}
}

void pq_entity_system_update()
{
	int i;
	for (i = 0; i < _pq_entity_manager.max_entities; i++)
	{
		if (!_pq_entity_manager.entities_list[i]._is_active) continue;
		pq_entity_update(&_pq_entity_manager.entities_list[i]);
	}
	
}

void pq_entity_draw(pq_entity* entity)
{
	if (!entity) return;

	if (entity->sprite) {
		gf2d_sprite_render(entity->sprite, entity->position, NULL, NULL, NULL, NULL, NULL, NULL, (Uint32)entity->frame);
	}
}

void pq_entity_system_draw()
{
	int i;
	for (i = 0; i < _pq_entity_manager.max_entities; i++)
	{
		if (!_pq_entity_manager.entities_list[i]._is_active) continue;
		pq_entity_draw(&_pq_entity_manager.entities_list[i]);
	}
}
