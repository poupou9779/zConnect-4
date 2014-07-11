#include "AI.h"

/**
        WARNING ! AI HAS TO BE FIXED !
        AI DOESN'T WORK WELL ENOUGH : IT HAS TO BE BUGGED, IT ISN'T POSSIBLE OTHERWISE;
        EVEN WITH 4 DEPTH-LEVEL, AI STILL LOOSES.
        GOTTA BE CHECKED.
**/

void turn_AI(struct context_t *context, int player) {
    int max_value = -INFINITE,
        value,
        i,
        best_column;
    for(i = 0; i < NB_CELLS_W; ++i) {
        if(push_on_column(context, player, i)) {
            value = min(context, (PLAYER_1+PLAYER_2)-player, context->ply);
            if(value > max_value || (value == max_value && rand()%100 < PERCENTAGE_CHOICE)) {
                max_value = value;
                best_column = i;
            }
            pop_on_column(context, i);
        }
    }
    push_on_column(context, player, best_column);
}

int min(struct context_t *context, int player, int ply) {
    int min_value = +INFINITE,
        value,
        i;
    if(ply == 0 || context->nb_free_cells == 0)
        return eval(context, player);
    for(i = 0; i < NB_CELLS_W; ++i) {
        if(push_on_column(context, player, i)) {
            value = max(context, (PLAYER_1+PLAYER_2)-player, ply-1);
            if(value < min_value) {
                min_value = value;
            }
            pop_on_column(context, i);
        }
    }
    return min_value;
}

int max(struct context_t *context, int player, int ply) {
    int max_value = -INFINITE,
        value,
        i;
    if(ply == 0 || context->nb_free_cells == 0)
        return eval(context, player);
    for(i = 0; i < NB_CELLS_W; ++i) {
        if(push_on_column(context, player, i)) {
            value = min(context, (PLAYER_1+PLAYER_2)-player, ply-1);
            if(value > max_value) {
                max_value = value;
            }
            pop_on_column(context, i);
        }
    }
    return max_value;
}

int eval(struct context_t *context, int player) {
    int winner;
    if((winner = isover(context)) > 0) {
        if(winner == player)
            return 1000 + context->nb_free_cells - NB_CELLS_H*NB_CELLS_W;
        else
            return -(1000 + context->nb_free_cells - NB_CELLS_H*NB_CELLS_W);
    }
    if(context->nb_free_cells == 0)
        return 0;
    else
        return 2;
}

