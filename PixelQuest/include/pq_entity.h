#pragma once

#include <gfc_types.h>
#include <gf2d_sprite.h>

/**
* @purpose: This struct defines a pixel quest entity
*/

typedef struct PQ_Entity_S
{
	///////// <Commons> /////////

	Uint8		_is_active;										// A flag for keeping track of memory usage
	Sprite*		sprite;											// The entity's sprite (null if entity does not have one)
	float		frame;											// The current frame of animation for the sprite
	Uint16		width;											// The entity's collision box width
	Uint16		height;											// The entity's collision box height
	Vector2D	position;										// The entity's position
	Vector2D	velocity;										// The entity's velocity
	void (*think)(struct PQ_Entity_S* self);					// The function to call to make decisions
	void (*update)(struct PQ_Entity_S* self);					// The function to call to execute the think function's decisions
	void (*free)(struct PQ_Entity_S* self);						// The function to call to clean up any custom allocated data
	void* data;													// For ad hoc addition data for the entity

	///////// <Player and Enemies Only> /////////

	int health;													// The player's health
	int max_health;												// The player's max health
	int damage;													// The player's damage
	int defense;												// The player's defense

	void (*take_damage)(struct PQ_Entity_S* self);				// The function to call to handle taking damage

	///////// <Player Only> /////////

	void (*handle_input)(struct PQ_Entity_S* self);				// The function to call to handle any input

	///////// <Items Only> /////////

	TextWord	display_name;									// The display name of the item
	int			count;											// The amount of the item
} pq_entity;

/**
* @brief: This function initializes the pixel quest entity management system and queues up cleanup on exit.
* @param max_amount: This is the maximum number of entities that can exist at the same time.
*/
void init_pq_entity_system(Uint32 max_amount);

/**
* @brief: This function cleans up all the pixel quest entities except the passed in pixel quest entity.
* @param ignored_entity: This is the pixel quest entity to be ignored while cleaning up, pass in NULL if there is no entity to be ignored.
*/
void clear_all_pq_entities(pq_entity* ignored_entity);

/**
* @brief: This function creates a new pixel quest entity.
* @return: If our current total pq_entities did exceed the max_amount or there was any error in creating a new entity, it returns NULL, otherwise a new pq_entity pointer.
*/
pq_entity* new_pq_entity();

/**
* @brief: This function frees up the passed in pixel quest entity.
* @param entity: This is the pixel quest entity to be freed.
*/
void free_pq_entity(pq_entity* entity);

/**
* @brief: This function runs all the think functions for all the pixel quest entities.
*/
void pq_entity_system_think();

/**
* @brief: This function runs all the update functions for all the pixel quest entities.
*/
void pq_entity_system_update();

/**
* @brief: This function will draw all the pixel quest entities on the screen.
*/
void pq_entity_system_draw();