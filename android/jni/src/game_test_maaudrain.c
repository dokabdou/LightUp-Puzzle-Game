#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

#define ASSERT(expr)                                                          \
  do {                                                                        \
    if ((expr) == 0) {                                                        \
      fprintf(stderr, "[%s:%d] Assertion '%s' failed!\n", __FILE__, __LINE__, \
              #expr);                                                         \
      abort();                                                                \
    }                                                                         \
  } while (0)

/* ********** TEST DUMMY ********** */
bool test_dummy() { return true; }

/* ********** TEST GAME_NEW ********** */
bool test_game_new() {
  square* squares =
      (square*)malloc(DEFAULT_SIZE * DEFAULT_SIZE * (sizeof(square)));
  ASSERT(squares);
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    squares[i] = S_BLANK;
  }
  game g = game_new(squares);
  ASSERT(g);
  for (int j = 0; j < DEFAULT_SIZE; j++) {
    for (int i = 0; i < DEFAULT_SIZE; i++) {
      ASSERT(game_get_state(g, i, j) == S_BLANK);
    }
  }
  free(squares);
  game_delete(g);
  return true;
}

/* ********** TEST GAME_NEW_EMPTY ********** */
bool test_game_new_empty() {
  game g = game_new_empty();
  ASSERT(g);
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      ASSERT(game_get_square(g, i, j) == S_BLANK);
    }
  }
  game_delete(g);
  return true;
}

/* ********** TEST GAME_COPY ********** */
bool test_game_copy() {
  game g1 = game_default();
  ASSERT(g1);
  game g2 = game_copy(g1);
  ASSERT(g2);
  ASSERT(game_equal(g1, g2));
  game_delete(g1);
  game_delete(g2);
  return true;
}
// AAA
/* ********** TEST GAME_EQUAL ********** */
bool test_game_equal() {
  game g1 = game_default();
  ASSERT(g1);
  game g2 = game_default();
  ASSERT(g2);
  ASSERT(game_equal(g1, g2));
  game_set_square(g2, 1, 1, S_LIGHTBULB);
  ASSERT(!game_equal(g1, g2));
  game g3 = game_default();
  ASSERT(g3);
  ASSERT(game_equal(g1, g3));
  game_set_square(g3, 1, 1, S_LIGHTBULB);
  game_update_flags(g3);
  ASSERT(!game_equal(g1, g3));
  ASSERT(!game_equal(g2, g3));
  game_set_square(g2, 0, 1, S_LIGHTBULB);
  game_set_square(g3, 0, 1, S_LIGHTBULB);
  game_update_flags(g2);
  game_update_flags(g3);
  ASSERT(game_equal(g2, g3));
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  return true;
}

/* ********** TEST GAME_DELETE ********** */
bool test_game_delete() {
  game g = game_new_empty();
  ASSERT(g);
  game_delete(g);
  ASSERT(g);
  return true;
}

/* ********** TEST GAME_SET_SQUARE ********** */
bool test_game_set_square(int i, int j) {
  game g = game_new_empty();
  ASSERT(g);
  game_set_square(g, i, j, S_BLACK0);
  ASSERT(game_get_square(g, i, j) == S_BLACK0);
  game_set_square(g, i, j, S_BLANK);
  game g2 = game_new_empty();
  ASSERT(game_equal(g, g2));
  game_delete(g);
  game_delete(g2);
  return true;
}

/* ********** TEST GAME_GET_SQUARE ********** */
bool test_game_get_square(int i, int j) {
  game g = game_new_empty();
  ASSERT(g);
  ASSERT(game_get_square(g, i, j) == S_BLANK);
  game_set_square(g, j, i, S_BLACK0);
  ASSERT(game_get_square(g, j, i) == S_BLACK0);
  game_delete(g);
  return true;
}

/* ********** TEST GAME_PRINT ********** */
bool test_game_print(int i, int j) {
  game g = game_new_empty();
  ASSERT(g);
  game_set_square(g, i, j, S_BLACK0);
  game_print(g);
  game_delete(g);
  return true;
}

/* ********** TEST GAME_NEW_EXT ********** */
bool test_game_new_ext(uint height, uint width, bool wrapping) {
  square* squares = (square*)malloc(height * width * (sizeof(square)));
  ASSERT(squares);
  for (int i = 0; i < height * width; i++) {
    squares[i] = S_BLACK0;
  }
  game g = game_new_ext(height, width, squares, wrapping);
  ASSERT(g);
  for (int j = 0; j < width; j++) {
    for (int i = 0; i < height; i++) {
      ASSERT(game_get_state(g, i, j) == S_BLACK0);
    }
  }
  ASSERT(game_nb_rows(g) == height);
  ASSERT(game_nb_cols(g) == width);
  ASSERT(game_is_wrapping(g) == wrapping);
  free(squares);
  game_delete(g);
  return true;
}

/* ********** TEST GAME_NEW_EMPTY_EXT ********** */
bool test_game_new_empty_ext(uint height, uint width, bool wrapping) {
  game g = game_new_empty_ext(height, width, wrapping);
  ASSERT(g);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      ASSERT(game_get_square(g, i, j) == S_BLANK);
    }
  }
  ASSERT(game_nb_rows(g) == height);
  ASSERT(game_nb_cols(g) == width);
  ASSERT(game_is_wrapping(g) == wrapping);
  game_delete(g);
  return true;
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char* argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    return EXIT_FAILURE;
  }
  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("game_new", argv[1]) == 0)
    ok = test_game_new();
  else if (strcmp("game_new_empty", argv[1]) == 0)
    ok = test_game_new_empty();
  else if (strcmp("game_copy", argv[1]) == 0)
    ok = test_game_copy();
  else if (strcmp("game_equal", argv[1]) == 0)
    ok = test_game_equal();
  else if (strcmp("game_delete", argv[1]) == 0)
    ok = test_game_delete();
  else if (strcmp("game_set_square", argv[1]) == 0)
    ok = test_game_set_square(2, 3);
  else if (strcmp("game_get_square", argv[1]) == 0)
    ok = test_game_get_square(2, 3);
  else if (strcmp("game_print", argv[1]) == 0)
    ok = test_game_print(2, 3);
  else if (strcmp("game_new_ext", argv[1]) == 0)
    ok = test_game_new_ext(5, 10, true);
  else if (strcmp("game_new_empty_ext", argv[1]) == 0)
    ok = test_game_new_empty_ext(5, 10, true);
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    return EXIT_FAILURE;
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
