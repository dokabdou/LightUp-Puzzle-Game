#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

typedef unsigned int uint;

struct game_s {
  square **grid;
  uint width;
  uint height;
  bool wrapping;
  queue *stack_moves;   // stack of moves done with latest at top
  queue *stack_undone;  // stack of undone moves
};

/**
 * @brief Structure used in the history to undo or redo a move.
 *
 */
struct history {
  uint row;
  uint column;
  square state;  // The state of the case before the move
};

/**
 * @brief Exit the programm if an expression is true i.e if there is an error
 *
 * @param expr An boolean expression
 * @param msg The message to print if there is an error
 **/
void check_if_error(bool expr, char *msg) {
  if (expr) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
  }
}

/*Fonctions Maxime*/

game game_new(square *squares) {
  check_if_error(squares == NULL, "Address invalid !");
  game g = (game)malloc(sizeof(struct game_s));
  check_if_error(g == NULL, "Not enough memory !");
  g->height = DEFAULT_SIZE;
  g->width = DEFAULT_SIZE;
  g->grid = (square **)malloc(g->height * sizeof(square *));
  check_if_error(g->grid == NULL, "Not enough memory !");
  for (int i = 0; i < g->height; i++) {
    g->grid[i] = (square *)malloc(g->width * sizeof(square));
    check_if_error(g->grid[i] == NULL, "Not enough memory !");
  }
  int k = 0;  // Index to go through the squares
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      g->grid[i][j] = squares[k];
      k++;
    }
  }
  g->wrapping = false;
  g->stack_moves = queue_new();
  g->stack_undone = queue_new();
  return g;
}

game game_new_empty() {
  struct game_s *g = (game)malloc(sizeof(struct game_s));
  check_if_error(g == NULL, "Not enough memory !");
  g->grid = (square **)malloc(DEFAULT_SIZE * sizeof(square *));
  check_if_error(g->grid == NULL, "Not enough memory !");
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    g->grid[i] = (square *)malloc(DEFAULT_SIZE * sizeof(square));
    check_if_error(g->grid[i] == NULL, "Not enough memory !");
  }
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      g->grid[i][j] = S_BLANK;
    }
  }
  g->height = DEFAULT_SIZE;
  g->width = DEFAULT_SIZE;
  g->wrapping = false;
  g->stack_moves = queue_new();
  g->stack_undone = queue_new();
  return g;
}

game game_copy(cgame g) {
  check_if_error(g == NULL || g->grid == NULL, "Address invalid !");
  game g2 = game_new_empty_ext(g->height, g->width, g->wrapping);
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      g2->grid[i][j] = g->grid[i][j];
    }
  }
  return g2;
}

bool game_equal(cgame g1, cgame g2) {
  check_if_error(
      g1 == NULL || g1->grid == NULL || g2 == NULL || g2->grid == NULL,
      "Address invalid !");
  if (g1->height != g2->height || g2->width != g1->width ||
      g1->wrapping != g2->wrapping) {
    return false;
  }
  for (int i = 0; i < g1->height; i++) {
    for (int j = 0; j < g1->width; j++) {
      if (g1->grid[i][j] != g2->grid[i][j]) {
        return false;
      }
    }
  }
  return true;
}

void game_delete(game g) {
  if (g != NULL) {
    if (g->grid != NULL) {
      for (int i = 0; i < g->height; i++) {
        if (g->grid[i] != NULL) {
          free(g->grid[i]);
        }
      }
      free(g->grid);
    }
    if (g->stack_moves != NULL) {
      queue_free_full(g->stack_moves, free);
    }
    if (g->stack_undone != NULL) {
      queue_free_full(g->stack_undone, free);
    }
    free(g);
  }
}

