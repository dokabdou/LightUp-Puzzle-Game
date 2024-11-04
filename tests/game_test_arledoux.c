#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

/*----------------------USAGE--------------------------------*/

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
}

/*---------------------DUMMY--------------------------------*/

bool test_dummy(char *argument) { return true; }

/*---------------------GAME-ROWS--------------------------------*/

uint test_game_nb_rows() {
  game g = game_new_empty_ext(5, 4, false);
  assert(game_nb_rows(g) == 5);
  game_delete(g);

  return true;
}

/*---------------------GAME-COLUMNS--------------------------------*/

uint test_game_nb_cols() {
  game g = game_new_empty_ext(5, 4, false);
  assert(game_nb_cols(g) == 4);
  game_delete(g);

  return true;
}

/*---------------------GAME-WRAPPING--------------------------------*/

uint test_game_is_wrapping() {
  game g = game_new_empty_ext(5, 4, false);
  assert(game_is_wrapping(g) == false);
  game_delete(g);

  game g2 = game_new_empty_ext(4, 5, true);
  assert(game_is_wrapping(g2) == true);
  game_delete(g2);

  return true;
}

/*---------------------GAME-LIGHTED--------------------------------*/

bool test_game_is_lighted() {
  game g = game_default();

  assert(g != NULL);

  game_set_square(g, 0, 0, S_LIGHTBULB);
  game_update_flags(g);

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((i == 0 && j == 0) || (i == 0 && j == 1) || (i == 1 && j == 0) ||
          (i == 2 && j == 0) || (i == 3 && j == 0)) {
        fprintf(stderr, "%d %d", i, j);
        assert(game_is_lighted(g, i, j));
      }

      else {
        assert(game_is_lighted(g, i, j) == false);
      }
    }
  }

  game_delete(g);
  return true;
}

/*---------------------GAME-ERROR--------------------------------*/

bool test_game_has_error() {
  game g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 0, 2, S_BLACK0);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2) == false);

  game_set_square(g, 0, 1, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2));

  game_set_square(g, 0, 2, S_BLACK1);
  game_set_square(g, 0, 1, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2) == false);

  game_set_square(g, 0, 3, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2));

  game_delete(g);
  g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 0, 2, S_BLACK2);
  game_set_square(g, 0, 1, S_LIGHTBULB);
  game_set_square(g, 0, 3, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2) == false);

  game_set_square(g, 1, 2, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 0, 2));

  game_delete(g);
  g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 1, 2, S_BLACK3);
  game_set_square(g, 0, 2, S_LIGHTBULB);
  game_set_square(g, 1, 1, S_LIGHTBULB);
  game_set_square(g, 1, 3, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 1, 2) == false);

  game_set_square(g, 2, 2, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 1, 2));

  game_delete(g);
  g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 1, 2, S_BLACK4);
  game_set_square(g, 0, 2, S_LIGHTBULB);
  game_set_square(g, 1, 1, S_LIGHTBULB);
  game_set_square(g, 1, 3, S_LIGHTBULB);
  game_set_square(g, 2, 2, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 1, 2) == false);

  game_delete(g);
  g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 1, 2, S_BLACKU);
  game_set_square(g, 0, 2, S_LIGHTBULB);
  game_set_square(g, 1, 1, S_LIGHTBULB);
  game_set_square(g, 1, 3, S_LIGHTBULB);
  game_set_square(g, 2, 2, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_has_error(g, 1, 2) == false);

  game_delete(g);
  return true;
}

/*---------------------GAME-CHECK-MOVE--------------------------------*/

bool test_game_check_move() {
  game g = game_new_empty();

  assert(g != NULL);

  fprintf(stderr, "%d\n", game_get_state(g, 0, 0));
  assert(game_check_move(g, 0, 0, S_BLANK));
  assert(game_check_move(g, 0, 0, S_LIGHTBULB));
  assert(game_check_move(g, 0, 0, S_MARK));

  assert(game_check_move(g, -1, 0, S_LIGHTBULB) == false);
  assert(game_check_move(g, 0, -1, S_LIGHTBULB) == false);
  assert(game_check_move(g, 7, 0, S_LIGHTBULB) == false);
  assert(game_check_move(g, 0, 7, S_LIGHTBULB) == false);

  assert(game_check_move(g, 0, 0, S_BLACK0) == false);
  assert(game_check_move(g, 0, 0, S_BLACK1) == false);
  assert(game_check_move(g, 0, 0, S_BLACK2) == false);
  assert(game_check_move(g, 0, 0, S_BLACK3) == false);
  assert(game_check_move(g, 0, 0, S_BLACK4) == false);
  assert(game_check_move(g, 0, 0, S_BLACKU) == false);

  game_set_square(g, 0, 0, S_BLACKU);
  game_update_flags(g);

  assert(game_check_move(g, 0, 0, S_BLANK) == false);
  assert(game_check_move(g, 0, 0, S_LIGHTBULB) == false);
  assert(game_check_move(g, 0, 0, S_MARK) == false);

  game_delete(g);
  return true;
}

