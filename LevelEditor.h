//
// Created by Megaport on 20/05/2021.
//

#ifndef MARIOSOKOBAN_LEVELEDITOR_H
#define MARIOSOKOBAN_LEVELEDITOR_H

#include <SDL2/SDL.h>
#include "Tilemap.h"
#include "State.h"

typedef struct
{
    int targetTile;
    TilemapID id;
    char* level;
}LevelEditor;

/*
 * void -> LevelEditor*
 *
 * Permet la création d'une structure LevelEditor qui contiendra
 * les données sous forme de tableau des informations des tuiles
 * */
LevelEditor* leveleditor_create();
/*
 * LevelEditor* -> void
 *
 *  Surpprime, le paramètre passez argument de la mémoire
 * */
void leveleditor_delete(LevelEditor* editor);
/*
 * LevelEditor* -> void
 *
 * Mets a jour l'editeur de niveau,
 * - Clique droite pour ajouter une tuile
 * - Clique gauche pour supprimer une tuile
 * - 'S' pour sauvegarder le niveau
 * - Revenir au menu principal en appuyant sur Echap
 * */
State leveleditor_update(LevelEditor* editor);
/*
 * LevelEditor* -> SDL_bool
 *
 * Enregistre le niveau dans un fichier
 * */
SDL_bool leveleditor_save(LevelEditor* editor);
/*
 *  LevelEditor*, Tilemap* -> void
 *
 *  Dessine le niveau en cours de construction
 * */
void leveleditor_draw(LevelEditor* editor, Tilemap* tilemap, SDL_Renderer* renderer);
/*
 *  LevelEditor* -> SDL_bool
 *
 *  Regarde, si dans les tuiles le constructeur de niveau a placer le point de départ de mario
 * */
SDL_bool leveleditor_mario_start_present(LevelEditor* editor);

#endif //MARIOSOKOBAN_LEVELEDITOR_H
