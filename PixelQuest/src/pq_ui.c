#include <simple_logger.h>
#include <gf2d_sprite.h>
#include <gfc_shape.h>
#include <pq_ui.h>

void init_pq_ui_system()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        slog("SDL_Init Error: %s", SDL_GetError());
        return;
    }

    if (TTF_Init() != 0) {
        slog("TTF_Init Error: %s", TTF_GetError());
        SDL_Quit();
        return;
    }
}

void pq_MainMenu(SDL_Renderer* renderer, GameState* gameState)
{
    Sprite* background = gf2d_sprite_load_image("images/backgrounds/Main_Menu_Background.png");
    Bool menuOpen = true;

    // Load button sprites
    Sprite* startButton = gf2d_sprite_load_image("images/ui/Start_Game_Button.png");
    Sprite* quitButton = gf2d_sprite_load_image("images/ui/Quit_Game_Button.png");

    // Button positions
    Vector2D startButtonPos = vector2d(512, 200);
    Vector2D quitButtonPos = vector2d(512, 400);

    while (menuOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Draw the buttons
        gf2d_sprite_draw_image(startButton, startButtonPos);
        gf2d_sprite_draw_image(quitButton, quitButtonPos);

        // Present the rendered frame
        gf2d_graphics_next_frame();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                Vector2D mouse_pos = { mouseX, mouseY };
                Rect StartButtonBox =  { 512, 200, 256, 128 };
                Rect QuitButtonBox = { 512, 400, 256, 128 };

                // Check if the mouse click is within the bounds of the start button
                if (gfc_point_in_rect(mouse_pos, StartButtonBox))
                {
                    *gameState = LOADING;
                    menuOpen = false;
                }
                // Check if the mouse click is within the bounds of the quit button
                else if (gfc_point_in_rect(mouse_pos, QuitButtonBox))
                {
                    menuOpen = false;
                    *gameState = GAME_QUIT;
                }
            }
        }

    }

    // Free resources
    gf2d_sprite_free(background);
    gf2d_sprite_free(startButton);
    gf2d_sprite_free(quitButton);
}

void pq_PauseMenu(SDL_Renderer* renderer, GameState* gameState)
{
    slog("Game Paused!");
    Sprite* background = gf2d_sprite_load_image("images/backgrounds/Pause_Menu_Background.png");
    Bool menuOpen = true;

    // Load button sprites
    Sprite* resumeButton = gf2d_sprite_load_image("images/ui/Resume_Game_Button.png");
    Sprite* quitButton = gf2d_sprite_load_image("images/ui/Quit_Game_Button.png");

    // Button positions
    Vector2D resumeButtonPos = vector2d(512, 200);
    Vector2D quitButtonPos = vector2d(512, 400);

    while (menuOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Draw the buttons
        gf2d_sprite_draw_image(resumeButton, resumeButtonPos);
        gf2d_sprite_draw_image(quitButton, quitButtonPos);

        // Present the rendered frame
        gf2d_graphics_next_frame();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                Vector2D mouse_pos = { mouseX, mouseY };
                Rect ResumeButtonBox = { 512, 200, 256, 128 };
                Rect QuitButtonBox = { 512, 400, 256, 128 };

                // Check if the mouse click is within the bounds of the resume button
                if (gfc_point_in_rect(mouse_pos, ResumeButtonBox))
                {
                    *gameState = GAME_RUNNING;
                    menuOpen = false;
                }
                // Check if the mouse click is within the bounds of the quit button
                else if (gfc_point_in_rect(mouse_pos, QuitButtonBox))
                {
                    menuOpen = false;
                    *gameState = GAME_QUIT;
                }
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_ShowCursor(SDL_DISABLE);
                menuOpen = false;
                *gameState = GAME_RUNNING;
            }
        }
    }

    // Free resources
    gf2d_sprite_free(background);
    gf2d_sprite_free(resumeButton);
    gf2d_sprite_free(quitButton);
}

void pq_ShopMenu()
{
    slog("Shop Open!");
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();
    Sprite* background = gf2d_sprite_load_image("images/ui/Shop.png");
    Bool shopOpen = true;
    SDL_ShowCursor(SDL_ENABLE);

    while (shopOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Present the rendered frame
        gf2d_graphics_next_frame();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                Vector2D mouse_pos = { mouseX, mouseY };
                Rect maceBuyButtonBox = { 118, 391, 188, 103 };
                Rect wandBuyButtonBox = { 536, 390, 188, 103 };
                Rect staffBuyButtonBox = { 932, 390, 188, 103 };
                Rect quitButtonBox = { 1179, 73, 39, 45 };

                // Check if the mouse click is within the bounds of the mace buy button
                if (gfc_point_in_rect(mouse_pos, maceBuyButtonBox))
                {
                    shopOpen = false;
                }
                else if (gfc_point_in_rect(mouse_pos, wandBuyButtonBox))
                {
                    shopOpen = false;
                }
                else if (gfc_point_in_rect(mouse_pos, staffBuyButtonBox))
                {
                    shopOpen = false;
                }
                // Check if the mouse click is within the bounds of the quit button
                else if (gfc_point_in_rect(mouse_pos, quitButtonBox))
                {
                    shopOpen = false;
                }
            }
        }
    }

    // Free resources
    gf2d_sprite_free(background);
}

void pq_render_announcement(SDL_Color text_color, const char* msg, int pos_x, int pos_y, int seconds)
{
    TTF_Font* font = TTF_OpenFont(GAME_FONT, 48); // Adjust font size as needed
    if (!font) {
        slog("Error: Failed to load font - %s", TTF_GetError());
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, msg, text_color);
    if (!textSurface) {
        slog("Error: Failed to render text surface - %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Renderer* renderer = gf2d_graphics_get_renderer();

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        slog("Error: Failed to create texture from surface - %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect;
    textRect.x = (1280 - textSurface->w) / 2; // Adjust the position to center horizontally
    textRect.y = (720 - textSurface->h) / 8; // Adjust the position to center vertically
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    // Free resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

    // Delay for 5 seconds
    SDL_Delay(seconds);
}

void pq_render_player_health_bar(int player_health)
{
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();

    // Define the health bar dimensions and position
    int barWidth = 200;
    int barHeight = 20;
    int barPadding = 10;
    int barPosX = barPadding;
    int barPosY = barPadding;

    // Calculate the width of the filled portion of the health bar based on player health
    int filledWidth = (int)((float)player_health / 100 * (barWidth - 2)); // Subtract 2 for borders

    // Draw the health bar outline
    SDL_Rect outlineRect = { barPosX, barPosY, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderDrawRect(renderer, &outlineRect);

    // Draw the filled portion of the health bar
    SDL_Rect fillRect = { barPosX + 1, barPosY + 1, filledWidth, barHeight - 2 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_RenderFillRect(renderer, &fillRect);

    // Present the rendered health bar
    SDL_RenderPresent(renderer);
}
