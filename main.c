#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core.h"
#include "State.h"
#include "Mario.h"
#include "Tilemap.h"
#include "LevelEditor.h"
#include "Menu.h"

#undef main

int main(int argc, char** argv)
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_bool closeApp = SDL_FALSE;
    SDL_Event events;

    if(!initialize_sdl(&window, &renderer))
    {
        SDL_Log("Connot initialize SDL...");
        return EXIT_FAILURE;
    }

    ////////////////////////////////////////////////
    State state_game = MAIN_MENU;

    Tilemap* tilemap = tilemap_create(renderer, "./niveaux/levels.txt");

    if(tilemap == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "tilemap connot be initialized");
    }

    Mario* mario = mario_create(renderer, tilemap);

    if(mario == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "mario connot be initialized");
    }

    LevelEditor* editor = leveleditor_create();

    if(editor == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "leveleditor_create connot be initialized");
    }

    Menu* menu = menu_create(renderer);

    if(menu == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Menu connot be initialized");
    }

    while(!closeApp)
    {
        while(SDL_PollEvent(&events))
        {
            if(events.type == SDL_QUIT)
            {
                closeApp = SDL_TRUE;
            }
            else
            {
                if(state_game == MAIN_MENU)
                {
                    state_game = menu_update(menu, &events, &closeApp);
                }

                switch(state_game)
                {
                    case IN_GAME:
                        state_game = mario_update(mario, tilemap);

                        if(state_game == MAIN_MENU)
                        {
                            tilemap = tilemap_create(renderer, "./niveaux/levels.txt");
                            mario = mario_create(renderer, tilemap);
                        }

                        break;
                    case IN_EDITOR:
                        state_game = leveleditor_update(editor);
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 25, 10, 10, 255);
        SDL_RenderClear(renderer);

        switch(state_game)
        {
            case MAIN_MENU:
                menu_draw(menu, renderer);
                break;
            case IN_GAME:
                    //tilemap_display_console(tilemap);
                    tilemap_draw(renderer, tilemap);
                    mario_draw(renderer, mario);

                break;
            case IN_EDITOR:
                leveleditor_draw(editor, tilemap, renderer);
                break;
            default:
                SDL_Log("<UNKNOW DRAW>");
                break;
        }

        SDL_RenderPresent(renderer);
    }

    free_sdl(window, renderer, mario, tilemap, editor, menu);

    return EXIT_SUCCESS;
}
