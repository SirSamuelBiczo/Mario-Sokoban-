//
// Created by Megaport on 17/05/2021.
//

#include "Mario.h"
#include "Constants.h"
#include "core.h"


Mario* mario_create(SDL_Renderer* renderer, Tilemap* tilemap)
{
    Mario* mario = SDL_malloc(sizeof(*mario));

    if(mario == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_malloc: %s\n", SDL_GetError());
        return NULL;
    }

    mario->solved = img_load_texture(renderer, "./ressources/bravo.jpg");

    if(mario->solved == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Connot load solver image");
    }

    mario->displaySolvedImage = SDL_FALSE;
    mario->levelID = 1;
    tilemap_load_level(tilemap, mario->levelID);
    mario->tilePosition = mario_get_tile_position(tilemap->level);

    if(mario->tilePosition.y < 0 && mario->tilePosition.x < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Start mario position <unknow>");
    }
    else
    {
        mario->screenPosition.w = BLOCKSIZE;
        mario->screenPosition.h = BLOCKSIZE;
        mario->screenPosition.x = mario->tilePosition.x * BLOCKSIZE;
        mario->screenPosition.y = mario->tilePosition.y * BLOCKSIZE;
        mario->spriteID = MARIO_SPRITE_DOWN;

        for(int i = 0; i < NUMBER_OF_SPRITES; i++)
        {
            mario->sprites[i] = img_load_texture(renderer, MARIO_PATH[i]);
            SDL_Log("+Ressource allocated %p as SDL_Texture*", mario->sprites[i]);

            if(mario->sprites[i] == NULL)
            {
                SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "Impossible to load the tile number %d : %s\n", i, MARIO_PATH[i]);
                return NULL;
            }
        }

    }

    return mario;
}

void mario_delete(Mario* mario)
{
    SDL_DestroyTexture(mario->solved);

    for(int i = 0; i < NUMBER_OF_SPRITES; i++)
        SDL_DestroyTexture(mario->sprites[i]);

    SDL_free(mario);
}

State mario_update(Mario* mario, Tilemap* tilemap)
{
    SDL_PumpEvents();
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    if(tilemap_completed(tilemap))
    {
        if(mario->levelID == tilemap_number_of_levels(tilemap))
        {
            tilemap_delete(tilemap);
            mario_delete(mario);
            return MAIN_MENU;
        }
        else
        {
            if(keyboard[SDL_SCANCODE_RETURN])
            {
                mario->levelID++;
                mario_load_level(mario, tilemap);
                mario->displaySolvedImage = SDL_FALSE;
            }
            else
            {
                mario->displaySolvedImage = SDL_TRUE;
            }
        }
    }
    else
    {
        if(keyboard[SDL_SCANCODE_ESCAPE])
            return MAIN_MENU;

        if(keyboard[SDL_SCANCODE_PAGEUP] && mario->levelID < tilemap_number_of_levels(tilemap) - 1)
        {
            mario->levelID++;
            mario_load_level(mario, tilemap);
        }

        if(keyboard[SDL_SCANCODE_PAGEDOWN] && mario->levelID > 1)
        {
            mario->levelID--;
            mario_load_level(mario, tilemap);
        }

        if(keyboard[SDL_SCANCODE_R])
        {
            mario_load_level(mario, tilemap);
        }

        if(keyboard[SDL_SCANCODE_RIGHT])
        {
            mario_move(mario, tilemap->level, 1, 0);
            mario->spriteID = MARIO_SPRITE_RIGHT;
        }
        else if(keyboard[SDL_SCANCODE_LEFT])
        {
            mario_move(mario, tilemap->level, -1, 0);
            mario->spriteID = MARIO_SPRITE_LEFT;
        }
        else if(keyboard[SDL_SCANCODE_UP])
        {
            mario_move(mario, tilemap->level, 0, -1);
            mario->spriteID = MARIO_SPRITE_UP;
        }
        else if(keyboard[SDL_SCANCODE_DOWN])
        {
            mario_move(mario, tilemap->level, 0, 1);
            mario->spriteID = MARIO_SPRITE_DOWN;
        }
    }
    return IN_GAME;
}

