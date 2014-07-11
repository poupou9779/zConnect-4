#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "context.h"

void menu(struct context_t *context, const char *path_menu);

int play_AI(struct context_t *context);

int play_2_players(struct context_t *context);

void turn(struct context_t *context, int player);

void purge_events(void);

int isover(struct context_t *context);

SDL_bool check_identical(struct context_t *context, int x_depart, int y_depart, int x_fin, int y_fin, int nb_cells);

void blit_all(struct context_t *context);

SDL_bool push_on_column(struct context_t *context, int player, int column);

void pop_on_column(struct context_t *context, int column);

void set_initial_ply(struct context_t *context, const char *path_difficulty_image);

#endif
