#include <SDL/SDL.h>
#include "functions.h"

#define PATH_TILESET "data\\tileset.tst"
#define PATH_MENU "data\\menu.bmp"

int main(int ac, char **av) {
    struct context_t context;
    if(init_context(&context, PATH_TILESET) == FAIL) {
        fputs("Unable to load context", stderr);
        return EXIT_FAILURE;
    }

    menu(&context, PATH_MENU);

    free_context(&context);
    return EXIT_SUCCESS;
    (void)ac;
    (void)av;
}
