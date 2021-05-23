//
// Created by Megaport on 16/05/2021.
//

#ifndef MARIOSOKOBAN_CORE_H
#define MARIOSOKOBAN_CORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Mario.h"
#include "Constants.h"
#include "LevelEditor.h"
#include "Menu.h"

/*
 * SDL_Window**, SDL_Renderer** -> SDL_bool
 *
 * Fonction qui initialize les besoins fondamentale pour l'affichage graphique
 * retourne SDL_TRUE si l'initialisation c'est bien passé, SDL_FALSE sinon
 * */
SDL_bool initialize_sdl(SDL_Window** pp_Window, SDL_Renderer** pp_Renderer);
/*
 * const char* -> SDL_Texture*
 *
 * Charge une texture qui contient une image
 * */
SDL_Texture* img_load_texture(SDL_Renderer* renderer, const char* path);
/*
 * SDL_Window**, SDL_Renderer** -> SDL_bool
 *
 * Fonction qui libère les ressources alloué
 * */
void free_sdl(SDL_Window* window, SDL_Renderer* renderer, Mario* mario, Tilemap* tilemap, LevelEditor* editor, Menu* menu);

#endif //MARIOSOKOBAN_CORE_H
