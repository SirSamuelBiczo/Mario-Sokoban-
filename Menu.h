//
// Created by Megaport on 22/05/2021.
//

#ifndef MARIOSOKOBAN_MENU_H
#define MARIOSOKOBAN_MENU_H

#include <SDL2/SDL.h>
#include "State.h"
enum
{
    MAIN_MENU_TEXT = 0,
    GAME_INSTRUCTION_TEXT = 1,
    EDITOR_INSTRUCTION_TEXT = 2
};

typedef struct
{
    State state;
    SDL_Texture* sprites[3];
}Menu;

/*
 * void -> Menu*
 *
 * Initialize, la structure permetant de manipuler le menu du jeux
 * */
Menu* menu_create(SDL_Renderer* renderer);
/*
 * Menu* -> void
 *
 * Libère les ressources associer a un menu
 * */
void menu_delete(Menu* menu);
/*
 * Menu* -> State
 *
 * Mets a jour le menu et retourne si on nous somme dans le menu ou dans le jeux
 * */
State menu_update(Menu* menu, SDL_Event* events, SDL_bool* closeApp);
/*
 *  Menu* -> void
 *
 *  Dessine le menu
 * */
void menu_draw(Menu* menu, SDL_Renderer* renderer);


#endif //MARIOSOKOBAN_MENU_H