void mario_draw(SDL_Renderer* renderer, Mario* mario)
{
    SDL_RenderCopy(renderer, mario->sprites[mario->spriteID], NULL, &mario->screenPosition);

    if(mario->displaySolvedImage)
    {
        SDL_Rect dst ={0, 0, 0, 0};

        SDL_QueryTexture(mario->solved, NULL, NULL, &dst.w, &dst.h);
        dst.x = WINDOW_WIDTH / 2 - dst.w / 2;
        dst.y = WINDOW_HEIGHT / 2 - dst.h / 2;
        SDL_RenderCopy(renderer, mario->solved, NULL, &dst);
    }

}

SDL_Point mario_get_tile_position(const char* level)
{
    SDL_Point start = {-1, -1};
    for(int i = 0; i < NUMBER_OF_TILES_VERTICAL; i++)
    {
        for(int j = 0; j < NUMBER_OF_TILES_VERTICAL; j++)
        {
            int k = (i * NUMBER_OF_TILES_HORIZONTAL) + j;

            if(level[k] == START_ID)
            {
                start.x = j;
                start.y = i;
            }
        }
    }
    return start;
}

void mario_move(Mario* mario, unsigned char* level, int x, int y)
{
    const int mario_level_position = ((mario->tilePosition.y + y) * NUMBER_OF_TILES_HORIZONTAL) + (mario->tilePosition.x + x);

    if(level[mario_level_position] == EMPTY_ID || level[mario_level_position] == BOX_ID || level[mario_level_position] == BOX_TARGETED_ID || level[mario_level_position] == TARGET_ID || level[mario_level_position] == START_ID)
    {
        if(level[mario_level_position] == EMPTY_ID || level[mario_level_position] == START_ID || level[mario_level_position] == TARGET_ID)
        {
            mario->tilePosition.x += x;
            mario->tilePosition.y += y;
            mario->screenPosition.x = mario->tilePosition.x * BLOCKSIZE;
            mario->screenPosition.y = mario->tilePosition.y * BLOCKSIZE;
        }

        if(level[mario_level_position] == BOX_TARGETED_ID)
        {
            int box_level_position = ((mario->tilePosition.y + y*2) * NUMBER_OF_TILES_HORIZONTAL) + (mario->tilePosition.x + x*2);

            if(level[box_level_position] != WALL_ID && level[box_level_position] != BOX_ID && level[box_level_position] != BOX_TARGETED_ID)
            {
                mario->tilePosition.x += x;
                mario->tilePosition.y += y;
                mario->screenPosition.x = mario->tilePosition.x * BLOCKSIZE;
                mario->screenPosition.y = mario->tilePosition.y * BLOCKSIZE;

                if(level[box_level_position] == TARGET_ID)
                {
                    level[box_level_position] = BOX_TARGETED_ID;
                }
                else
                {
                    level[box_level_position] = BOX_ID;
                }
                level[mario_level_position] = TARGET_ID;
            }
        }

        if(level[mario_level_position] == BOX_ID)
        {
            int box_level_position = ((mario->tilePosition.y + y*2) * NUMBER_OF_TILES_HORIZONTAL) + (mario->tilePosition.x + x*2);

            if(level[box_level_position] != WALL_ID && level[box_level_position] != BOX_ID && level[box_level_position] != BOX_TARGETED_ID)
            {
                mario->tilePosition.x += x;
                mario->tilePosition.y += y;
                mario->screenPosition.x = mario->tilePosition.x * BLOCKSIZE;
                mario->screenPosition.y = mario->tilePosition.y * BLOCKSIZE;

                if(level[box_level_position] == TARGET_ID)
                {
                    level[box_level_position] = BOX_TARGETED_ID;
                }
                else
                {
                    level[box_level_position] = BOX_ID;
                }
                level[mario_level_position] = EMPTY_ID;
            }
        }
    }
}

void mario_load_level(Mario* mario, Tilemap* tilemap)
{
    tilemap_load_level(tilemap, mario->levelID);
    mario->tilePosition = mario_get_tile_position(tilemap->level);
    mario->screenPosition.w = BLOCKSIZE;
    mario->screenPosition.h = BLOCKSIZE;
    mario->screenPosition.x = mario->tilePosition.x * BLOCKSIZE;
    mario->screenPosition.y = mario->tilePosition.y * BLOCKSIZE;
}