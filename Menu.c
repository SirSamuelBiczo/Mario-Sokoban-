//
// Created by Megaport on 22/05/2021.
//

#include "Menu.h"
#include "core.h"

Menu* menu_create(SDL_Renderer* renderer)
{
    const char* paths[255] = {"./ressources/menu.jpg", "./ressources/instructions_jeu.jpg","./ressources/instructions.jpg"};
    Menu* menu = SDL_malloc(sizeof(*menu));

    if(menu == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_malloc: %s", SDL_GetError());
        return NULL;
    }

    menu->state = MAIN_MENU;

    SDL_Log("+Ressource allocated %p as Menu*", menu);

    for(int i = 0; i < 3; i++)
    {
        menu->sprites[i] = img_load_texture(renderer, paths[i]);
        SDL_Log("+Ressource allocated %p as SDL_Texture*",  menu->sprites[i]);
    }

    return menu;
}

void menu_delete(Menu* menu)
{
    for(int i = 0; i < 3; i++)
    {
        SDL_Log("-Ressource liberated %p as SDL_Texture*",  menu->sprites[i]);
        SDL_DestroyTexture(menu->sprites[i]);

    }

    SDL_Log("-Ressource liberated %p as Menu*", menu);
    SDL_free(menu);
}

State menu_update(Menu* menu, SDL_Event* events, SDL_bool* closeApp)
{
    static SDL_bool pressed = SDL_FALSE;

    if(events->type == SDL_QUIT || events->type == SDL_KEYDOWN && events->key.keysym.sym == SDLK_ESCAPE)
    {
        if(menu->state == MAIN_MENU)
        {
            *closeApp = SDL_TRUE;
        }
        else
        {
            pressed = SDL_FALSE;
            menu->state = MAIN_MENU;
        }
    }

    if(events->type == SDL_KEYDOWN && pressed)
        return menu->state;

    if(events->type == SDL_KEYDOWN && events->key.keysym.sym == SDLK_1 && menu->state == MAIN_MENU)
    {
        menu->state = IN_GAME;
        pressed = SDL_TRUE;
    }

    if(events->type == SDL_KEYDOWN && events->key.keysym.sym == SDLK_2 && menu->state == MAIN_MENU)
    {
        menu->state = IN_EDITOR;
        pressed = SDL_TRUE;
    }

    if(events->type == SDL_KEYDOWN && events->key.keysym.sym == SDLK_ESCAPE && menu->state == IN_GAME || menu->state == IN_EDITOR)
        return MAIN_MENU;

    return MAIN_MENU;
}

void menu_draw(Menu* menu, SDL_Renderer* renderer)
{
    switch(menu->state)
    {
        case IN_EDITOR:
            SDL_RenderCopy(renderer, menu->sprites[EDITOR_INSTRUCTION_TEXT], NULL, NULL);
            break;
        case IN_GAME:
            SDL_RenderCopy(renderer, menu->sprites[GAME_INSTRUCTION_TEXT], NULL, NULL);
            break;
        default:
            SDL_RenderCopy(renderer, menu->sprites[MAIN_MENU_TEXT], NULL, NULL);
            break;
    }
}