/*---------------------GAME-PLAY-MOVE--------------------------------*/

bool test_game_play_move() {
  game g = game_new_empty();

  assert(g != NULL);

  game_play_move(g, 0, 0, S_LIGHTBULB);
  assert(game_get_state(g, 0, 0) == S_LIGHTBULB);
  assert(game_is_lighted(g, 0, 0));

  game_play_move(g, 0, 0, S_BLANK);
  assert(game_get_state(g, 0, 0) == S_BLANK);
  assert(game_is_lighted(g, 0, 0) == false);

  game_play_move(g, 0, 0, S_MARK);
  assert(game_get_state(g, 0, 0) == S_MARK);
  assert(game_is_lighted(g, 0, 0) == false);

  game_delete(g);
  g = game_new_empty();

  game_set_square(g, 0, 0, S_BLACK1);

  game_delete(g);
  return true;
}

/*---------------------GAME-UPDATE-FLAGS--------------------------------*/

bool test_game_update_flags() {
  game g = game_new_empty();

  assert(g != NULL);

  game_set_square(g, 3, 3, S_LIGHTBULB);

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      assert(game_is_lighted(g, i, j) == false);
    }
  }

  game_update_flags(g);

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((i == 3 && j == 0) || (i == 3 && j == 1) || (i == 3 && j == 2) ||
          (i == 3 && j == 3) || (i == 3 && j == 4) || (i == 3 && j == 5) ||
          (i == 3 && j == 6) || (i == 0 && j == 3) || (i == 1 && j == 3) ||
          (i == 2 && j == 3) || (i == 4 && j == 3) || (i == 5 && j == 3) ||
          (i == 6 && j == 3)) {
        assert(game_is_lighted(g, i, j));
      } else {
        assert(game_is_lighted(g, i, j) == false);
      }
    }
  }

  game_set_square(g, 3, 0, S_LIGHTBULB);

  assert(game_has_error(g, 3, 0) == false);
  assert(game_has_error(g, 3, 3) == false);

  game_update_flags(g);

  assert(game_has_error(g, 3, 0));
  assert(game_has_error(g, 3, 3));

  game_set_square(g, 3, 0, S_BLANK);
  game_update_flags(g);

  game_set_square(g, 3, 4, S_BLACK0);

  assert(game_has_error(g, 3, 4) == false);
  assert(game_has_error(g, 3, 3) == false);

  game_update_flags(g);

  assert(game_has_error(g, 3, 4));

  game_delete(g);
  return true;
}

/*---------------------GAME-OVER--------------------------------*/

bool test_game_is_over() {
  game g = game_default_solution();  // We initialise the game to the
                                     // default_solution grid

  assert(g != NULL);  // We check the pointer

  game_set_square(g, 6, 6, S_LIGHTBULB);
  game_update_flags(g);

  assert(game_is_over(g) == false);

  game_delete(g);
  g = game_default_solution();

  assert(game_is_over(g));

  game_delete(g);
  return true;
}

/*--------------------RESTART------------------------------*/

bool test_game_restart() {
  game g = game_default();

  assert(g != NULL);

  game_set_square(g, 0, 0, S_LIGHTBULB);
  game_set_square(g, 0, 3, S_MARK);
  game_set_square(g, 0, 1, S_LIGHTBULB);
  game_set_square(g, 6, 6, S_LIGHTBULB);
  game_update_flags(g);

  // then we use the function we want to test to see if it works correctly
  game_restart(g);

  for (int i = 0; i < DEFAULT_SIZE; i++)  // Double loop to look
  {
    for (int j = 0; j < DEFAULT_SIZE; j++)  // throught all the grid
    {
      if (!(game_is_black(g, i, j))) {  // for all the square wich are not black
                                        // wall they must be blank squares
        assert(game_is_blank(g, i, j));
        assert(game_is_lighted(g, i, j) == false);
        assert(game_has_error(g, i, j) == false);
      } else {
        assert(game_has_error(g, i, j) == false);
        assert(game_is_lighted(g, i, j) == false);

        if ((i == 0 && j == 2) || (i == 4 && j == 0)) {
          assert(game_get_state(g, i, j) == S_BLACK1);
        } else if ((i == 1 && j == 2) || (i == 2 && j == 6) ||
                   (i == 5 && j == 4)) {
          assert(game_get_state(g, i, j) == S_BLACK2);
        } else if ((i == 2 && j == 5) || (i == 4 && j == 1) ||
                   (i == 6 && j == 4)) {
          assert(game_get_state(g, i, j) == S_BLACKU);
        }
      }
    }
  }

  game_delete(g);
  return true;
}