void game_set_square(game g, uint i, uint j, square s) {
  check_if_error(g == NULL || g->grid == NULL, "Address invalid !");
  /*check_if_error(i >= g->height || j >= g->width, "Invalid size !");*/

  if (i >= g->height) {
    fprintf(stderr, "i is too big in game_set_square\n");
    exit(EXIT_FAILURE);
  }

  if (j >= g->width) {
    fprintf(stderr, "j is too big in game_set_square\n");
    exit(EXIT_FAILURE);
  }

  check_if_error((s < 0 || s > 61) || (s > 13 && s < 16) ||
                     (s > 29 && s < 32) || (s > 45 && s < 48),
                 "s must be a valid square value !");
  g->grid[i][j] = s;
  return;
}

square game_get_square(cgame g, uint i, uint j) {
  check_if_error(g == NULL || g->grid == NULL, "Address invalid !");
  check_if_error(i >= g->height || j >= g->width, "Invalid size !");
  return g->grid[i][j];
}

/*Fonctions Abdoulaye*/
square game_get_state(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  square state_sq = g->grid[i][j];

  square state_without_flags =
      state_sq & S_MASK;  // Gets the state of the square without its flag

  return state_without_flags;
}

square game_get_flags(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");
  square state_sq = g->grid[i][j];

  square without_state =
      state_sq & F_MASK;  // Gets the flag of the square without its state

  return without_state;
}

bool game_is_blank(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  return (game_get_state(g, i, j) == S_BLANK);
}

bool game_is_lightbulb(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  return (game_get_state(g, i, j) == S_LIGHTBULB);
}

bool game_is_black(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  // check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  if (i >= g->height) {
    fprintf(stderr, "i(%u) is too big in game_is_black\n", i);
    exit(EXIT_FAILURE);
  }

  if (j >= g->width) {
    fprintf(stderr, "j(%u) is too big in game_is_black\n", j);
    exit(EXIT_FAILURE);
  }

  /*8 is the value of S_BLACK and 13 is the value of S_BLACKU*/
  return (8 <= game_get_state(g, i, j) && game_get_state(g, i, j) <= 13);
}

int game_get_black_number(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  square b_sq = game_get_state(g, i, j);

  /*8 is the value of S_BLACK and 12 value of S_BLACK4 and 13 is the value of
   * S_BLACKU*/
  if (game_is_black(g, i, j)) {
    if (b_sq == 8) {
      return 0;
    } else if (b_sq == 9) {
      return 1;
    } else if (b_sq == 10) {
      return 2;
    } else if (b_sq == 11) {
      return 3;
    } else if (b_sq == 12) {
      return 4;
    } else {
      return -1;
    }
  } else {
    fprintf(stderr, "It is not a black wall");
    exit(EXIT_FAILURE);
  }
}

bool game_is_marked(cgame g, uint i, uint j) {
  check_if_error((g == NULL || g->grid == NULL), "Address invalid !");
  check_if_error((i >= g->height || j >= g->width), "Invalid size !");

  return (game_get_state(g, i, j) == S_MARK);
}

/*Fonctions Arthur*/

bool game_is_lighted(cgame g, uint i, uint j) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  return game_get_flags(g, i, j) == F_LIGHTED ||
         game_get_flags(g, i, j) == F_LIGHTED + F_ERROR;
}

bool game_has_error(cgame g, uint i, uint j) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  return game_get_flags(g, i, j) == F_ERROR ||
         game_get_flags(g, i, j) == F_LIGHTED + F_ERROR;
}

bool game_check_move(cgame g, uint i, uint j, square s) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  if (!(s == S_BLANK) & !(s == S_LIGHTBULB) &
      !(s ==
        S_MARK))  // if s is not a blank, lightbulb or a mark -> illegal move
  {
    return false;
  }

  if (i >= g->height ||
      j >= g->width)  // if i and j are not valid coordinates -> illegal move
  {
    return false;
  }

  if (game_is_black(
          g, i, j))  // if it is already a black wall at (i,j) -> illegal move
  {
    return false;
  }

  return true;
}

