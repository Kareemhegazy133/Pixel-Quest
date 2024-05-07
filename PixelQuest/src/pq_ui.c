#include <simple_logger.h>
#include <gf2d_sprite.h>
#include <gfc_audio.h>
#include <gfc_shape.h>
#include <pq_ui.h>
#include <pq_item.h>
#include <pq_player.h>

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
    // Load game_running music
    Sound* gameRunningMusic = gfc_sound_load("sounds/gameplay_music.mp3", 0.5f, -1);
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
                    // Play game running music
                    gfc_sound_play(gameRunningMusic, -1, 0.0625f, 1, -1);
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

void pq_ShopMenu(pq_entity* player)
{
    slog("Shop Open!");
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();
    Sprite* background = gf2d_sprite_load_image("images/ui/Shop.png");
    Bool shopOpen = true;
    SDL_ShowCursor(SDL_ENABLE);

    int player_coins = 0;
    //TODO Find a better solution than hardcoding prices
    int mace_cost = 2;
    int wand_cost = 3;
    int staff_cost = 4;
    int food_cost = 1;

    TTF_Font* font = TTF_OpenFont(GAME_FONT, 32); // Adjust font size as needed
    if (!font) {
        slog("Error: Failed to load font - %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    while (shopOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Render player coins text
        pq_player_data* player_data = (pq_player_data*)player->data;
        if (player_data && player_data->inventory)
        {
            player_coins = pq_inventory_get_item_amount(player_data->inventory, "Coin");
        }

        char coinsText[50];
        snprintf(coinsText, 50, "%d", player_coins);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, coinsText, textColor);
        if (!textSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            break;
        }

        SDL_Rect textRect = { 40, 70, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Free text resources
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render mace coins text
        char macecoinsText[50];
        snprintf(macecoinsText, 50, "%d", mace_cost);
        SDL_Surface* maceTextSurface = TTF_RenderText_Solid(font, macecoinsText, textColor);
        if (!maceTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* maceTextTexture = SDL_CreateTextureFromSurface(renderer, maceTextSurface);
        if (!maceTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(maceTextSurface);
            break;
        }

        SDL_Rect macetextRect = { 200, 340, maceTextSurface->w, maceTextSurface->h };
        SDL_RenderCopy(renderer, maceTextTexture, NULL, &macetextRect);

        // Free text resources
        SDL_FreeSurface(maceTextSurface);
        SDL_DestroyTexture(maceTextTexture);

        // Render wand coins text
        char wandcoinsText[50];
        snprintf(wandcoinsText, 50, "%d", wand_cost);
        SDL_Surface* wandTextSurface = TTF_RenderText_Solid(font, wandcoinsText, textColor);
        if (!wandTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* wandTextTexture = SDL_CreateTextureFromSurface(renderer, wandTextSurface);
        if (!wandTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(wandTextSurface);
            break;
        }

        SDL_Rect wandtextRect = { 620, 340, wandTextSurface->w, wandTextSurface->h };
        SDL_RenderCopy(renderer, wandTextTexture, NULL, &wandtextRect);

        // Free text resources
        SDL_FreeSurface(wandTextSurface);
        SDL_DestroyTexture(wandTextTexture);

        // Render staff coins text
        char staffcoinsText[50];
        snprintf(staffcoinsText, 50, "%d", staff_cost);
        SDL_Surface* staffTextSurface = TTF_RenderText_Solid(font, staffcoinsText, textColor);
        if (!staffTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* staffTextTexture = SDL_CreateTextureFromSurface(renderer, staffTextSurface);
        if (!staffTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(staffTextSurface);
            break;
        }

        SDL_Rect stafftextRect = { 1010, 340, staffTextSurface->w, staffTextSurface->h };
        SDL_RenderCopy(renderer, staffTextTexture, NULL, &stafftextRect);

        // Free text resources
        SDL_FreeSurface(staffTextSurface);
        SDL_DestroyTexture(wandTextTexture);

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
                SJson* itemsList = get_pq_items_list();

                // Check if the mouse click is within the bounds of the mace buy button
                if (gfc_point_in_rect(mouse_pos, maceBuyButtonBox))
                {
                    //n is hardcoded aka 0 is mace, 1 is wand, 2 is staff, 3 is coin, 4 is food
                    SJson* mace_item_def = sj_array_get_nth(itemsList, 0);
                    if (!mace_item_def)
                    {
                        slog("Mace item is missing from the items list object in pq_items def file.");
                        return;
                    }

                    pq_entity* item = new_pq_item(mace_item_def);
                    item->collected = 1;
                    // if we have enough coins to buy
                    if (player_coins - item->cost >= 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Coin", item->cost);
                        pq_inventory_add_item(player_data->inventory, item);
                        slog("Purchased Mace!");
                        pq_render_text(RED, 24, 470, 650, "Purchased Mace!", 2);
                    }
                    else {
                        slog("Not Enough Coins to Purchase Mace");
                        pq_render_text(RED, 24, 300, 630, "Not Enough Coins to Purchase Mace", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, wandBuyButtonBox))
                {
                    SJson* wand_item_def = sj_array_get_nth(itemsList, 1);
                    if (!wand_item_def)
                    {
                        slog("Wand item is missing from the items list object in pq_items def file.");
                        return;
                    }

                    pq_entity* item = new_pq_item(wand_item_def);
                    item->collected = 1;
                    if (player_coins - item->cost >= 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Coin", item->cost);
                        pq_inventory_add_item(player_data->inventory, item);
                        slog("Purchased Wand!");
                        pq_render_text(RED, 24, 470, 650, "Purchased Wand!", 2);
                    }
                    else {
                        slog("Not Enough Coins to Purchase Wand");
                        pq_render_text(RED, 24, 300, 630, "Not Enough Coins to Purchase Wand", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, staffBuyButtonBox))
                {
                    SJson* staff_item_def = sj_array_get_nth(itemsList, 2);
                    if (!staff_item_def)
                    {
                        slog("Staff item is missing from the items list object in pq_items def file.");
                        return;
                    }

                    pq_entity* item = new_pq_item(staff_item_def);
                    item->collected = 1;
                    if (player_coins - item->cost >= 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Coin", item->cost);
                        pq_inventory_add_item(player_data->inventory, item);
                        slog("Purchased Staff!");
                        pq_render_text(RED, 24, 470, 650, "Purchased Staff!", 2);
                    }
                    else {
                        slog("Not Enough Coins to Purchase Staff");
                        pq_render_text(RED, 24, 300, 630, "Not Enough Coins to Purchase Staff", 2);
                    }
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
    TTF_CloseFont(font);
}

void pq_BlacksmithMenu(pq_entity* player)
{
    slog("Blacksmith Open!");
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();
    Sprite* background = gf2d_sprite_load_image("images/ui/Blacksmithing.png");
    Bool blacksmithOpen = true;
    SDL_ShowCursor(SDL_ENABLE);

    int player_diamonds = 0;
    //TODO Find a better solution than hardcoding prices
    int mace_upgrade_cost = 2;
    int wand_upgrade_cost = 3;
    int staff_upgrade_cost = 4;

    TTF_Font* font = TTF_OpenFont(GAME_FONT, 32); // Adjust font size as needed
    if (!font) {
        slog("Error: Failed to load font - %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    while (blacksmithOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Render player diamonds text
        pq_player_data* player_data = (pq_player_data*)player->data;
        if (player_data && player_data->inventory)
        {
            player_diamonds = pq_inventory_get_item_amount(player_data->inventory, "Diamond");
        }

        char diamondsText[50];
        snprintf(diamondsText, 50, "%d", player_diamonds);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, diamondsText, textColor);
        if (!textSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            break;
        }

        SDL_Rect textRect = { 40, 70, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Free text resources
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render mace coins text
        char macecoinsText[50];
        snprintf(macecoinsText, 50, "%d", mace_upgrade_cost);
        SDL_Surface* maceTextSurface = TTF_RenderText_Solid(font, macecoinsText, textColor);
        if (!maceTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* maceTextTexture = SDL_CreateTextureFromSurface(renderer, maceTextSurface);
        if (!maceTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(maceTextSurface);
            break;
        }

        SDL_Rect macetextRect = { 200, 340, maceTextSurface->w, maceTextSurface->h };
        SDL_RenderCopy(renderer, maceTextTexture, NULL, &macetextRect);

        // Free text resources
        SDL_FreeSurface(maceTextSurface);
        SDL_DestroyTexture(maceTextTexture);

        // Render wand coins text
        char wandcoinsText[50];
        snprintf(wandcoinsText, 50, "%d", wand_upgrade_cost);
        SDL_Surface* wandTextSurface = TTF_RenderText_Solid(font, wandcoinsText, textColor);
        if (!wandTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* wandTextTexture = SDL_CreateTextureFromSurface(renderer, wandTextSurface);
        if (!wandTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(wandTextSurface);
            break;
        }

        SDL_Rect wandtextRect = { 620, 340, wandTextSurface->w, wandTextSurface->h };
        SDL_RenderCopy(renderer, wandTextTexture, NULL, &wandtextRect);

        // Free text resources
        SDL_FreeSurface(wandTextSurface);
        SDL_DestroyTexture(wandTextTexture);

        // Render staff coins text
        char staffcoinsText[50];
        snprintf(staffcoinsText, 50, "%d", staff_upgrade_cost);
        SDL_Surface* staffTextSurface = TTF_RenderText_Solid(font, staffcoinsText, textColor);
        if (!staffTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* staffTextTexture = SDL_CreateTextureFromSurface(renderer, staffTextSurface);
        if (!staffTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(staffTextSurface);
            break;
        }

        SDL_Rect stafftextRect = { 1010, 340, staffTextSurface->w, staffTextSurface->h };
        SDL_RenderCopy(renderer, staffTextTexture, NULL, &stafftextRect);

        // Free text resources
        SDL_FreeSurface(staffTextSurface);
        SDL_DestroyTexture(staffTextTexture);

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
                Rect maceUpgradeButtonBox = { 118, 391, 188, 103 };
                Rect wandUpgradeButtonBox = { 536, 390, 188, 103 };
                Rect staffUpgradeButtonBox = { 932, 390, 188, 103 };
                Rect quitButtonBox = { 1179, 73, 39, 45 };

                // Check if the mouse click is within the bounds of the mace Upgrade button
                if (gfc_point_in_rect(mouse_pos, maceUpgradeButtonBox))
                {
                    if (gfc_word_cmp(player_data->equippedWeaponText, "Mace") == 0)
                    {
                        // if we have enough diamonds to buy
                        if (player_diamonds - mace_upgrade_cost >= 0)
                        {
                            pq_inventory_remove_item(player_data->inventory, "Diamond", mace_upgrade_cost);
                            // Loose binding with these vars with other menus
                            player_data->equippedWeapon->level++;
                            player_data->equippedWeaponLevel++;
                            player_data->equippedWeapon->damage += 5;
                            player_data->strength += 5;
                            slog("Upgraded Mace!");
                            pq_render_text(RED, 24, 470, 650, "Upgraded Mace!", 2);
                        }
                        else {
                            slog("Not Enough Diamonds to Upgrade Mace");
                            pq_render_text(RED, 24, 300, 630, "Not Enough Diamonds to Upgrade Mace", 2);
                        }
                    }
                    else {
                        slog("Mace must be equipped to be upgraded!");
                        pq_render_text(RED, 24, 300, 630, "Mace must be equipped to be upgraded!", 2);
                    }
                    
                }
                else if (gfc_point_in_rect(mouse_pos, wandUpgradeButtonBox))
                {
                    if (gfc_word_cmp(player_data->equippedWeaponText, "Wand") == 0)
                    {
                        if (player_diamonds - wand_upgrade_cost >= 0)
                        {
                            pq_inventory_remove_item(player_data->inventory, "Diamond", wand_upgrade_cost);
                            // Loose binding with these vars with other menus
                            player_data->equippedWeapon->level++;
                            player_data->equippedWeaponLevel++;
                            player_data->equippedWeapon->damage += 5;
                            player_data->magic += 5;
                            slog("Upgraded Wand!");
                            pq_render_text(RED, 24, 470, 650, "Upgraded Wand!", 2);
                        }
                        else {
                            slog("Not Enough Diamonds to Upgrade Wand");
                            pq_render_text(RED, 24, 300, 630, "Not Enough Diamonds to Upgrade Wand", 2);
                        }
                    }
                    else {
                        slog("Wand must be equipped to be upgraded!");
                        pq_render_text(RED, 24, 300, 630, "Wand must be equipped to be upgraded!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, staffUpgradeButtonBox))
                {
                    if (gfc_word_cmp(player_data->equippedWeaponText, "Staff") == 0)
                    {
                        if (player_diamonds - staff_upgrade_cost >= 0)
                        {
                            pq_inventory_remove_item(player_data->inventory, "Diamond", staff_upgrade_cost);
                            // Loose binding with these vars with other menus
                            player_data->equippedWeapon->level++;
                            player_data->equippedWeaponLevel++;
                            player_data->equippedWeapon->damage += 5;
                            player_data->spirit += 5;
                            slog("Upgraded Staff!");
                            pq_render_text(RED, 24, 470, 650, "Upgraded Staff!", 2);
                        }
                        else {
                            slog("Not Enough Diamonds to Upgrade Staff");
                            pq_render_text(RED, 24, 300, 630, "Not Enough Diamonds to Upgrade Staff", 2);
                        }
                    }
                    else {
                        slog("Staff must be equipped to be upgraded!");
                        pq_render_text(RED, 24, 300, 630, "Staff must be equipped to be upgraded!", 2);
                    }
                }
                // Check if the mouse click is within the bounds of the quit button
                else if (gfc_point_in_rect(mouse_pos, quitButtonBox))
                {
                    blacksmithOpen = false;
                }
            }
        }
    }

    // Free resources
    gf2d_sprite_free(background);
    TTF_CloseFont(font);
}

void pq_InventoryMenu(pq_entity* player)
{
    slog("Inventory Open!");
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();
    Sprite* background = gf2d_sprite_load_image("images/ui/Inventory.png");
    Bool inventoryOpen = true;
    SDL_ShowCursor(SDL_ENABLE);

    int player_coins = 0;
    int mace_count = 0;
    int wand_count = 0;
    int staff_count = 0;
    int bat_count = 0;
    int sickle_count = 0;

    TTF_Font* font = TTF_OpenFont(GAME_FONT, 32); // Adjust font size as needed
    if (!font) {
        slog("Error: Failed to load font - %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    while (inventoryOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        // Render player coins text
        pq_player_data* player_data = (pq_player_data*)player->data;
        if (player_data && player_data->inventory)
        {
            player_coins = pq_inventory_get_item_amount(player_data->inventory, "Coin");
            mace_count = player_data->inventory->mace_count;
            wand_count = player_data->inventory->wand_count;
            staff_count = player_data->inventory->staff_count;
            bat_count = player_data->inventory->bat_count;
            sickle_count = player_data->inventory->sickle_count;
        }

        char coinsText[50];
        snprintf(coinsText, 50, "%d", player_coins);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, coinsText, textColor);
        if (!textSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            break;
        }

        SDL_Rect textRect = { 40, 70, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Free text resources
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render mace coins text
        char macecoinsText[50];
        snprintf(macecoinsText, 50, "%d", mace_count);
        SDL_Surface* maceTextSurface = TTF_RenderText_Solid(font, macecoinsText, textColor);
        if (!maceTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* maceTextTexture = SDL_CreateTextureFromSurface(renderer, maceTextSurface);
        if (!maceTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(maceTextSurface);
            break;
        }

        SDL_Rect macetextRect = { 200, 290, maceTextSurface->w, maceTextSurface->h };
        SDL_RenderCopy(renderer, maceTextTexture, NULL, &macetextRect);

        // Free text resources
        SDL_FreeSurface(maceTextSurface);
        SDL_DestroyTexture(maceTextTexture);

        // Render wand coins text
        char wandcoinsText[50];
        snprintf(wandcoinsText, 50, "%d", wand_count);
        SDL_Surface* wandTextSurface = TTF_RenderText_Solid(font, wandcoinsText, textColor);
        if (!wandTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* wandTextTexture = SDL_CreateTextureFromSurface(renderer, wandTextSurface);
        if (!wandTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(wandTextSurface);
            break;
        }

        SDL_Rect wandtextRect = { 620, 290, wandTextSurface->w, wandTextSurface->h };
        SDL_RenderCopy(renderer, wandTextTexture, NULL, &wandtextRect);

        // Free text resources
        SDL_FreeSurface(wandTextSurface);
        SDL_DestroyTexture(wandTextTexture);

        // Render staff coins text
        char staffcoinsText[50];
        snprintf(staffcoinsText, 50, "%d", staff_count);
        SDL_Surface* staffTextSurface = TTF_RenderText_Solid(font, staffcoinsText, textColor);
        if (!staffTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* staffTextTexture = SDL_CreateTextureFromSurface(renderer, staffTextSurface);
        if (!staffTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(staffTextSurface);
            break;
        }

        SDL_Rect stafftextRect = { 1010, 290, staffTextSurface->w, staffTextSurface->h };
        SDL_RenderCopy(renderer, staffTextTexture, NULL, &stafftextRect);

        // Free text resources
        SDL_FreeSurface(staffTextSurface);
        SDL_DestroyTexture(staffTextTexture);

        // Render bat coins text
        char batcoinsText[50];
        snprintf(batcoinsText, 50, "%d", bat_count);
        SDL_Surface* batTextSurface = TTF_RenderText_Solid(font, batcoinsText, textColor);
        if (!batTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* batTextTexture = SDL_CreateTextureFromSurface(renderer, batTextSurface);
        if (!batTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(batTextSurface);
            break;
        }

        SDL_Rect battextRect = { 200, 550, batTextSurface->w, batTextSurface->h };
        SDL_RenderCopy(renderer, batTextTexture, NULL, &battextRect);

        // Free text resources
        SDL_FreeSurface(batTextSurface);
        SDL_DestroyTexture(batTextTexture);

        // Render sickle coins text
        char sicklecoinsText[50];
        snprintf(sicklecoinsText, 50, "%d", sickle_count);
        SDL_Surface* sickleTextSurface = TTF_RenderText_Solid(font, sicklecoinsText, textColor);
        if (!sickleTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* sickleTextTexture = SDL_CreateTextureFromSurface(renderer, sickleTextSurface);
        if (!sickleTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(sickleTextSurface);
            break;
        }

        SDL_Rect sickletextRect = { 1010, 550, sickleTextSurface->w, sickleTextSurface->h };
        SDL_RenderCopy(renderer, sickleTextTexture, NULL, &sickletextRect);

        // Free text resources
        SDL_FreeSurface(sickleTextSurface);
        SDL_DestroyTexture(sickleTextTexture);

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
                Rect maceUseButtonBox = { 142, 360, 156, 86 };
                Rect wandUseButtonBox = { 562, 360, 156, 86 };
                Rect staffUseButtonBox = { 944, 360, 156, 86 };
                Rect batUseButtonBox = { 147, 618, 156, 86 };
                Rect sickleUseButtonBox = { 955, 625, 156, 86 };
                Rect quitButtonBox = { 1179, 73, 39, 45 };
                SJson* itemsList = get_pq_items_list();

                // Check if the mouse click is within the bounds of the mace buy button
                if (gfc_point_in_rect(mouse_pos, maceUseButtonBox))
                {
                    if (pq_inventory_get_item_amount(player_data->inventory, "Mace") > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Mace", 1);
                        pq_entity* weapon = new_pq_item(sj_array_get_nth(itemsList, 0));
                        weapon->_is_active = 0;
                        player_data->equippedWeapon = weapon;
                        gfc_word_cpy(player_data->equippedWeaponText, weapon->display_name);
                        player_data->equippedWeaponLevel = weapon->level;
                        player->equippedWeapon = 1;
                        player_data->strength += weapon->damage;
                        slog("Equipped Mace");
                        pq_render_text(RED, 24, 470, 650, "Equipped Mace", 2);
                    }
                    else {
                        slog("No Mace to Equip!");
                        pq_render_text(RED, 24, 470, 650, "No Mace to Equip!", 2);
                    }
                    
                }
                else if (gfc_point_in_rect(mouse_pos, wandUseButtonBox))
                {
                    if (pq_inventory_get_item_amount(player_data->inventory, "Wand") > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Wand", 1);
                        pq_entity* weapon = new_pq_item(sj_array_get_nth(itemsList, 1));
                        weapon->_is_active = 0;
                        player_data->equippedWeapon = weapon;
                        gfc_word_cpy(player_data->equippedWeaponText, weapon->display_name);
                        player_data->equippedWeaponLevel = weapon->level;
                        player->equippedWeapon = 1;
                        player_data->magic += weapon->damage;
                        slog("Equipped Wand");
                        pq_render_text(RED, 24, 470, 650, "Equipped Wand", 2);
                    }
                    else {
                        slog("No Wand to Equip!");
                        pq_render_text(RED, 24, 470, 650, "No Wand to Equip!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, staffUseButtonBox))
                {
                    if (pq_inventory_get_item_amount(player_data->inventory, "Staff") > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Staff", 1);
                        pq_entity* weapon = new_pq_item(sj_array_get_nth(itemsList, 2));
                        weapon->_is_active = 0;
                        player_data->equippedWeapon = weapon;
                        gfc_word_cpy(player_data->equippedWeaponText, weapon->display_name);
                        player_data->equippedWeaponLevel = weapon->level;
                        player->equippedWeapon = 1;
                        player_data->spirit += weapon->damage;
                        slog("Equipped Staff");
                        pq_render_text(RED, 24, 470, 650, "Equipped Staff", 2);
                    }
                    else {
                        slog("No Staff to Equip!");
                        pq_render_text(RED, 24, 470, 650, "No Staff to Equip!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, batUseButtonBox))
                {
                    if (pq_inventory_get_item_amount(player_data->inventory, "Bat") > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Bat", 1);
                        pq_entity* weapon = new_pq_item(sj_array_get_nth(itemsList, 3));
                        weapon->_is_active = 0;
                        player_data->equippedWeapon = weapon;
                        gfc_word_cpy(player_data->equippedWeaponText, weapon->display_name);
                        player_data->equippedWeaponLevel = weapon->level;
                        player->equippedWeapon = 1;
                        player_data->strength += weapon->damage;
                        player_data->defense += weapon->damage;
                        slog("Equipped Bat");
                        pq_render_text(RED, 24, 470, 650, "Equipped Bat", 2);
                    }
                    else {
                        slog("No Bat to Equip!");
                        pq_render_text(RED, 24, 470, 650, "No Bat to Equip!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, sickleUseButtonBox))
                {
                    if (pq_inventory_get_item_amount(player_data->inventory, "Sickle") > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Sickle", 1);
                        pq_entity* weapon = new_pq_item(sj_array_get_nth(itemsList, 4));
                        weapon->_is_active = 0;
                        player_data->equippedWeapon = weapon;
                        gfc_word_cpy(player_data->equippedWeaponText, weapon->display_name);
                        player_data->equippedWeaponLevel = weapon->level;
                        player->equippedWeapon = 1;
                        player_data->magic += weapon->damage;
                        player_data->defense += weapon->damage;
                        slog("Equipped Sickle");
                        pq_render_text(RED, 24, 470, 650, "Equipped Sickle", 2);
                    }
                    else {
                        slog("No Sickle to Equip!");
                        pq_render_text(RED, 24, 470, 650, "No Sickle to Equip!", 2);
                    }
                }
                // Check if the mouse click is within the bounds of the quit button
                else if (gfc_point_in_rect(mouse_pos, quitButtonBox))
                {
                    inventoryOpen = false;
                }
            }
        }
    }

    // Free resources
    gf2d_sprite_free(background);
    TTF_CloseFont(font);
}

void pq_StatsMenu(pq_entity* player)
{
    slog("Stats Open!");
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();
    Sprite* background = gf2d_sprite_load_image("images/ui/Stats.png");
    Bool statsOpen = true;
    SDL_ShowCursor(SDL_ENABLE);

    int player_diamonds = 0;
    int player_level = 0;
    int vigor_level = 0;
    int strength_level = 0;
    int defense_level = 0;
    int magic_level = 0;
    int spirit_level = 0;

    TTF_Font* font = TTF_OpenFont(GAME_FONT, 32); // Adjust font size as needed
    if (!font) {
        slog("Error: Failed to load font - %s", TTF_GetError());
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    while (statsOpen)
    {
        // Clear the screen
        gf2d_graphics_clear_screen();

        // Draw the background
        gf2d_sprite_draw_image(background, vector2d(0, 0));

        pq_player_data* player_data = (pq_player_data*)player->data;
        if (player_data && player_data->inventory)
        {
            player_diamonds = pq_inventory_get_item_amount(player_data->inventory, "Diamond");
            player_level = player_data->level;
            vigor_level = player_data->vigor;
            strength_level = player_data->strength;
            defense_level = player_data->defense;
            magic_level = player_data->magic;
            spirit_level = player_data->spirit;
        }

        char diamondsText[50];
        snprintf(diamondsText, 50, "%d", player_diamonds);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, diamondsText, textColor);
        if (!textSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            break;
        }

        SDL_Rect textRect = { 160, 80, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Free text resources
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Render current level text
        char currentlevelText[50];
        snprintf(currentlevelText, 50, "%d", player_level);
        SDL_Surface* currentlevelTextSurface = TTF_RenderText_Solid(font, currentlevelText, textColor);
        if (!currentlevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* currentlevelTextTexture = SDL_CreateTextureFromSurface(renderer, currentlevelTextSurface);
        if (!currentlevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(currentlevelTextSurface);
            break;
        }

        SDL_Rect currentleveltextRect = { 250, 300, currentlevelTextSurface->w, currentlevelTextSurface->h };
        SDL_RenderCopy(renderer, currentlevelTextTexture, NULL, &currentleveltextRect);

        // Free text resources
        SDL_FreeSurface(currentlevelTextSurface);
        SDL_DestroyTexture(currentlevelTextTexture);

        // Render current weapon text
        char currentweaponText[50];
        snprintf(currentweaponText, 50, "%s: %d", player_data->equippedWeaponText, player_data->equippedWeaponLevel);
        SDL_Surface* currentweaponTextSurface = TTF_RenderText_Solid(font, currentweaponText, textColor);
        if (!currentlevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* currentweaponTextTexture = SDL_CreateTextureFromSurface(renderer, currentweaponTextSurface);
        if (!currentweaponTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(currentweaponTextSurface);
            break;
        }

        SDL_Rect currentweapontextRect = { 160, 350, currentweaponTextSurface->w, currentweaponTextSurface->h };
        SDL_RenderCopy(renderer, currentweaponTextTexture, NULL, &currentweapontextRect);

        // Free text resources
        SDL_FreeSurface(currentweaponTextSurface);
        SDL_DestroyTexture(currentweaponTextTexture);

        // Render vigor level text
        char vigorlevelText[50];
        snprintf(vigorlevelText, 50, "%d", vigor_level);
        SDL_Surface* vigorlevelTextSurface = TTF_RenderText_Solid(font, vigorlevelText, textColor);
        if (!vigorlevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* vigorlevelTextTexture = SDL_CreateTextureFromSurface(renderer, vigorlevelTextSurface);
        if (!vigorlevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(vigorlevelTextSurface);
            break;
        }

        SDL_Rect vigorleveltextRect = { 750, 230, vigorlevelTextSurface->w, vigorlevelTextSurface->h };
        SDL_RenderCopy(renderer, vigorlevelTextTexture, NULL, &vigorleveltextRect);

        // Free text resources
        SDL_FreeSurface(vigorlevelTextSurface);
        SDL_DestroyTexture(vigorlevelTextTexture);

        // Render strength level text
        char strengthlevelText[50];
        snprintf(strengthlevelText, 50, "%d", strength_level);
        SDL_Surface* strengthlevelTextSurface = TTF_RenderText_Solid(font, strengthlevelText, textColor);
        if (!strengthlevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* strengthlevelTextTexture = SDL_CreateTextureFromSurface(renderer, strengthlevelTextSurface);
        if (!strengthlevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(strengthlevelTextSurface);
            break;
        }

        SDL_Rect strengthleveltextRect = { 1150, 230, strengthlevelTextSurface->w, strengthlevelTextSurface->h };
        SDL_RenderCopy(renderer, strengthlevelTextTexture, NULL, &strengthleveltextRect);

        // Free text resources
        SDL_FreeSurface(strengthlevelTextSurface);
        SDL_DestroyTexture(strengthlevelTextTexture);

        // Render defense level text
        char defenselevelText[50];
        snprintf(defenselevelText, 50, "%d", defense_level);
        SDL_Surface* defenselevelTextSurface = TTF_RenderText_Solid(font, defenselevelText, textColor);
        if (!defenselevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* defenselevelTextTexture = SDL_CreateTextureFromSurface(renderer, defenselevelTextSurface);
        if (!defenselevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(defenselevelTextSurface);
            break;
        }

        SDL_Rect defenseleveltextRect = { 400, 460, defenselevelTextSurface->w, defenselevelTextSurface->h };
        SDL_RenderCopy(renderer, defenselevelTextTexture, NULL, &defenseleveltextRect);

        // Free text resources
        SDL_FreeSurface(defenselevelTextSurface);
        SDL_DestroyTexture(defenselevelTextTexture);

        // Render magic level text
        char magiclevelText[50];
        snprintf(magiclevelText, 50, "%d", magic_level);
        SDL_Surface* magiclevelTextSurface = TTF_RenderText_Solid(font, magiclevelText, textColor);
        if (!magiclevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* magiclevelTextTexture = SDL_CreateTextureFromSurface(renderer, magiclevelTextSurface);
        if (!magiclevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(magiclevelTextSurface);
            break;
        }

        SDL_Rect magicleveltextRect = { 750, 460, magiclevelTextSurface->w, magiclevelTextSurface->h };
        SDL_RenderCopy(renderer, magiclevelTextTexture, NULL, &magicleveltextRect);

        // Free text resources
        SDL_FreeSurface(magiclevelTextSurface);
        SDL_DestroyTexture(magiclevelTextTexture);

        // Render spirit level text
        char spiritlevelText[50];
        snprintf(spiritlevelText, 50, "%d", spirit_level);
        SDL_Surface* spiritlevelTextSurface = TTF_RenderText_Solid(font, spiritlevelText, textColor);
        if (!spiritlevelTextSurface)
        {
            slog("Failed to create text surface: %s", TTF_GetError());
            break;
        }

        SDL_Texture* spiritlevelTextTexture = SDL_CreateTextureFromSurface(renderer, spiritlevelTextSurface);
        if (!spiritlevelTextTexture)
        {
            slog("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(spiritlevelTextSurface);
            break;
        }

        SDL_Rect spiritleveltextRect = { 1150, 460, spiritlevelTextSurface->w, spiritlevelTextSurface->h };
        SDL_RenderCopy(renderer, spiritlevelTextTexture, NULL, &spiritleveltextRect);

        // Free text resources
        SDL_FreeSurface(spiritlevelTextSurface);
        SDL_DestroyTexture(spiritlevelTextTexture);

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
                Rect vigorUpgradeButtonBox = { 546, 291, 188, 103 };
                Rect strengthUpgradeButtonBox = { 910, 295, 188, 103 };
                Rect defenseUpgradeButtonBox = { 146, 524, 188, 103 };
                Rect magicUpgradeButtonBox = { 546, 521, 188, 103 };
                Rect spiritUpgradeButtonBox = { 920, 523, 188, 103 };
                Rect quitButtonBox = { 1179, 73, 39, 45 };

                if (gfc_point_in_rect(mouse_pos, vigorUpgradeButtonBox))
                {
                    if (player_diamonds > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Diamond", 1);
                        player_data->level++;
                        player_data->vigor++;
                        player->max_health += 20;
                        slog("Upgraded Vigor");
                    }
                    else {
                        slog("Not Enough Diamonds to Level Up!");
                        pq_render_text(RED, 24, 320, 650, "Not Enough Diamonds to Level Up!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, strengthUpgradeButtonBox))
                {
                    if (player_diamonds > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Diamond", 1);
                        player_data->level++;
                        player_data->strength++;
                        slog("Upgraded Strength");
                    }
                    else {
                        slog("Not Enough Diamonds to Level Up!");
                        pq_render_text(RED, 24, 320, 650, "Not Enough Diamonds to Level Up!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, defenseUpgradeButtonBox))
                {
                    if (player_diamonds > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Diamond", 1);
                        player_data->level++;
                        player_data->defense++;
                        slog("Upgraded Defense");
                    }
                    else {
                        slog("Not Enough Diamonds to Level Up!");
                        pq_render_text(RED, 24, 320, 650, "Not Enough Diamonds to Level Up!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, magicUpgradeButtonBox))
                {
                    if (player_diamonds > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Diamond", 1);
                        player_data->level++;
                        player_data->magic++;
                        slog("Upgraded Magic");
                    }
                    else {
                        slog("Not Enough Diamonds to Level Up!");
                        pq_render_text(RED, 24, 320, 650, "Not Enough Diamonds to Level Up!", 2);
                    }
                }
                else if (gfc_point_in_rect(mouse_pos, spiritUpgradeButtonBox))
                {
                    if (player_diamonds > 0)
                    {
                        pq_inventory_remove_item(player_data->inventory, "Diamond", 1);
                        player_data->level++;
                        player_data->spirit++;
                        slog("Upgraded Spirit");
                    }
                    else {
                        slog("Not Enough Diamonds to Level Up!");
                        pq_render_text(RED, 24, 320, 650, "Not Enough Diamonds to Level Up!", 2);
                    }
                }

                // Check if the mouse click is within the bounds of the quit button
                if (gfc_point_in_rect(mouse_pos, quitButtonBox))
                {
                    statsOpen = false;
                }
            }
        }
    }

    // Free resources
    gf2d_sprite_free(background);
    TTF_CloseFont(font);
}

void pq_render_announcement(SDL_Color text_color, const char* msg, int seconds)
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
    SDL_Delay(seconds * 1000);
}

void pq_render_text(SDL_Color text_color, int font_size, int pos_x, int pos_y, const char* msg, int seconds)
{
    TTF_Font* font = TTF_OpenFont(GAME_FONT, font_size); // Adjust font size as needed
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
    textRect.x = pos_x;
    textRect.y = pos_y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_RenderPresent(renderer);

    // Free resources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

    // Delay for 5 seconds
    SDL_Delay(seconds * 1000);
}

void pq_render_player_health_bar(pq_entity* player)
{
    SDL_Renderer* renderer = gf2d_graphics_get_renderer();

    // Define the health bar dimensions and position
    int barWidth = player->max_health;
    int barHeight = 20;
    int barPadding = 10;
    int barPosX = barPadding;
    int barPosY = barPadding;

    // Calculate the width of the filled portion of the health bar based on player health
    int filledWidth = (int)((float)player->health / 100 * (barWidth - 2)); // Subtract 2 for borders

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
