#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/*----------------OPTIMIZE SOLVER---------------------*/

void g_solve_rec(game g /*, uint *count*/, int s_rows, int s_cols) {
  /*
      If lighted or wall skip
  */
  // for(int ; ;){
  if (game_is_over(g)) {
    return;
  }

  if (s_cols >= game_nb_cols(g) && s_rows >= game_nb_rows(g)) {
    return;
  }
  if (s_rows >= game_nb_rows(g)) return;

  if (s_cols >= game_nb_cols(g)) {
    return g_solve_rec(g /*, count*/, s_rows + 1, 0);
  }

  if (game_is_lighted(g, s_rows, s_cols)) {
    g_solve_rec(g /*, count*/, s_rows, s_cols + 1);
  } else if (game_is_black(g, s_rows, s_cols)) {
    g_solve_rec(g /*, count*/, s_rows, s_cols + 1);
  } else {
    game_play_move(g, s_rows, s_cols, S_LIGHTBULB);
    if (game_has_error(g, s_rows, s_cols)) {
      game_play_move(g, s_rows, s_cols, S_BLANK);
      g_solve_rec(g /*, count*/, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
    } else {
      g_solve_rec(g /*, count*/, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
      game_play_move(g, s_rows, s_cols, S_BLANK);
      g_solve_rec(g /*, count*/, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
    }
    game_print(g);
  }
}

bool g_s(game g) {
  game gbis = game_copy(g);
  uint c = 0;
  /*game gsbis =*/g_solve_rec(gbis /*, &c*/, 0, 0);
  if (game_is_over(gbis /*gsbis*/)) {
    for (int i = 0; i < game_nb_rows(g); i++)
      for (int j = 0; j < game_nb_cols(g); j++)
        game_set_square(g, i, j, game_get_square(gbis, i, j));
    return true;
  } else {
    return false;
  }
}
