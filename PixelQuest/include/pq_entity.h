#pragma once

#include <gfc_types.h>
#include <gf2d_sprite.h>

#define MAX_ENTITIES 1024

#define GRAVITY 0.25f

typedef enum
{
	IDLE,
	GROUNDED
} EntityState;

typedef enum
{
	PLAYER_ENTITY,
	ENEMY_ENTITY,
	ITEM_ENTITY,
	ABILITY_ENTITY
} EntityType;

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
	EntityType  type;											// The type of this entity
	EntityState current_state;									// The current state of this entity

	///////// <Player and Enemies Only> /////////

	int health;													// The character's health
	int max_health;												// The character's max health
	int damage;													// The character's damage
	int defense;												// The character's defense
	int movement_speed;											// The character's movement speed
	int jump_force;												// The character's jump force

	void (*die)(struct PQ_Entity_S* self);						// The function to call when entity dies

	///////// <Player Only> /////////

	void (*handle_input)(struct PQ_Entity_S* self);				// The function to call to handle any input

	///////// <Enemies Only> /////////
	void (*handle_actions)(struct PQ_Entity_S* self);			// The function to call to handle enemy's actions

	///////// <Items Only> /////////

	TextWord	display_name;									// The display name of the item
	Sprite*		inventory_sprite;								// The item's inventory sprite (null if entity does not have one)
	int			count;											// The amount of the item
	Uint8		collected;										// A flag for keeping track if item is collected (1 for true, 0 for false)

	///////// <Abilities Only> /////////

	TextWord name;												// Ability Name
	int hotkey;													// The hotkey for the ability
	float cooldown;												// The ability's default cooldown
	int ability_damage;											// The ability's default damage
	float duration;												// The ability's duration
	float max_duration;											// The ability's max_duration
} pq_entity;

/**
* @brief: This function initializes the pixel quest entity management system and queues up cleanup on exit.
* @param max_amount: This is the maximum number of entities that can exist at the same time.
*/
void init_pq_entity_system();

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

/**
* @brief: This function handles taking damage for the pixel quest entity.
* @param entity: This is the pixel quest entity that takes damage.
* @param damage: The damage that the entity will take.
*/
void pq_entity_take_damage(pq_entity* entity, int damage);

/**
* @brief: This function kills for the pixel quest entity.
* @param entity: This is the pixel quest entity that will die.
*/
void pq_entity_die(pq_entity* entity);