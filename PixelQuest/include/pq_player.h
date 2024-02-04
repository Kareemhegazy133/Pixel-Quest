#pragma once

#include <pq_entity.h>


/**
* @brief: This function creates a new pixel quest player entity.
* @return: If there was any error in creating a new player entity, it returns NULL, otherwise a new pq_player entity pointer.
*/
pq_entity* new_pq_player();