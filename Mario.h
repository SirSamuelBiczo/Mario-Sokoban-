//
// Created by Megaport on 17/05/2021.
//

#ifndef MARIOSOKOBAN_MARIO_H
#define MARIOSOKOBAN_MARIO_H

#include <SDL2/SDL.h>
#include "Tilemap.h"
#include "State.h"

#define NUMBER_OF_SPRITES 4
static const char* MARIO_PATH[255] = {"./ressources/mario_bas.gif", "./ressources/mario_haut.gif", "./ressources/mario_droite.gif", "./ressources/mario_gauche.gif"};

/*
 * Enumeration qui contient l'index associé a l'image de la Texture
 * */
typedef enum
{
    MARIO_SPRITE_DOWN = 0,
    MARIO_SPRITE_UP = 1,
    MARIO_SPRITE_RIGHT = 2,
    MARIO_SPRITE_LEFT = 3
}SpriteID;

typedef struct
{
    SDL_Point tilePosition;
    SDL_Rect screenPosition;
    SDL_Texture* sprites[4];
    SDL_Texture* solved;
    SDL_bool displaySolvedImage;
    SpriteID spriteID;
    SDL_bool* target_ok;
    Uint32 levelID;
}Mario;

Mario* mario_create(SDL_Renderer* renderer, Tilemap* tilemap);
/*
 * Mario* -> void
 * Destructeur du mario
 * */
void mario_delete(Mario* mario);
/*
 * Mario* -> State
 *
 * Mise a jour du mario
 * */
State mario_update(Mario* mario, Tilemap* tilemap);
/*
 * SDL_Renderer* -> void
 *
 * Dessine Mario a l'écran
 * */
void mario_draw(SDL_Renderer* renderer, Mario* mario);

/*
 * char* -> SDL_Point
 *
 * Retourne le point de départ du personnage d'après le niveau
 * */
SDL_Point mario_get_tile_position(const char* level);
/*
 * Mario*, char* -> void
 *
 * Deplace le mario si possible
 * */
void mario_move(Mario* mario, unsigned char* level, int x, int y);
/*
 *
 * */
void mario_load_level(Mario* mario, Tilemap* tilemap);

#endif //MARIOSOKOBAN_MARIO_H
