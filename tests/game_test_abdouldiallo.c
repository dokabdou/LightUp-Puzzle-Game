#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

#define ASSERT(expr)                                                          \
  do {                                                                        \
    if ((expr) == 0) {                                                        \
      fprintf(stderr, "[%s:%d] ASSERTion '%s' failed!\n", __FILE__, __LINE__, \
              #expr);                                                         \
      abort();                                                                \
    }                                                                         \
  } while (0)

/*Test Dummy*/
bool test_dummy() { return true; }

/* *****Test de Game Solution***** */
bool test_game_solution() {
  game game_p = game_default_solution();
  game default_solution = game_default();  // Reprise du modèle de base
  ASSERT(game_p);
  ASSERT(default_solution);

  game_set_square(default_solution, 0, 0, S_LIGHTBULB);
  game_set_square(default_solution, 0, 3, S_LIGHTBULB);
  game_set_square(default_solution, 1, 1, S_LIGHTBULB);
  game_set_square(default_solution, 1, 6, S_LIGHTBULB);
  game_set_square(default_solution, 2, 2, S_LIGHTBULB);  // Placement des lampes
  game_set_square(default_solution, 3, 6, S_LIGHTBULB);
  game_set_square(default_solution, 4, 4, S_LIGHTBULB);
  game_set_square(default_solution, 5, 0, S_LIGHTBULB);
  game_set_square(default_solution, 5, 5, S_LIGHTBULB);
  game_set_square(default_solution, 6, 1, S_LIGHTBULB);
  game_update_flags(default_solution);

  ASSERT(game_is_over(game_p));
  ASSERT(game_is_over(default_solution));
  ASSERT(game_equal(game_p, default_solution));

  for (int i = 0; i < DEFAULT_SIZE; i++)  // Double loop to look
  {
    for (int j = 0; j < DEFAULT_SIZE; j++)  // throught all the grid
    {
      fprintf(stderr, "%d %d %d %d %d %d\n", i, j, game_get_state(game_p, i, j),
              game_get_state(default_solution, i, j),
              game_get_flags(game_p, i, j),
              game_get_flags(default_solution, i, j));
      ASSERT(game_has_error(game_p, i, j) == false);

      if ((i == 0 && j == 2) || (i == 4 && j == 0)) {
        ASSERT(game_is_lighted(game_p, i, j) == false);
        ASSERT(game_get_state(game_p, i, j) == S_BLACK1);
      } else if ((i == 1 && j == 2) || (i == 2 && j == 6) ||
                 (i == 5 && j == 4)) {
        ASSERT(game_is_lighted(game_p, i, j) == false);
        ASSERT(game_get_state(game_p, i, j) == S_BLACK2);
      } else if ((i == 2 && j == 5) || (i == 4 && j == 1) ||
                 (i == 6 && j == 4)) {
        ASSERT(game_is_lighted(game_p, i, j) == false);
        ASSERT(game_get_state(game_p, i, j) == S_BLACKU);
      } else if ((i == 0 && j == 0) || (i == 0 && j == 3) ||
                 (i == 1 && j == 1) || (i == 1 && j == 6) ||
                 (i == 2 && j == 2) || (i == 3 && j == 6) ||
                 (i == 4 && j == 4) || (i == 5 && j == 0) ||
                 (i == 5 && j == 5) || (i == 6 && j == 1)) {
        ASSERT(game_is_lightbulb(game_p, i, j));
        ASSERT(game_is_lighted(game_p, i, j));
      } else {
        ASSERT(game_is_blank(game_p, i, j));
        ASSERT(game_is_lighted(game_p, i, j));
      }
    }
  }

  game_delete(game_p);
  game_delete(default_solution);
  return true;
}

/* *****Test de get state|square game_get_state(cgame g, uint i, uint j);*****
 */
bool test_get_state(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_BLACK0);
  ASSERT(game_get_state(gamep, i, j) == S_BLACK0);
  game_delete(gamep);
  return true;
}

/* *****Test de get flags|square game_get_flags(cgame g, uint i, uint j);*****
 */
bool test_get_flags(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, F_LIGHTED);
  ASSERT(game_get_flags(gamep, i, j) == F_LIGHTED);

  game_set_square(gamep, i, j, F_ERROR);
  ASSERT(game_get_flags(gamep, i, j) == F_ERROR);

  game_delete(gamep);
  return true;
}

/* *****Test de is blank|bool game_is_blank(cgame g, uint i, uint j);***** */
bool test_is_blank(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_BLANK);
  ASSERT(game_is_blank(gamep, i, j) == true);

  game_set_square(gamep, i, j, S_BLACK0);
  ASSERT(game_is_blank(gamep, i, j) == false);

  game_delete(gamep);
  return true;
}

/* *****Test de is lightbulb|bool game_is_lightbulb(cgame g, uint i, uint
 * j);***** */
