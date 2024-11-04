#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    return EXIT_FAILURE;
  }
  char* option = argv[1];
  char* input = argv[2];

  game g = game_load(input);

  if (!(strcmp(option, "-s"))) {
    bool gsol = game_solve(g);
    if (gsol == false) {
      return EXIT_FAILURE;
    } else {
      if (argc > 3) {
        game_save(g, argv[3]);
      } else {
        game_print(g);
      }
    }
  }

  else if (!(strcmp(option, "-c"))) {
    uint nbsol = game_nb_solutions(g);
    if (argc > 3) {
      FILE* f = fopen(argv[3], "w");
      fprintf(f, "%u\n", nbsol);
      fclose(f);
    } else {
      printf("%u\n", nbsol);
    }
  }

  return EXIT_SUCCESS;
}