void game_play_move(game g, uint i, uint j, square s) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  if (!game_check_move(g, i, j, s))  // we first check it the move is legal
  {
    return;
  }
  struct history *last_move = (struct history *)malloc(sizeof(struct history));
  check_if_error(last_move == NULL, "last_move is NULL pointer");

  last_move->row = i;
  last_move->column = j;
  last_move->state = game_get_square(g, i, j);
  queue_push_head(g->stack_moves, last_move);

  if (!queue_is_empty(g->stack_undone)) {
    queue_clear_full(
        g->stack_undone,
        free);  // Clear undone moves history used by redo, because if a move is
                // played you cannot redo it before undoing it first
  }
  game_set_square(g, i, j, s);  // then we set set the square  loop to do to
                                // save game in g->stack_moves
  game_update_flags(g);         // and we update the flags of the grid
}

void game_update_flags(game g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  // We first do a loop to initialize the flags of the squares
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      g->grid[i][j] = g->grid[i][j] & S_MASK;
    }
  }

  // Then the second loop which set the flags
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_is_lightbulb(g, i, j)) {
        if (!game_is_lighted(g, i,
                             j))  // We check that the square is not already
                                  // flagged else it would cause an error
        {
          g->grid[i][j] +=
              F_LIGHTED;  // We add the flag F_LIGHTED to the square (i,j)
        }
        // We put lighted marks on all the squares in the row or
        // in the column of the lighbulb until we meet a black wall
        int k = j - 1;  // Row negative
        while (k != j) {
          if (k < 0) {
            if (g->wrapping)  // If wrapping is enabled, then we go to the
                              // opposite side and we continue in the same
                              // direction
            {
              k = g->width - 1;
              continue;
            } else {
              break;  // Else we stop here for this direction
            }
          }
          if (game_is_black(g, i, k)) {
            break;
          }
          if (game_is_lightbulb(g, i, k)) {
            if (!game_has_error(g, i, j)) {
              g->grid[i][j] += F_ERROR;
            }
            if (!game_has_error(g, i, k)) {
              g->grid[i][k] += F_ERROR;
            }
            break;
          }
          if (!game_is_lighted(g, i, k)) {
            g->grid[i][k] += F_LIGHTED;
          }
          k--;
        }

        k = j + 1;  // Row positive
        while (k != j) {
          if (k >= g->width) {
            if (g->wrapping) {
              k = 0;
              continue;
            } else {
              break;
            }
          }
          if (game_is_black(g, i, k)) {
            break;
          }
          if (game_is_lightbulb(g, i, k)) {
            if (!game_has_error(g, i, j)) {
              g->grid[i][j] += F_ERROR;
            }
            if (!game_has_error(g, i, k)) {
              g->grid[i][k] += F_ERROR;
            }
            break;
          }
          if (!game_is_lighted(g, i, k)) {
            g->grid[i][k] += F_LIGHTED;
          }
          k++;
        }

        k = i - 1;  // Column negative
        while (k != i) {
          if (k < 0) {
            if (g->wrapping) {
              k = g->height - 1;
              continue;
            } else {
              break;
            }
          }
          if (game_is_black(g, k, j)) {
            break;
          }
          if (game_is_lightbulb(g, k, j)) {
            if (!game_has_error(g, i, j)) {
              g->grid[i][j] += F_ERROR;
            }
            if (!game_has_error(g, k, j)) {
              g->grid[k][j] += F_ERROR;
            }
            break;
          }
          if (!game_is_lighted(g, k, j)) {
            g->grid[k][j] += F_LIGHTED;
          }
          k--;
        }

        k = i + 1;  // Column positive
        while (k != i) {
          if (k >= g->height) {
            if (g->wrapping) {
              k = 0;
              continue;
            } else {
              break;
            }
          }
          if (game_is_black(g, k, j)) {
            break;
          }
          if (game_is_lightbulb(g, k, j)) {
            if (!game_has_error(g, i, j)) {
              g->grid[i][j] += F_ERROR;
            }
            if (!game_has_error(g, k, j)) {
              g->grid[k][j] += F_ERROR;
            }
            break;
          }
          if (!game_is_lighted(g, k, j)) {
            g->grid[k][j] += F_LIGHTED;
          }
          k++;
        }
      }
    }
  }
  // Else we check if the black wall haven't got too much lighbulb nearby
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_is_black(g, i, j)) {
        int black_number = game_get_black_number(g, i, j);
        if (black_number != -1) {
          uint lightbulb_nearby =
              0;  // this is the counter of lightbulb near of a black wall

          uint unlightable_squares = 0;  // Count of lighted (not lightbulb) or
                                         // marked squares around wall

          uint available_squares = 4;  // Count of squares around wall that can
                                       // be filled (not a wall or a border)

          int k;  // Variable usefull if wrapping is on to access the opposite
                  // side of the grid

          // Bottom case
          k = i + 1;
          if (k == g->height) {
            if (g->wrapping)  // If wrapping is enabled, we have to check the
                              // square on the opposite side
            {
              k = 0;
            } else {
              available_squares--;
              k = -1;  // Unused value to not trigger the next if
            }
          }
          if (k >= 0) {
            if (game_is_black(g, k, j)) {
              available_squares--;
            } else if (game_is_lightbulb(g, k, j)) {
              lightbulb_nearby++;
            } else if (game_is_lighted(g, k, j) || game_is_marked(g, k, j)) {
              unlightable_squares++;
            }
          }

          // Top case
          k = i - 1;
          if (k == -1) {
            if (g->wrapping) {
              k = g->height - 1;
            } else {
              available_squares--;
            }
          }
          if (k >= 0) {
            if (game_is_black(g, k, j)) {
              available_squares--;
            } else if (game_is_lightbulb(g, k, j)) {
              lightbulb_nearby++;
            } else if (game_is_lighted(g, k, j) || game_is_marked(g, k, j)) {
              unlightable_squares++;
            }
          }

          // Right case
          k = j + 1;
          if (k == g->width) {
            if (g->wrapping) {
              k = 0;
            } else {
              available_squares--;
              k = -1;
            }
          }
          if (k >= 0) {
            if (game_is_black(g, i, k)) {
              available_squares--;
            } else if (game_is_lightbulb(g, i, k)) {
              lightbulb_nearby++;
            } else if (game_is_lighted(g, i, k) || game_is_marked(g, i, k)) {
              unlightable_squares++;
            }
          }

          // Left case
          k = j - 1;
          if (k == -1) {
            if (g->wrapping) {
              k = g->width - 1;
            } else {
              available_squares--;
            }
          }
          if (k >= 0) {
            if (game_is_black(g, i, k)) {
              available_squares--;
            } else if (game_is_lightbulb(g, i, k)) {
              lightbulb_nearby++;
            } else if (game_is_lighted(g, i, k) || game_is_marked(g, i, k)) {
              unlightable_squares++;
            }
          }

          if (lightbulb_nearby >
              black_number) {  // If there is too much lightbulb
            g->grid[i][j] = game_get_state(g, i, j) +
                            F_ERROR;  // then we add the ERROR flag
          }                           // to the black wall
          else {
            if (available_squares - unlightable_squares <
                black_number) {  // If there is not enough blank cases around
                                 // the wall to put the
              g->grid[i][j] = game_get_state(g, i, j) +
                              F_ERROR;  // number of lightbulb required then we
                                        // add the ERROR flag
            }
          }
        }
      }
    }
  }
}

