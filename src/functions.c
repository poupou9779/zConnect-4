#include "functions.h"
#include "AI.h"

#define PATH_DIFFICULTY "data\\difficulty.bmp"

/*
    1 PLAYER
    2 PLAYERS
    3 LEAVE
*/
void menu(struct context_t *context, const char *path_menu) {
    SDL_Surface *im_menu = SDL_LoadBMP(path_menu);
    SDL_Event e;
    int i, j;
    if(im_menu == NULL) {
        fprintf(stderr, "unable to load %s\n", path_menu);
        return;
    }
    do {
        for(i = 0; i < NB_CELLS_H; ++i)
            for(j = 0; j < NB_CELLS_W; ++j)
                context->grid[i][j] = EMPTY;
        context->nb_free_cells = NB_CELLS_H*NB_CELLS_W;
        SDL_BlitSurface(im_menu, NULL, context->screen, NULL);
        SDL_Flip(context->screen);
        do
            SDL_WaitEvent(&e);
        while(e.type != SDL_KEYDOWN || (e.key.keysym.sym < SDLK_KP1 && e.key.keysym.sym > SDLK_KP3));
        SDL_Delay(500);
        if(e.key.keysym.sym == SDLK_KP1)
            play_AI(context);
        else if(e.key.keysym.sym == SDLK_KP2)
            play_2_players(context);
    } while(e.key.keysym.sym != SDLK_KP3);
}

#define PLAYER 0
#define AI     1
const char *titles_turn_AI[] = {"YOUR TURN, YOU PLAY YELLOW", "AI TURN, IT PLAYS RED"};

int play_AI(struct context_t *context) {
    int winner = 0;
    char title_won[] = "    --    PLAYER X WON ! PRESS ANY KEY TO RETURN TO MAIN MENU    --";
    SDL_Event e;
    purge_events();
    set_initial_ply(context, PATH_DIFFICULTY);
    do {
        blit_all(context);
        SDL_WM_SetCaption(titles_turn_AI[PLAYER], NULL);
        turn(context, PLAYER_1);
        if(isover(context) == 0) {
            blit_all(context);
            SDL_WM_SetCaption(titles_turn_AI[AI], NULL);
            SDL_Delay(500);
            turn_AI(context, PLAYER_2);
        }
        else
            winner = PLAYER_1;
    } while(isover(context) == 0 && context->nb_free_cells > 0);
    if(winner == 0)
        winner = PLAYER_2;
    blit_all(context);
    title_won[17] = winner + '0';
    SDL_WM_SetCaption(title_won, NULL);
    purge_events();
    do
        SDL_WaitEvent(&e);
    while(e.type != SDL_KEYDOWN);
    return winner;
}

const char *titles_turn_2_players[] = {"NONE", "PLAYER 1 PLAYS YELLOW", "PLAYER 2 PLAYS RED"};

int play_2_players(struct context_t *context) {
    SDL_Event e;
    char title_won[] = "    --    PLAYER X WON ! PRESS ANY KEY TO RETURN TO MAIN MENU    --";
    int player = PLAYER_2;
    do {
        blit_all(context);
        player = (PLAYER_1+PLAYER_2) - player;
        SDL_WM_SetCaption(titles_turn_2_players[player], NULL);
        turn(context, player);
    } while(isover(context) == 0 && context->nb_free_cells > 0);

    blit_all(context);
    if(isover(context) > 0)
        title_won[17] = player + '0';
    else
        strcpy(title_won, "NO ONE WON : EX-AEQUO. PRESS ANY KEY TO RETURN TO MAIN MENU");
    SDL_WM_SetCaption(title_won, NULL);
    do
        SDL_WaitEvent(&e);
    while(e.type != SDL_KEYDOWN);
    return player;
}

