//
// Created by Megaport on 17/05/2021.
//
#include "Tilemap.h"
#include "Constants.h"
#include "core.h"
#include <stdio.h>

Tilemap* tilemap_create(SDL_Renderer* renderer, char* filepath)
{
    Tilemap* tilemap = SDL_malloc(sizeof(*tilemap));

    if(tilemap == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_malloc: %s", SDL_GetError());
    }
    else
    {
        SDL_Log("+Ressource allocated %p as Tilemap", tilemap);


        tilemap->path = filepath;
        tilemap->level = SDL_malloc(sizeof(Uint8)*NUMBER_OF_TILES_VERTICAL * NUMBER_OF_TILES_HORIZONTAL);

        if(tilemap->level == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_malloc: %s", SDL_GetError());
            return NULL;
        }

        SDL_Log("+Ressource allocated %p as Uint8*", tilemap->level);
        for(int i = 0; i < 5; i++)
        {
            tilemap->sprites[i] = img_load_texture(renderer, TILES_PATH[i]);

            if(tilemap->sprites[i] == NULL)
            {
                SDL_Log(SDL_LOG_CATEGORY_APPLICATION, "Impossible to load the tile number %d : %s\n", i, TILES_PATH[i]);
            }
        }
    }

    return tilemap;
}

void tilemap_delete(Tilemap* tilemap)
{
    for(int i = 0; i < 5; i++)
    {
        SDL_Log("-Ressource liberated %p as SDL_Texture*", tilemap->sprites[i]);
        SDL_DestroyTexture(tilemap->sprites[i]);
    }    
    SDL_Log("-Ressource liberated %p as Tilemap*", tilemap);
    SDL_free(tilemap);
}
void tilemap_draw(SDL_Renderer* renderer, Tilemap* tilemap)
{
    SDL_Texture* tile = NULL;
    SDL_Rect dst;
    dst.w = BLOCKSIZE;
    dst.h = BLOCKSIZE;

    for(int i = 0; i < NUMBER_OF_TILES_VERTICAL; i++)
    {
        for (int j = 0; j < NUMBER_OF_TILES_HORIZONTAL; j++)
        {
            int index = (i * NUMBER_OF_TILES_HORIZONTAL) + j;
            char id = tilemap->level[index];

            dst.x = j * BLOCKSIZE;
            dst.y = i * BLOCKSIZE;

            switch(id)
            {
                case WALL_ID:
                    tile = tilemap->sprites[WALL_TEXT];
                    SDL_RenderCopy(renderer, tile, NULL, &dst);
                    break;
                case BOX_ID:
                    tile = tilemap->sprites[BOX_TEXT];
                    SDL_RenderCopy(renderer, tile, NULL, &dst);
                    break;
                case TARGET_ID:
                    tile = tilemap->sprites[TARGET_TEXT];
                    SDL_RenderCopy(renderer, tile, NULL, &dst);
                    break;
                case BOX_TARGETED_ID:
                    tile = tilemap->sprites[BOX_TARGETED_TEXT];
                    SDL_RenderCopy(renderer, tile, NULL, &dst);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 25, 10, 10, 255);
                    SDL_RenderFillRect(renderer, &dst);
                    break;
            }
        }
    }
}

void tilemap_display_console(Tilemap* tilemap)
{
    for(int i = 0; i < NUMBER_OF_TILES_VERTICAL; i++)
    {
        for(int j = 0; j < NUMBER_OF_TILES_HORIZONTAL; j++)
        {
            int k = (i * NUMBER_OF_TILES_HORIZONTAL) + j;
            printf("%c", tilemap->level[k]);
        }
        printf("\n");
    }
}

Uint8 tilemap_count_id(Tilemap* tilemap, TilemapID id)
{
    Uint8 cnt = 0;

    for(int i = 0; i < NUMBER_OF_TILES_HORIZONTAL*NUMBER_OF_TILES_VERTICAL; i++)
    {
        if(tilemap->level[i] == id)
            cnt++;
    }

    return cnt;
}

SDL_bool tilemap_completed(Tilemap* tilemap)
{
    if(tilemap_count_id(tilemap, TARGET_ID) == 0)
        return SDL_TRUE;

    return SDL_FALSE;
}

Uint32 tilemap_number_of_levels(Tilemap* tilemap)
{
    FILE* file = fopen(tilemap->path, "r");
    int i = 0;

    if(file != NULL)
    {
        char str[255];
        while(!feof(file))
        {
            i++;
            fgets(str, 255, file);
        }
        fclose(file);
    }

    return i+1;
}

void tilemap_load_level(Tilemap* tilemap, Uint32 levelID)
{
    FILE* file = fopen(tilemap->path, "r");
    int i = 0;

    if(file != NULL)
    {
        char str[255];
        while(i != levelID)
        {
            i++;
            fgets(str, 255, file);
        }

        SDL_memcpy(tilemap->level, str, NUMBER_OF_TILES_VERTICAL*NUMBER_OF_TILES_HORIZONTAL);

        fclose(file);
    }
}