bool game_is_over(cgame g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++)  // Checks each square of the grid for
                                        // errors and lighting problems
    {
      if (game_has_error(g, i, j)) {
        return false;
      }
      if ((game_is_blank(g, i, j) || game_is_lightbulb(g, i, j)) &&
          !game_is_lighted(g, i, j)) {
        return false;
      }
    }
  }
  return true;
}

void game_restart(game g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (!game_is_black(g, i, j)) {
        game_set_square(g, i, j, S_BLANK);
      }
    }
  }
  queue_clear_full(g->stack_moves, free);
  queue_clear_full(g->stack_undone, free);
  game_update_flags(g);
}

/* game_ext.c */

// Fonctions Maxime
game game_new_ext(uint nb_rows, uint nb_cols, square *squares, bool wrapping) {
  check_if_error(squares == NULL, "Address invalid !");
  game g = (game)malloc(sizeof(struct game_s));
  check_if_error(g == NULL, "Not enough memory !");
  g->grid = (square **)malloc(nb_rows * sizeof(square *));
  check_if_error(g->grid == NULL, "Not enough memory !");
  for (int i = 0; i < nb_rows; i++) {
    g->grid[i] = (square *)malloc(nb_cols * sizeof(square));
    check_if_error(g->grid[i] == NULL, "Not enough memory !");
  }
  int k = 0;  // Indice pour parcourir squares
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      g->grid[i][j] = squares[k];
      k++;
    }
  }
  g->width = nb_cols;
  g->height = nb_rows;
  g->wrapping = wrapping;
  g->stack_moves = queue_new();
  g->stack_undone = queue_new();
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) {
  struct game_s *g = (game)malloc(sizeof(struct game_s));
  check_if_error(g == NULL, "Not enough memory !");
  g->grid = (square **)malloc(nb_rows * sizeof(square *));
  check_if_error(g->grid == NULL, "Not enough memory !");
  for (int i = 0; i < nb_rows; i++) {
    g->grid[i] = (square *)malloc(nb_cols * sizeof(square));
    check_if_error(g->grid[i] == NULL, "Not enough memory !");
  }
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      g->grid[i][j] = S_BLANK;
    }
  }
  g->width = nb_cols;
  g->height = nb_rows;
  g->wrapping = wrapping;
  g->stack_moves = queue_new();
  g->stack_undone = queue_new();
  return g;
}