bool test_is_lightbulb(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_LIGHTBULB);
  ASSERT(game_is_lightbulb(gamep, i, j) == true);

  game_set_square(gamep, i, j, S_BLACK0);
  ASSERT(game_is_lightbulb(gamep, i, j) == false);

  game_delete(gamep);
  return true;
}

/* *****Test de is black|bool game_is_black(cgame g, uint i, uint j);***** */
bool test_is_black(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_BLACK0);
  ASSERT(game_is_black(gamep, i, j) == true);

  game_set_square(gamep, i, j, S_MARK);
  ASSERT(game_is_black(gamep, i, j) == false);

  game_delete(gamep);
  return true;
}

/* *****Test de get black number|int game_get_black_number(cgame g, uint i, uint
 * j);***** */
bool test_get_black_number(uint i, int j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_BLACK2);
  ASSERT(game_get_black_number(gamep, i, j) == 0 || 1 || 2 || 3 || 4);

  game_set_square(gamep, i, j, S_BLACKU);
  ASSERT(game_get_black_number(gamep, i, j) == -1);

  game_delete(gamep);
  return true;
}

/* *****Test de is marked|bool game_is_marked(cgame g, uint i, uint j);***** */
bool test_is_marked(uint i, uint j) {
  game gamep = game_default();
  ASSERT(gamep);

  if (i > DEFAULT_SIZE || j > DEFAULT_SIZE) {
    game_delete(gamep);
    return false;
  }

  game_set_square(gamep, i, j, S_MARK);
  ASSERT(game_is_marked(gamep, i, j) == true);

  game_set_square(gamep, i, j, S_BLACK0);
  ASSERT(game_is_marked(gamep, i, j) == false);

  game_delete(gamep);
  return true;
}

/* *****Test de void game_undo();***** */
bool test_game_undo() {
  game g1 = game_default();
  ASSERT(g1);
  game g2 = game_default();
  ASSERT(g2);
  game_undo(g1);
  ASSERT(game_equal(g1, g2));

  game_play_move(g1, 0, 1, S_LIGHTBULB);
  game_play_move(g1, 2, 2, S_LIGHTBULB);
  game_play_move(g2, 0, 1, S_LIGHTBULB);
  game_undo(g1);
  ASSERT(game_equal(g1, g2));
  game_undo(g2);
  ASSERT(!game_equal(g1, g2));
  game_delete(g1);
  game_delete(g2);
  return true;
}

/* *****Test de void game_redo();***** */
bool test_game_redo() {
  game g1 = game_default();
  ASSERT(g1);
  game g2 = game_default();
  ASSERT(g2);
  game_play_move(g1, 0, 1, S_LIGHTBULB);
  game_play_move(g2, 0, 1, S_LIGHTBULB);
  game_redo(g1);
  ASSERT(game_equal(g1, g2));
  game_undo(g2);
  game_redo(g2);
  ASSERT(game_equal(g1, g2));
  game_play_move(g2, 3, 0, S_LIGHTBULB);
  game_undo(g2);
  ASSERT(game_equal(g1, g2));
  game_play_move(g2, 2, 2, S_LIGHTBULB);
  game_play_move(g1, 2, 2, S_LIGHTBULB);
  game_redo(g2);
  ASSERT(game_equal(g1, g2));
  game_delete(g1);
  game_delete(g2);
  return true;
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* *************MAIN************* */

int main(int argc, char *argv[]) {
  if (argc == 1) {
    return EXIT_FAILURE;
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool test = false;

  uint i = 3, j = 4;

  if (strcmp(argv[1], "dummy") == 0) {
    test = test_dummy();
  } else if (strcmp("test_game_solution", argv[1]) == 0) {
    test = test_game_solution();
  } else if (strcmp("test_get_state", argv[1]) == 0) {
    test = test_get_state(i, j);
  } else if (strcmp("test_get_flags", argv[1]) == 0) {
    test = test_get_flags(i, j);
  } else if (strcmp("test_is_blank", argv[1]) == 0) {
    test = test_is_blank(i, j);
  } else if (strcmp("test_is_lightbulb", argv[1]) == 0) {
    test = test_is_lightbulb(i, 6);
  }  //
  else if (strcmp("test_is_black", argv[1]) == 0) {
    test = test_is_black(i, j);
  } else if (strcmp("test_get_black_number", argv[1]) == 0) {
    test = test_get_black_number(i, j);
  } else if (strcmp("test_is_marked", argv[1]) == 0) {
    test = test_is_marked(i, j);
  } else if (strcmp("test_undo", argv[1]) == 0) {
    test = test_game_undo();
  } else if (strcmp("test_redo", argv[1]) == 0) {
    test = test_game_redo();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    return EXIT_FAILURE;
  }

  if (test) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}