//
// Created by Megaport on 20/05/2021.
//

#include "LevelEditor.h"
#include "Constants.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

LevelEditor* leveleditor_create()
{
    LevelEditor* editor = SDL_malloc(sizeof(*editor));

    if(editor == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "malloc: %s", SDL_GetError());
        return NULL;
    }

    editor->level = SDL_malloc(sizeof(char) * NUMBER_OF_TILES_HORIZONTAL * NUMBER_OF_TILES_VERTICAL);

    if(editor->level == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "malloc: %s", SDL_GetError());
        SDL_free(editor);
        return NULL;
    }

    SDL_memset(editor->level, '0', NUMBER_OF_TILES_VERTICAL*NUMBER_OF_TILES_HORIZONTAL);
    editor->targetTile = EMPTY_ID;

    return editor;
}

void leveleditor_delete(LevelEditor* editor)
{
    SDL_free(editor->level);
    SDL_free(editor);
}

void leveleditor_draw(LevelEditor* editor, Tilemap* tilemap, SDL_Renderer* renderer)
{
    SDL_Texture* tile = NULL;
    SDL_Rect dst;
    dst.w = BLOCKSIZE;
    dst.h = BLOCKSIZE;

    for(int i = 0; i < NUMBER_OF_TILES_VERTICAL; i++)
    {
        for(int j = 0; j < NUMBER_OF_TILES_HORIZONTAL; j++)
        {
            int index = (i * NUMBER_OF_TILES_HORIZONTAL) + j;
            char id = editor->level[index];

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
                case START_ID:
                    tile = tilemap->sprites[START_TEXT];
                    SDL_RenderCopy(renderer, tile, NULL, &dst);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 25, 10, 10, 255);
                    SDL_RenderFillRect(renderer, &dst);
                    break;
            }

            if(index == editor->targetTile)
            {
                switch(editor->id)
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
                    case START_ID:
                        tile = tilemap->sprites[START_TEXT];
                        SDL_RenderCopy(renderer, tile, NULL, &dst);
                        break;
                    default:
                        SDL_SetRenderDrawColor(renderer, 25, 10, 10, 255);
                        SDL_RenderFillRect(renderer, &dst);
                        break;
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 150, 255);
                SDL_RenderDrawRect(renderer, &dst);
            }
        }
    }

}

State leveleditor_update(LevelEditor* editor)
{
    int x, y;
    int i, j;

    SDL_PumpEvents();
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    Uint32 mouse = SDL_GetMouseState(&x, &y);

    j = x / BLOCKSIZE;
    i = y / BLOCKSIZE;
    editor->targetTile = (i * NUMBER_OF_TILES_HORIZONTAL) + j;

    if(keyboard[SDL_SCANCODE_ESCAPE])
        return MAIN_MENU;

    if(keyboard[SDL_SCANCODE_1])
        editor->id = WALL_ID;
    if(keyboard[SDL_SCANCODE_2])
        editor->id = BOX_ID;
    if(keyboard[SDL_SCANCODE_3])
        editor->id = BOX_TARGETED_ID;
    if(keyboard[SDL_SCANCODE_4])
        editor->id = TARGET_ID;
    if(keyboard[SDL_SCANCODE_5])
        editor->id = START_ID;

    if(mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        editor->level[editor->targetTile] = editor->id;
    }

    if(mouse & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        editor->level[editor->targetTile] = EMPTY_ID;
    }

    if(keyboard[SDL_SCANCODE_S])
    {
        if(leveleditor_mario_start_present(editor))
        {
            if(leveleditor_save(editor))
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Enregistrement", "Votre niveau a était enregistrer avec succès", NULL);
            else
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Enregistrement", "Impossible d'enregistrer le niveau", NULL);
        }
        else
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Enregistrement", "L'édition de niveau n'est pas terminer ! Veuillez le terminer avant enregistrement !", NULL);
        }
    }

    return IN_EDITOR;
}

SDL_bool leveleditor_save(LevelEditor* editor)
{
    FILE* file = NULL;
    int i = 0;

    file = fopen("./niveaux/levels.txt", "a+");

    if(file  == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "fopen: %s", strerror(errno));
        errno = 0;
        return SDL_FALSE;
    }

    fputc('\n', file);
    for(i = 0; i < NUMBER_OF_TILES_HORIZONTAL*NUMBER_OF_TILES_VERTICAL; i++)
        fputc(editor->level[i], file);

    fclose(file);
    return SDL_TRUE;
}

SDL_bool leveleditor_mario_start_present(LevelEditor* editor)
{
    for(int i = 0; i < NUMBER_OF_TILES_HORIZONTAL*NUMBER_OF_TILES_VERTICAL; i++)
    {
        if(editor->level[i] == START_ID)
            return SDL_TRUE;
    }
    return SDL_FALSE;
}