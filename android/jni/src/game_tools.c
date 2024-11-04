#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

game game_load(char* filename) {
  check_if_error(filename == NULL, "Invalid file name !");
  FILE* f = fopen(filename, "r");
  check_if_error(f == NULL, "File error");

  int rows, cols, val_wrapping;

  fscanf(f, "%u %u %d", &rows, &cols, &val_wrapping);

  game g = game_new_empty_ext(rows, cols, val_wrapping);

  fgetc(f);

  char state;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fscanf(f, "%c", &state);
      if (state == 'b') {
        game_set_square(g, i, j, S_BLANK);
      } else if (state == '0') {
        game_set_square(g, i, j, S_BLACK0);
      } else if (state == '1') {
        game_set_square(g, i, j, S_BLACK1);
      } else if (state == '2') {
        game_set_square(g, i, j, S_BLACK2);
      } else if (state == '3') {
        game_set_square(g, i, j, S_BLACK3);
      } else if (state == '4') {
        game_set_square(g, i, j, S_BLACK4);
      } else if (state == 'w') {
        game_set_square(g, i, j, S_BLACKU);
      } else if (state == '*') {
        game_set_square(g, i, j, S_LIGHTBULB);
      } else if (state == '-') {
        game_set_square(g, i, j, S_MARK);
      } else {
        fprintf(stderr, "It is not the correct state format ! [%d,%d]\n", i, j);
        exit(EXIT_FAILURE);
        // printf("not valid");
      }
    }
    fgetc(f);
  }
  game_update_flags(g);
  fclose(f);
  return g;
}

void game_save(cgame g, char* filename) {
  check_if_error(g == NULL, "Invalid game !");
  check_if_error(filename == NULL, "Invalid file name !");

  FILE* f = fopen(filename, "w");
  check_if_error(f == NULL, "Unable to open the file !");

  fprintf(f, "%u %u %d\n", game_nb_rows(g), game_nb_cols(g),
          game_is_wrapping(g));

  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      square s = game_get_state(g, i, j);

      if (s == S_BLANK) {
        fprintf(f, "b");
      } else if (s == S_LIGHTBULB) {
        fprintf(f, "*");
      } else if (s == S_MARK) {
        fprintf(f, "-");
      } else if (s == S_BLACKU) {
        fprintf(f, "w");
      } else {
        fprintf(f, "%d", game_get_black_number(g, i, j));
      }
    }
    fprintf(f, "\n");
  }

  fclose(f);
  return;
}

/*----------------OPTIMIZE SOLVER---------------------*/
void g_solve_rec(game g, int s_rows, int s_cols) {
  /*
      Solver that uses an optimized method to find solutions. If lighted or wall
     it is skipped!
  */

  if (game_is_over(g)) {
    return;
  }

  if (s_cols == game_nb_cols(g) && s_rows == game_nb_rows(g)) {
    return;
  }
  if (s_rows == game_nb_rows(g)) return;

  if (s_cols == game_nb_cols(g)) {
    return g_solve_rec(g, s_rows + 1, 0);
  }

  // checks if there is an error anywhere on the grid
  if (!no_error_whole_game(g)) return;

  if ((game_is_lighted(g, s_rows, s_cols)) ||
      (game_is_black(g, s_rows, s_cols))) {
    g_solve_rec(g, s_rows, s_cols + 1);
  }

  else {
    if (game_check_move(g, s_rows, s_cols, S_LIGHTBULB)) {
      game_set_square(g, s_rows, s_cols, S_LIGHTBULB);  // places lightbulb
      game_update_flags(g);
    } /*else {
      g_solve_rec(g, s_rows, s_cols + 1);
    }*/
    if (game_has_error(g, s_rows,
                       s_cols)) {  // the placement causes an error, we remove
                                   // it and go to the next one
      game_set_square(g, s_rows, s_cols, S_BLANK);
      game_update_flags(g);
      g_solve_rec(g, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
    } else {  // no errors so we go to the next one
      g_solve_rec(g, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
      game_set_square(g, s_rows, s_cols, S_BLANK);
      game_update_flags(g);
      g_solve_rec(g, s_rows, s_cols + 1);
      if (game_is_over(g)) {
        return;
      }
    }
  }
}

/*-----------SOLVER------------------*/
bool game_solve(game g) {
  game g1 = game_copy(g);
  game_restart(g1);

  g_solve_rec(g1, 0, 0);  // optimized method
  // game g_sol = game_solve_aux(g1, 0, 0);//brute force method
  if (game_is_over(g1)) {
    for (int i = 0; i < game_nb_rows(g); i++)
      for (int j = 0; j < game_nb_cols(g); j++)
        game_set_square(g, i, j, game_get_square(g1, i, j));
    // game_delete(g_sol);
    game_delete(g1);
    return true;
  }
  game_delete(g1);
  return false;
}

bool no_error_whole_game(game g) {
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_has_error(g, i, j)) {
        return false;
      }
    }
  }
  return true;
}

/*-----------SOLUTION COUNTER------------------*/

