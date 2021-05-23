//
// Created by Megaport on 16/05/2021.
//

#include "core.h"


SDL_bool initialize_sdl(SDL_Window** pp_Window, SDL_Renderer** pp_Renderer)
{
    SDL_assert(*pp_Window == NULL);
    SDL_assert(*pp_Renderer == NULL);

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, pp_Window, pp_Renderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    SDL_Surface* icon = IMG_Load("./ressources/caisse.jpg");

    SDL_SetWindowIcon(*pp_Window, icon);

    SDL_FreeSurface(icon);

    SDL_SetWindowTitle(*pp_Window, WINDOW_TITLE);

    return SDL_TRUE;
}

SDL_Texture* img_load_texture(SDL_Renderer* renderer, const char* path)
{
    SDL_assert(renderer != NULL);

    SDL_Surface* surface = IMG_Load(path);

    if(surface == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if(texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load: %s\n", IMG_GetError());
        return NULL;
    }

    return texture;
}

void free_sdl(SDL_Window* window, SDL_Renderer* renderer, Mario* mario, Tilemap* tilemap, LevelEditor* editor, Menu* menu)
{
    menu_delete(menu);
    leveleditor_delete(editor);
    tilemap_delete(tilemap);
    mario_delete(mario);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

