#ifndef AI_H
#define AI_H

#include "functions.h"

#define INFINITE ((1) << (16))

#define PERCENTAGE_CHOICE 25
/*means that even if a new brach is as well evaluated than a previous one,
this branch has 25% chances to be selected as optimal*/

void turn_AI(struct context_t *context, int player);

int min(struct context_t *context, int player, int ply);

int max(struct context_t *context, int player, int ply);

int eval(struct context_t *context, int player);

#endif

