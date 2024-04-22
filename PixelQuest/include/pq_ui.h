#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <gf2d_graphics.h>
#include <pq_entity.h>

typedef enum {
    MAIN_MENU,
    LOADING,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_QUIT
} GameState;

#define RED (SDL_Color){ 255, 0, 0 }
#define GREEN (SDL_Color){ 0, 255, 0 }
#define BLUE (SDL_Color){ 0, 0, 255 }

#define GAME_FONT "fonts/MoriaCitadel.ttf"

void init_pq_ui_system();

void pq_MainMenu(SDL_Renderer* renderer, GameState* gameState);

void pq_PauseMenu(SDL_Renderer* renderer, GameState* gameState);

void pq_ShopMenu(pq_entity* player);

void pq_InventoryMenu(pq_entity* player);

void pq_render_announcement(SDL_Color text_color, const char* msg, int seconds);

void pq_render_text(SDL_Color text_color, int font_size, int pos_x, int pos_y, const char* msg, int seconds);

void pq_render_player_health_bar(int player_health);