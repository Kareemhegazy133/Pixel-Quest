#pragma once

#include <gfc_vector.h>
#include <gfc_shape.h>

typedef struct
{
	Vector2D position;		// The camera's position in world space
	Vector2D size;			// The camera's width and height for the screen
	Rect bounds;			// The camera's bounds
	Bool bind_camera;		// If this flag is true, the camera will stay in bounds
} pq_camera;

/**
* @brief: This function sets the pixel quest camera's position in world space.
* @param position: The vector containing the new camera's position.
*/
void pq_camera_set_position(Vector2D position);

/**
* @brief: This function gets the pixel quest camera's position in world space.
* @return: The vector containing the camera's position.
*/
Vector2D pq_camera_get_position();

/**
* @brief: This function sets the pixel quest camera's size.
* @param size: The vector containing the new camera's size.
*/
void pq_camera_set_size(Vector2D size);

/**
* @brief: This function gets the pixel quest camera's offset which can be used to draw things relative to the camera.
* @return: The vector containing the camera's offset.
*/
Vector2D pq_camera_get_offset();

/**
* @brief: This function snaps the pixel quest camera's into world bounds.
*/
void pq_camera_set_bounds(Rect bounds);

/**
* @brief: This function enables binding the pixel quest camera.
* @param bind_camera: The bool that will determine whether the camera binding should be enabled.
*/
void pq_camera_enable_binding(Bool bind_camera);

/**
* @brief: This function keeps the pixel quest camera's into world bounds.
*/
void pq_camera_apply_bounds();

/**
* @brief: This function centers the pixel quest camera's on to target.
* @param target: The target in which the camera will be centered on.
*/
void pq_camera_center_on(Vector2D target);