// Fonctions Arthur

uint game_nb_rows(cgame g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  return g->height;
}

uint game_nb_cols(cgame g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  return g->width;
}

bool game_is_wrapping(cgame g) {
  check_if_error(g == NULL, "Game is NULL pointer");
  check_if_error(g->grid == NULL, "Grid of the game is NULL pointer");

  return g->wrapping;
}

// Fonctions Abdoulaye

void game_undo(game g) {
  if (!queue_is_empty(
          g->stack_moves))  // If there is nothing to undo, we ignore the rest
  {
    struct history *last_move = (struct history *)queue_peek_head(
        g->stack_moves);  // We get the last move played
    struct history *last_undone =
        (struct history *)malloc(sizeof(struct history));
    // check_if_error(last_undone == NULL, "Not enough memory !");
    if (last_undone == NULL) {
      fprintf(stderr, "Not enough memory !\n");
      exit(EXIT_FAILURE);
    }
    last_undone->row = last_move->row;
    last_undone->column = last_move->column;
    last_undone->state = game_get_state(g, last_move->row, last_move->column);
    queue_push_head(
        g->stack_undone,
        last_undone);  // Stocks the state of the case before the undo in the
                       // undone stack to be used in redo
    game_set_square(g, last_move->row, last_move->column, last_move->state);
    free(queue_peek_head(g->stack_moves));
    queue_pop_head(g->stack_moves);
    game_update_flags(g);
  }
}

void game_redo(game g) {
  if (!queue_is_empty(g->stack_undone)) {
    struct history *last_undone =
        (struct history *)queue_peek_head(g->stack_undone);
    struct history *last_move =
        (struct history *)malloc(sizeof(struct history));
    // check_if_error(last_undone == NULL, "Not enough memory !");
    if (last_move == NULL) {
      fprintf(stderr,
              "Not enough memory !\n");  // Store the state of the case before
                                         // the move in the history
      exit(EXIT_FAILURE);
    }
    last_move->row = last_undone->row;
    last_move->column = last_undone->column;
    last_move->state =
        game_get_square(g, last_undone->row, last_undone->column);
    queue_push_head(g->stack_moves, last_move);
    game_set_square(g, last_undone->row, last_undone->column,
                    last_undone->state);
    free(queue_peek_head(g->stack_undone));
    queue_pop_head(g->stack_undone);
    game_update_flags(g);
  }
}