/*---------------------GAME-DEFAULT--------------------------------*/

bool test_game_default() {
  game g = game_default();

  assert(g != NULL);

  for (int i = 0; i < DEFAULT_SIZE; i++)  // Double loop to look
  {
    for (int j = 0; j < DEFAULT_SIZE; j++)  // throught all the grid
    {
      assert(game_has_error(g, i, j) == false);
      assert(game_is_lighted(g, i, j) == false);

      if ((i == 0 && j == 2) || (i == 4 && j == 0)) {
        assert(game_get_state(g, i, j) == S_BLACK1);
      } else if ((i == 1 && j == 2) || (i == 2 && j == 6) ||
                 (i == 5 && j == 4)) {
        assert(game_get_state(g, i, j) == S_BLACK2);
      } else if ((i == 2 && j == 5) || (i == 4 && j == 1) ||
                 (i == 6 && j == 4)) {
        assert(game_get_state(g, i, j) == S_BLACKU);
      } else {
        assert(game_is_blank(g, i, j));
      }
    }
  }

  game_delete(g);
  return true;
}

/*---------------------MAIN--------------------------------*/

int main(int argc, char *argv[]) {
  if (argc < 2)         // We check if if the number of
  {                     // arguments is valid
    usage(argc, argv);  // if not we use the usage function
    return EXIT_FAILURE;
  }

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);

  bool test_passed = false;

  // We check if the second argument corresponds to a function to test

  if (strcmp("dummy", argv[1]) == 0) {
    test_passed = test_dummy(argv[1]);  // We test the dummy function
  } else if (strcmp("game_nb_rows", argv[1]) == 0) {
    test_passed = test_game_nb_rows();  // We test the game_nb_rows function
  } else if (strcmp("game_nb_cols", argv[1]) == 0) {
    test_passed = test_game_nb_cols();  // We test the game_nb_cols function
  } else if (strcmp("game_is_wrapping", argv[1]) == 0) {
    test_passed =
        test_game_is_wrapping();  // We test the game_is_wrapping function
  } else if (strcmp("game_is_lighted", argv[1]) == 0) {
    test_passed =
        test_game_is_lighted();  // We test the game_is_lighted function
  } else if (strcmp("game_has_error", argv[1]) == 0) {
    test_passed = test_game_has_error();  // We test the game_has_error function
  } else if (strcmp("game_check_move", argv[1]) == 0) {
    test_passed =
        test_game_check_move();  // We test the game_check_move function
  } else if (strcmp("game_play_move", argv[1]) == 0) {
    test_passed = test_game_play_move();  // We test the game_play_move function
  } else if (strcmp("game_update_flags", argv[1]) == 0) {
    test_passed =
        test_game_update_flags();  // We test the game_update_flags function
  } else if (strcmp("game_is_over", argv[1]) == 0) {
    test_passed = test_game_is_over();  // We test the game_is_over function
  } else if (strcmp("game_restart", argv[1]) == 0) {
    test_passed = test_game_restart();  // We test the game_restart function
  } else if (strcmp("game_default", argv[1]) == 0) {
    test_passed = test_game_default();  // We test the game_default function
  } else  // if the second argument corresponds to any function to test
  {
    fprintf(stderr, "Error: test \"%s\" not found!\n",
            argv[1]);     // We print an error text
    return EXIT_FAILURE;  // and quit the program
  }

  if (test_passed == false)  // if the test has failed
  {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n",
            argv[1]);     // We print an error text
    return EXIT_FAILURE;  // and return the EXIT_FAILURE value
  }

  else  // it means the test has passed
  {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n",
            argv[1]);     // we print a succes text
    return EXIT_SUCCESS;  // and quit the program with a succes value
  }
}