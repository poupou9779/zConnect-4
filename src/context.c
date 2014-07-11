#include "context.h"

int init_context(struct context_t *context, const char *path_tileset) {
    if(load_tileset(&context->tileset, path_tileset) == FAIL)
        return FAIL;
    if((context->screen = SDL_SetVideoMode(NB_CELLS_W*context->tileset.width_tile,
                                           NB_CELLS_H*context->tileset.height_tile,
                                           32,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF)) == NULL) {
        free_tileset(&context->tileset);
        return FAIL;
    }
    return OK;
}

void free_context(struct context_t *context) {
    free_tileset(&context->tileset);
    SDL_FreeSurface(context->screen);
}