void game_nb_solutions_aux(uint* count, game g, int s_rows, int s_cols) {
  /*
  Takes the game to solve, solutions already found, and counter for how many
  solution must be found. Takes s_rows(starting row) at 0, s_cols(starting
  column) at 0, lenght of rows and columns  of the game. For easier
  nagivation(maybe ??), inspired by binary brute-force function For now solves
  games that have a row size of 3
  */
  if (s_rows == game_nb_rows(g)) {
    if (game_is_over(g)) {
      (*count)++;
      // game_print(g);
    }
    return;
  }

  if (!no_error_whole_game(g)) return;

  if (s_cols == game_nb_cols(g)) {  // goes to next row
    return game_nb_solutions_aux(count, g, s_rows + 1, 0);
  }

  if (game_is_black(g, s_rows, s_cols)) {
    return game_nb_solutions_aux(count, g, s_rows, s_cols + 1);
  } else if (game_is_lighted(g, s_rows, s_cols)) {
    return game_nb_solutions_aux(count, g, s_rows, s_cols + 1);
  }

  else {
    if (game_check_move(g, s_rows, s_cols, S_LIGHTBULB)) {
      game_set_square(g, s_rows, s_cols, S_LIGHTBULB);  // places lightbulb
      game_update_flags(g);
    }
    if (game_has_error(g, s_rows,
                       s_cols)) {  // the placement causes an error, we remove
                                   // it and go to the next one
      game_play_move(g, s_rows, s_cols, S_BLANK);
      game_nb_solutions_aux(count, g, s_rows, s_cols + 1);
    } else {  // no errors so we go to the next one
      game_nb_solutions_aux(count, g, s_rows, s_cols + 1);
      game_set_square(g, s_rows, s_cols, S_BLANK);
      game_update_flags(g);
      game_nb_solutions_aux(count, g, s_rows, s_cols + 1);
    }
  }
}

uint game_nb_solutions(cgame g) {
  game g2 = game_copy(g);
  uint count = 0;
  game_nb_solutions_aux(&count, g2, 0, 0);
  return count;
}

/* ************************************************************************** */

static uint nb_neigh_lightbulbs(cgame g, uint i, uint j) {
  assert(g);
  int nb_rows = game_nb_rows(g);
  int nb_cols = game_nb_cols(g);
  assert(i < nb_rows);
  assert(j < nb_cols);

  int i_up = i - 1;
  int i_down = i + 1;
  int j_left = j - 1;
  int j_right = j + 1;
  if (game_is_wrapping(g)) {
    i_up = (i_up + nb_rows) % nb_rows;
    i_down = (i_down + nb_rows) % nb_rows;
    j_left = (j_left + nb_cols) % nb_cols;
    j_right = (j_right + nb_cols) % nb_cols;
  }

  uint count = 0;
  if ((i_up >= 0) && game_is_lightbulb(g, i_up, j)) count++;
  if ((i_down < nb_rows) && game_is_lightbulb(g, i_down, j)) count++;
  if ((j_left >= 0) && game_is_lightbulb(g, i, j_left)) count++;
  if ((j_right < nb_cols) && game_is_lightbulb(g, i, j_right)) count++;

  return count;
}

/* ************************************************************************** */

static uint nb_unlit_squares(cgame g) {
  int nb = 0;
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++)
      if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) nb++;
  return nb;
}

/* ************************************************************************** */

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only
 * walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls,
                 bool with_solution) {
  assert(nb_walls <= nb_rows * nb_cols);

  // step 0: create an empty game
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);

  // step 1: add random black walls
  uint k = 0;
  while (k < nb_walls) {
    uint i = rand() % nb_rows;
    uint j = rand() % nb_cols;
    if (!game_is_black(g, i, j)) {
      game_set_square(g, i, j, S_BLACKU);
      k++;
    }
  }
  game_update_flags(g);

  // step 2: add lightbulbs until every squares are lighted
  uint nb_unlit = nb_unlit_squares(g);
  while (nb_unlit != 0) {
    uint random_unlit_num = rand() % nb_unlit;
    uint num = 0;
    for (uint i = 0; i < game_nb_rows(g); i++)
      for (uint j = 0; j < game_nb_cols(g); j++) {
        if (!game_is_lighted(g, i, j) && game_is_blank(g, i, j)) {
          if (num == random_unlit_num) {
            game_set_square(g, i, j, S_LIGHTBULB);
            game_update_flags(g);
          }
          num++;
        }
      }
    nb_unlit = nb_unlit_squares(g);
  }

  // step 3 : set some black wall numbers
  for (uint i = 0; i < game_nb_rows(g); i++)
    for (uint j = 0; j < game_nb_cols(g); j++) {
      if (game_is_black(g, i, j)) {
        if (rand() % 2 == 0) {
          int nb_lightbulbs = nb_neigh_lightbulbs(g, i, j);
          game_set_square(g, i, j, S_BLACK + nb_lightbulbs);
        }
      }
    }

  // check
  assert(game_is_over(g));

  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */