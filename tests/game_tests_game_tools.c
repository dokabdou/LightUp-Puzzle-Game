#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

#define ASSERT(expr)                                                          \
  do {                                                                        \
    if ((expr) == 0) {                                                        \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, \
              #expr);                                                         \
      abort();                                                                \
    }                                                                         \
  } while (0)

/* ********** TEST GAME_LOAD ********** */

bool test_game_load() {
  game g = game_default();
  ASSERT(g);

  game_play_move(g, 0, 0, S_LIGHTBULB);  // play a few moves
  game_play_move(g, 1, 1, S_LIGHTBULB);

  game_save(g, "g_save");
  game g1 = game_load("g_save");

  ASSERT(game_equal(g, g1));

  game_delete(g);
  game_delete(g1);
  return true;
}

/* ********** TEST GAME_SAVE ********** */

bool test_game_save() {
  game g = game_default();
  ASSERT(g);

  game_play_move(g, 0, 0, S_LIGHTBULB);  // play a few moves
  game_play_move(g, 1, 1, S_LIGHTBULB);

  game_save(g, "g_save");
  game g1 = game_load("g_save");

  ASSERT(game_equal(g, g1));

  game_play_move(g, 2, 1, S_LIGHTBULB);
  ASSERT(!game_equal(g, g1));

  game_delete(g);
  game_delete(g1);
  return true;
}

bool test_game_solve() {
  game g = game_default();
  ASSERT(g);

  ASSERT(game_solve(g));

  game g2 = game_new_empty_ext(5, 5, false);
  ASSERT(game_solve(g2));

  game_delete(g);
  game_delete(g2);
  return true;
}

bool test_game_nb_sols() {
  game g = game_default();
  ASSERT(g);

  ASSERT(game_nb_solutions(g) == 1);

  game extp = game_new_empty_ext(3, 3, false);  // example of td
  game_set_square(extp, 1, 1, S_BLACK2);
  ASSERT(game_nb_solutions(extp) == 4);

  game_delete(g);
  game_delete(extp);
  return true;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    return EXIT_FAILURE;
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool test = false;

  if (strcmp("game_load", argv[1]) == 0) {
    test = test_game_load();
  } else if (strcmp("game_save", argv[1]) == 0) {
    test = test_game_save();
  } else if (strcmp("game_solve", argv[1]) == 0) {
    test = test_game_solve();
  } else if (strcmp("game_nb_sols", argv[1]) == 0) {
    test = test_game_nb_sols();
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
