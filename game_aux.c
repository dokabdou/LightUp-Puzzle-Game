#include "game_aux.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"

void game_print(cgame g) {
  uint i, j; /*i is vertical an j is horizontal*/
  printf("   ");
  for (int s = 0; s < game_nb_cols(g); s++) {
    printf("%d", s);
  }
  printf("\n");

  printf("   ");

  for (int t = 0; t < game_nb_cols(g); t++) {
    printf("-");
  }
  printf("\n");

  for (i = 0; i < game_nb_rows(g); i++) {
    printf("%u |", i);
    for (j = 0; j < game_nb_cols(g); j++) {
      if (game_is_blank(g, i, j)) {
        if (game_is_lighted(g, i, j)) {
          printf(".");  // Empty lighted square
        } else {
          printf(" ");  // Empty square
        }
      } else if (game_is_black(g, i, j)) {
        uint n = game_get_black_number(g, i, j);
        if (n == -1) {
          printf("w");  // Black square
        } else {
          printf("%u", n);  // Black numbered square
        }
      } else if (game_is_lightbulb(g, i, j)) {
        printf("*");  // Lightbulb
      } else if (game_is_marked(g, i, j)) {
        printf("-");  // Mark
      }
    }
    printf("|\n");
  }
  printf("   ");
  for (int t = 0; t < game_nb_cols(g); t++) {
    printf("-");
  }
  printf("\n");
}

game game_default(void) {
  /*
  Creating an empty game, then filling the squares that need to be filled, based
  on the base example
  */
  game default_game = game_new_empty();
  game_set_square(default_game, 0, 2, S_BLACK1);
  game_set_square(default_game, 1, 2, S_BLACK2);
  game_set_square(default_game, 2, 5, S_BLACKU);
  game_set_square(default_game, 2, 6, S_BLACK2);
  game_set_square(default_game, 4, 0, S_BLACK1);
  game_set_square(default_game, 4, 1, S_BLACKU);
  game_set_square(default_game, 5, 4, S_BLACK2);
  game_set_square(default_game, 6, 4, S_BLACKU);
  return default_game;
}

game game_default_solution(void) {
  game default_solution = game_default();  // Taking the default game
  game_set_square(default_solution, 0, 0, S_LIGHTBULB);
  game_set_square(default_solution, 0, 3, S_LIGHTBULB);
  game_set_square(default_solution, 1, 1, S_LIGHTBULB);
  game_set_square(default_solution, 1, 6, S_LIGHTBULB);
  game_set_square(default_solution, 2, 2, S_LIGHTBULB);  // Placing a lightbulb
  game_set_square(default_solution, 3, 6, S_LIGHTBULB);
  game_set_square(default_solution, 4, 4, S_LIGHTBULB);
  game_set_square(default_solution, 5, 0, S_LIGHTBULB);
  game_set_square(default_solution, 5, 5, S_LIGHTBULB);
  game_set_square(default_solution, 6, 1, S_LIGHTBULB);
  game_update_flags(default_solution);  // Updated to active F_LIGHTED flags
  return default_solution;
}