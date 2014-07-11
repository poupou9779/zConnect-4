#ifndef CONTEXT_H
#define CONTEXT_H

/*no need to include SDL.h which is included by tileset.h*/
#include "tileset.h"

#define NB_CELLS_H 6
#define NB_CELLS_W 7

enum cell_e {EMPTY = 0, PLAYER_1 = 1, PLAYER_2 = 2};

struct context_t {
    struct tileset_t tileset;
    SDL_Surface *screen;
    int nb_free_cells,
        ply;
    enum cell_e grid[NB_CELLS_H][NB_CELLS_W];
    /*
        (0;0) (1;0) (2;0) (3;0) (4;0) (5;0) (6;0)
        (0;1) (1;1) (2;1) (3;1) (4;1) (5;1) (6;1)
        (0;2) (1;2) (2;2) (3;2) (4;2) (5;2) (6;2)
        (0;3) (1;3) (2;3) (3;3) (4;3) (5;3) (6;3)
        (0;4) (1;4) (2;4) (3;4) (4;4) (5;4) (6;4)
        (0;5) (1;5) (2;5) (3;5) (4;5) (5;5) (6;5)
    */
};

int init_context(struct context_t *context, const char *path_tileset);

void free_context(struct context_t *context);

#endif
