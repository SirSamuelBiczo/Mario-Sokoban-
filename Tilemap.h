//
// Created by Megaport on 17/05/2021.
//

#ifndef MARIOSOKOBAN_TILEMAP_H
#define MARIOSOKOBAN_TILEMAP_H

#include <SDL2/SDL.h>

static const char* TILES_PATH[255] = {"./ressources/caisse.jpg", "./ressources/caisse_ok.jpg", "./ressources/mur.jpg", "./ressources/objectif.png", "./ressources/mario_bas.gif"};

typedef enum TilemapID
{
    EMPTY_ID = '0',
    WALL_ID = '1',
    BOX_ID = '2',
    TARGET_ID = '3',
    START_ID = '4',
    BOX_TARGETED_ID = '5'
}TilemapID;

enum
{
    WALL_TEXT = 2,
    BOX_TEXT = 0,
    BOX_TARGETED_TEXT = 1,
    TARGET_TEXT = 3,
    START_TEXT = 4
};


typedef struct
{
    char* path;
    SDL_Texture* sprites[5];
    Uint8* level;
    Uint8 w;
    Uint8 h;
}Tilemap;

Tilemap* tilemap_create(SDL_Renderer* renderer, char* filepath);
void tilemap_delete(Tilemap* tilemap);
void tilemap_draw(SDL_Renderer* renderer, Tilemap* tilemap);
void tilemap_display_console(Tilemap* tilemap);
Uint8 tilemap_count_id(Tilemap* tilemap, TilemapID id);
SDL_bool tilemap_completed(Tilemap* tilemap);
Uint32 tilemap_number_of_levels(Tilemap* tilemap);
void tilemap_load_level(Tilemap* tilemap, Uint32 levelID);



#endif //MARIOSOKOBAN_TILEMAP_H