void turn(struct context_t *context, int player) {
    SDL_Event ev;
    do {
        purge_events();
        do {
            SDL_WaitEvent(&ev);
        } while(ev.type != SDL_MOUSEBUTTONDOWN);
    } while(context->grid[0][ev.button.x/context->tileset.width_tile] != EMPTY);
    push_on_column(context, player, ev.button.x/context->tileset.width_tile);
}

SDL_bool push_on_column(struct context_t *context, int player, int column) {
    int i = 0;
    if(context->grid[i][column] != EMPTY)
        return SDL_FALSE;
    while(context->grid[i+1][column] == EMPTY)
        ++i;
    context->grid[i][column] = player;
    --context->nb_free_cells;
    return SDL_TRUE;
}

void pop_on_column(struct context_t *context, int column) {
    int i = 0;
    while(context->grid[i][column] == EMPTY)
        ++i;
    context->grid[i][column] = EMPTY;
    ++context->nb_free_cells;
}

void purge_events(void) {
    SDL_Event e;
    while(SDL_PollEvent(&e));
}

int isover(struct context_t *context) {
    int i, j;
    /*checks diagonals*/
    for(i = 0; i <= NB_CELLS_W-4; ++i)
        for(j = 0; j <= NB_CELLS_H-4; ++j)
            if(context->grid[j][i] != EMPTY && check_identical(context, i, j, i+3, j+3, 4))
                return context->grid[j][i];
    for(i = NB_CELLS_W-1; i >= 0; --i)
        for(j = 0; j <= NB_CELLS_H-4; ++j)
            if(context->grid[j][i] != EMPTY && check_identical(context, i, j, i-3, j+3, 4))
                return context->grid[j][i];
    /*checks horizontal*/
    for(i = 0; i <= NB_CELLS_W-4; ++i)
        for(j = 0; j <= NB_CELLS_H-1; ++j)
            if(context->grid[j][i] != EMPTY && check_identical(context, i, j, i+3, j, 4))
                return context->grid[j][i];
    /*checks vertical*/
    for(i = 0; i <= NB_CELLS_W-1; ++i)
        for(j = 0; j <= NB_CELLS_H-4; ++j)
            if(context->grid[j][i] != EMPTY && check_identical(context, i, j, i, j+3, 4))
                return context->grid[j][i];
    return 0;
}

SDL_bool check_identical(struct context_t *context, int x_depart, int y_depart, int x_fin, int y_fin, int nb_cells) {
    int i;
    enum cell_e last_occupation = context->grid[y_depart][x_depart];
    for(i = 1; i <= nb_cells; ++i)
        if(last_occupation != context->grid[y_depart+i*(y_fin-y_depart)/nb_cells][x_depart+i*(x_fin-x_depart)/nb_cells])
            return SDL_FALSE;
    return SDL_TRUE;
}

void blit_all(struct context_t *context) {
    int i, j;
    SDL_Rect dst;
    dst.w = context->tileset.width_tile;
    dst.h = context->tileset.height_tile;
    for(i = 0; i < NB_CELLS_H; ++i) {
        dst.y = i*context->tileset.height_tile;
        for(j = 0; j < NB_CELLS_W; ++j) {
            dst.x = j*context->tileset.width_tile;
            SDL_BlitSurface(context->tileset.im, &context->tileset.tab_pos[context->grid[i][j]],
                            context->screen, &dst);
        }
    }
    SDL_Flip(context->screen);
}

void set_initial_ply(struct context_t *context, const char *path_difficulty_image) {
    static const int ret[] = {1, 5, 9};
    SDL_Event e;
    SDL_Surface *difficulty = SDL_LoadBMP(path_difficulty_image);
    SDL_BlitSurface(difficulty, NULL, context->screen, NULL);
    SDL_Flip(context->screen);

    do
        SDL_WaitEvent(&e);
    while(e.type != SDL_KEYDOWN || (e.key.keysym.sym < SDLK_KP1 && e.key.keysym.sym > SDLK_KP3));
    context->ply = ret[e.key.keysym.sym-SDLK_KP1];
}

