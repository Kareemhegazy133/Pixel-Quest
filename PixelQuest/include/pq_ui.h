#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <gf2d_graphics.h>

#define RED (SDL_Color){ 255, 0, 0 }
#define GREEN (SDL_Color){ 0, 255, 0 }
#define BLUE (SDL_Color){ 0, 0, 255 }

void init_pq_ui_system();

void pq_render_announcement(SDL_Color text_color, const char* msg, int font_size, int pos_x, int pos_y, int seconds);