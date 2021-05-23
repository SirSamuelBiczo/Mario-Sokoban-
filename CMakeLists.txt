cmake_minimum_required(VERSION 3.17)
project(MarioSokoban C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin) # Change le répertoire ou CMAKE génère l'executable
include_directories(${CMAKE_SOURCE_DIR}/include) # Ajoute le fichier include en static
link_directories(${CMAKE_SOURCE_DIR}/lib) # Ajoute le repertoire lib

add_executable(MarioSokoban main.c Constants.h core.h core.c State.h Mario.h Mario.c Tilemap.h Tilemap.c LevelEditor.h LevelEditor.c Menu.c Menu.h)
target_link_libraries(${PROJECT_NAME} -lSDL2main -lSDL2_image -lSDL2) # linkage des libs