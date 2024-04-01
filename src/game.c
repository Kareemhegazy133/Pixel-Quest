#include <SDL.h>
#include <simple_logger.h>

#include <gf2d_graphics.h>
#include <gf2d_sprite.h>
#include <pq_ui.h>
#include <pq_camera.h>
#include <pq_entity.h>
#include <pq_player.h>
#include <pq_world.h>

static GameState game_state = MAIN_MENU;

GameState* get_game_state() {
    return &game_state;
}

int main(int argc, char * argv[])
{
    int mx,my;
    float mf = 0;
    Sprite *mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);;
    Color mouseColor = gfc_color8(255, 100, 255, 200);
    
    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Pixel Quest",
        1280,
        720,
        1280,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    init_pq_ui_system();

    pq_world* world = NULL;
    pq_entity* player = NULL;

    /*main game loop*/
    while(game_state != GAME_QUIT)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        const Uint8* keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf += 0.1;
        if (mf >= 16.0) mf = 0;

        switch (game_state) {
            case MAIN_MENU:
                pq_MainMenu(gf2d_graphics_get_renderer(), get_game_state());
                break;
            case LOADING:
                SDL_ShowCursor(SDL_DISABLE);

                init_pq_entity_system();
                load_pq_abilities_json("defs/pq_abilities.json");
                pq_camera_set_size(vector2d(1280, 720));
                world = load_pq_world("maps/pq_mp_test_world.json");
                player = new_pq_player();
                break;
            case GAME_RUNNING:
                pq_entity_system_think();
                pq_entity_system_update();

                gf2d_graphics_clear_screen();// clears drawing buffers
                // all drawing should happen betweem clear_screen and next_frame
                //backgrounds drawn first
                draw_pq_world(world);

                pq_entity_system_draw();

                //UI elements last
                pq_render_player_health_bar(player->health);
                gf2d_sprite_draw(
                    mouse,
                    vector2d(mx, my),
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    &mouseColor,
                    (int)mf);

                gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
                break;
            }

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                game_state = GAME_QUIT; // exit condition if the window is closed
            }
        }

        //slog("Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
    }

    if(player) free_pq_entity(player);

    free_pq_world(world);

    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
