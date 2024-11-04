#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  game current_game;
  if (argc > 1) {
    current_game = game_load(argv[1]);
  } else {
    current_game = game_default();
  }
  while (!game_is_over(current_game)) {
    game_print(current_game);
    uint i, j;
    for (i = 0; i < game_nb_rows(current_game); i++) {
      for (j = 0; j < game_nb_cols(current_game); j++) {
        if (game_has_error(current_game, i, j)) {
          square s_tmp =
              game_get_state(current_game, i,
                             j);  // Checks and prints errors, square by square
          if (s_tmp == S_LIGHTBULB) {
            printf("Error at lightbulb (%u,%u)\n", i, j);
          } else {
            printf("Error at wall (%u,%u)\n", i, j);
          }
        }
      }
    }
    printf("Enter your command: [h for help]\n");
    char cmd;
    int ret = scanf(" %c", &cmd);  // Scan de la commande
    if (ret == EOF) {
      printf("End of File!\n");
      return EXIT_SUCCESS;
    } else if (cmd == 'h') {
      printf("Commands : \n");
      printf(
          "  - Type 'l <i> <j>' to place a lightbulb at coordinates (i,j).\n");
      printf("  - Type 'm <i> <j>' to place a mark at coordinates (i,j).\n");
      printf(
          "  - Type 'b <i> <j>' to empty the square at coordinates (i,j).\n");
      printf("  - Type 'z' to undo the last move.\n");
      printf("  - Type 'y' to redo the last move.\n");
      printf("  - Type 'w' to save the game.\n");
      printf("  - Type 'r' to restart.\n");
      printf("  - Type 'q' to quit.\n");
    } else if (cmd == 'r') {  // Restart
      printf(">Game restarted");
      game_restart(current_game);
    } else if (cmd == 'q' && argc == 1) {
      current_game = game_default_solution();
      game_print(current_game);  // Quitting the game and printing the solution
                                 // for default game
      printf("Printing solution for default game.\n");
      printf("What a shame, you gave up... loser :(\n");
      game_delete(current_game);
      return EXIT_SUCCESS;
    } else if (cmd == 's')  // quit solve game and numb of solutions
    {
      if (game_solve(current_game)) {
        printf("The first solution of this game found is: \n");
        game_print(current_game);
      } else {
        printf("This game does not have a solution !\n");
      }
      printf("Number of solutions of this game is: %u !\n",
             game_nb_solutions(current_game));
      game_delete(current_game);
      return EXIT_SUCCESS;
    } else if (cmd == 'q') {
      printf("What a shame, you gave up... loser :(\n");
      return EXIT_SUCCESS;
    } else if (cmd == 'z') {
      printf("Last move undone.\n");
      game_undo(current_game);
    } else if (cmd == 'y') {
      printf("Last move redone.\n");
      game_redo(current_game);
    } else if (cmd == 'w') {
      char* s_filename = "";  // file in which the game will be saved
      scanf("%s", s_filename);

      game_save(current_game, s_filename);
      printf("Saving game! \n");
    } else if (cmd == 'l' || cmd == 'm' || cmd == 'b') {
      ret = scanf(
          "%u %u", &i,
          &j);  // If the command is placement(l,b,m) then we retrieve the data
      if (ret == EOF) {
        printf("End of File!\n");
        return EXIT_SUCCESS;
      } else if (ret < 1) {
        printf("Invalid command\nType h for help\n");
      } else {
        if (i >= game_nb_rows(current_game) ||
            j >= game_nb_cols(current_game)) {  // Checking of the coordinates
          printf(
              "Coordinates are out of the grid. Please enter a value "
              "between 0 and 6.\n");
        } else {
          if (game_check_move(current_game, i, j,
                              game_get_state(current_game, i, j))) {
            if (cmd == 'l') {
              game_play_move(current_game, i, j, S_LIGHTBULB);
              printf("Lightbulb placed at square (%u,%u)\n", i, j);
            }
            if (cmd ==
                'm') {  // Modification of the given square based on the command
              game_play_move(current_game, i, j, S_MARK);
              printf("Marked placed at square (%u,%u)\n", i, j);
            }
            if (cmd == 'b') {
              game_play_move(current_game, i, j, S_BLANK);
              printf("Blank placed at square (%u,%u)\n", i, j);
            }
          }
        }
      }
    } else {
      printf("Invalid command.\nType h for help.\n");
    }
    printf("\n");
    char clear;  // Erases the buffer to ignore all accumulation of characters
                 // creating an unwanted command because of scanf
    while ((clear = getchar()) != '\n' && clear != EOF) {
    }
  }
  game_print(current_game);
  game_delete(current_game);
  printf("Congratulations, you won :D\n");
  return EXIT_SUCCESS;
}
