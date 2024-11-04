// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL2/SDL.h>        /*"SDL2/" in order to find SDL2*/
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "model.h"

/* **************************************************************** */

// define all the images
#define FONT "arial.ttf"
#define SPACE "space.png"
#define SUN "Babysun.png"
#define ASTEROID0 "asteroid_0.png"
#define ASTEROID1 "asteroid_1.png"
#define ASTEROID2 "asteroid_2.png"
#define ASTEROID3 "asteroid_3.png"
#define ASTEROID4 "asteroid_4.png"
#define ASTEROIDU "asteroid_u.png"
#define STAR "star.png"
#define MARK_S "mark_satellite.png"
#define RESTART "button_R.png"
#define SOLVE "button_S.png"
#define UNDO "button_Z.png"
#define REDO "button_Y.png"
#define SAVE "button_save.png"
#define GRID "grid.png"
#define RULES "rules.png"
#define WON "victory.png"

/* **************************************************************** */
// https://wiki.libsdl.org/
struct Env_t {
  /* Background */
  SDL_Texture *background;

  /* Elements of the game */
  SDL_Texture *lightbulb;
  SDL_Texture *mark;
  SDL_Texture *lighted;
  SDL_Texture *wall_u;
  SDL_Texture *wall_0;
  SDL_Texture *wall_1;
  SDL_Texture *wall_2;
  SDL_Texture *wall_3;
  SDL_Texture *wall_4;

  /* Errors */
  SDL_Texture *error_lightbulb;
  SDL_Texture *error_wall_u;
  SDL_Texture *error_wall_0;
  SDL_Texture *error_wall_1;
  SDL_Texture *error_wall_2;
  SDL_Texture *error_wall_3;
  SDL_Texture *error_wall_4;

  /* Buttons */
  SDL_Texture *restart_button;
  SDL_Texture *solve_button;
  SDL_Texture *undo_button;
  SDL_Texture *redo_button;
  SDL_Texture *save_button;
  SDL_Texture *rando_game_button;

  /* Coordinates of buttons */
  int coord_x_restart;
  int coord_x_solve;
  int coord_x_undo;
  int coord_x_redo;
  int coord_x_save;
  int coord_x_rando_game;

  int coord_y_buttons;
  int width_buttons;
  int height_buttons;

  /* The game itself */
  game g;

  /* Grid and rules */
  SDL_Texture *grid;
  SDL_Texture *rules;

  /* Message when game is won */
  SDL_Texture *game_is_won;
};

/* **************************************************************** */
/**
 * @brief
 *
 * @param win
 * @param ren
 * @param argc
 * @param argv
 * @return Env*
 */
Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  // get window size
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  srand(time(NULL));

  uint r = (rand() % (10 - 4 + 1)) + 4,
       c = (rand() % (10 - 4 + 1)) + 4;  // random sizes from 4 to 10

  uint n_w;
  if ((r * c) % 2 == 0) {
    n_w = (r * c) / 2;
  } else {
    n_w = (r * c) / 2 + 1;
  }
  // uint n_w = (rand() % ((r * c) - (r * c) * 5 / 100) - 1) + ((r * c) * 5 /
  // 100);  //
  //  number of walls between 5% of number of
  //                                  // squares and number of squares on the
  //                                  grid
  //   printf("rows=%u, cols=%u \n",r,c);
  //   printf("(r*c) = %u, 1+((r*c)*5/100)= %u \n",(r*c),1+((r*c)*5/100));
  //   printf("number of walls= %u \n",n_w);

  env->g = game_random(r, c, false, n_w, false);
  /*   if (argc > 1) {
      env->g = game_load(argv[1]);
    } else {
      env->g = game_default();
    }
   */
  // initialization of all textures from PNGs
  // background
  env->background = IMG_LoadTexture(ren, SPACE);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", SPACE);
  // lightbulb
  env->lightbulb = IMG_LoadTexture(ren, SUN);
  if (!env->lightbulb) ERROR("IMG_LoadTexture: %s\n", SUN);
  // mark
  env->mark = IMG_LoadTexture(ren, MARK_S);
  if (!env->mark) ERROR("IMG_LoadTexture: %s\n", MARK_S);
  // lighted
  env->lighted = IMG_LoadTexture(ren, STAR);
  if (!env->lighted) ERROR("IMG_LoadTexture: %s\n", STAR);
  // wall_u
  env->wall_u = IMG_LoadTexture(ren, ASTEROIDU);
  if (!env->wall_u) ERROR("IMG_LoadTexture: %s\n", ASTEROIDU);
  // wall_0
  env->wall_0 = IMG_LoadTexture(ren, ASTEROID0);
  if (!env->wall_0) ERROR("IMG_LoadTexture: %s\n", ASTEROID0);
  // wall_1
  env->wall_1 = IMG_LoadTexture(ren, ASTEROID1);
  if (!env->wall_1) ERROR("IMG_LoadTexture: %s\n", ASTEROID1);
  // wall_2
  env->wall_2 = IMG_LoadTexture(ren, ASTEROID2);
  if (!env->wall_2) ERROR("IMG_LoadTexture: %s\n", ASTEROID3);
  // wall_3
  env->wall_3 = IMG_LoadTexture(ren, ASTEROID3);
  if (!env->wall_3) ERROR("IMG_LoadTexture: %s\n", ASTEROID3);
  // wall_4
  env->wall_4 = IMG_LoadTexture(ren, ASTEROID4);
  if (!env->wall_4) ERROR("IMG_LoadTexture: %s\n", ASTEROID4);

  // error_lightbulb
  env->error_lightbulb = IMG_LoadTexture(ren, SUN);
  if (!env->error_lightbulb) ERROR("IMG_LoadTexture: %s\n", SUN);
  SDL_SetTextureColorMod(env->error_lightbulb, 200, 0, 0);
  // error_wall_u
  env->error_wall_u = IMG_LoadTexture(ren, ASTEROIDU);
  if (!env->error_wall_u) ERROR("IMG_LoadTexture: %s\n", ASTEROIDU);
  SDL_SetTextureColorMod(env->error_wall_u, 200, 0, 0);
  // error_wall_0
  env->error_wall_0 = IMG_LoadTexture(ren, ASTEROID0);
  if (!env->error_wall_0) ERROR("IMG_LoadTexture: %s\n", ASTEROID0);
  SDL_SetTextureColorMod(env->error_wall_0, 200, 0, 0);
  // error_wall_1
  env->error_wall_1 = IMG_LoadTexture(ren, ASTEROID1);
  if (!env->error_wall_1) ERROR("IMG_LoadTexture: %s\n", ASTEROID1);
  SDL_SetTextureColorMod(env->error_wall_1, 200, 0, 0);
  // error_wall_2
  env->error_wall_2 = IMG_LoadTexture(ren, ASTEROID2);
  if (!env->error_wall_2) ERROR("IMG_LoadTexture: %s\n", ASTEROID3);
  SDL_SetTextureColorMod(env->error_wall_2, 200, 0, 0);
  // error_wall_3
  env->error_wall_3 = IMG_LoadTexture(ren, ASTEROID3);
  if (!env->error_wall_3) ERROR("IMG_LoadTexture: %s\n", ASTEROID3);
  SDL_SetTextureColorMod(env->error_wall_3, 200, 0, 0);
  // error_wall_4
  env->error_wall_4 = IMG_LoadTexture(ren, ASTEROID4);
  if (!env->error_wall_4) ERROR("IMG_LoadTexture: %s\n", ASTEROID4);
  SDL_SetTextureColorMod(env->error_wall_4, 200, 0, 0);

  // restart button
  env->restart_button = IMG_LoadTexture(ren, RESTART);
  if (!env->restart_button) ERROR("IMG_LoadTexture: %s\n", RESTART);

  // solve button
  env->solve_button = IMG_LoadTexture(ren, SOLVE);
  if (!env->solve_button) ERROR("IMG_LoadTexture: %s\n", SOLVE);

  // undo button
  env->undo_button = IMG_LoadTexture(ren, UNDO);
  if (!env->undo_button) ERROR("IMG_LoadTexture: %s\n", UNDO);

  // redo button
  env->redo_button = IMG_LoadTexture(ren, REDO);
  if (!env->redo_button) ERROR("IMG_LoadTexture: %s\n", REDO);

  // save button
  env->save_button = IMG_LoadTexture(ren, SAVE);
  if (!env->save_button) ERROR("IMG_LoadTexture: %s\n", SAVE);

  // random game button
  env->rando_game_button =
      IMG_LoadTexture(ren, RESTART);  // same icon as restart for now
  if (!env->rando_game_button) ERROR("IMG_LoadTexture: %s\n", RESTART);

  // grid border
  env->grid = IMG_LoadTexture(ren, GRID);
  if (!env->grid) ERROR("IMG_LoadTexture: %s\n", GRID);

  // rules
  env->rules = IMG_LoadTexture(ren, RULES);
  if (!env->rules) ERROR("IMG_LoadTexture: %s\n", RULES);

  // game is won
  env->game_is_won = IMG_LoadTexture(ren, WON);
  if (!env->game_is_won) ERROR("IMG_LoadTexture: %s\n", WON);

  return env;
}

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
  SDL_LogInfo(SDL_LOG_CATEGORY_RENDER,"Prob");

  SDL_Rect rect;
  game g = env->g;
  int cols = game_nb_cols(g);
  int rows = game_nb_rows(g);

  /* get current window size */
  int w, h;  // width, height of window
  SDL_GetWindowSize(win, &w, &h);

  uint sq;  // size of a square based on the size of the grid
  if (((w * 8 / 10) / cols) < ((h * 8 / 10) / rows)) {
    sq = (w * 8 / 10) / cols;
  } else {
    sq = (h * 8 / 10) / rows;
  }

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  int w1, h1;      // changes of window size for game grid
  w1 = sq * cols;  // width necessary for the game
  h1 = sq * rows;  // heigth necessary for the game

  SDL_Rect edge;                            // edge of the grid in white
  edge.x = ((w - w1) / 2) - 15;              // centered
  edge.y = (((h - sq - 10) - h1) / 2) - 15;  // centered
  // edge.x = (w1 / 10)-1; //not centered
  // edge.y = (h1 / 10)-1; //not centered
  edge.w = w1 + 15;
  edge.h = h1 + 15;
  SDL_RenderCopy(ren, env->grid, NULL, &edge);


  SDL_Rect help;  // image containing the game instructions
  help.x = edge.x + edge.w;
  help.y = edge.y - 1;
  help.w = w - (help.x);
  help.h = 3 * edge.h / 4;
  SDL_RenderCopy(ren, env->rules, NULL, &help);

  /*loop to make the grid of the game on screen based on the size of the game*/
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      rect.x = sq * j + edge.x + 5;
      rect.y = sq * i + edge.y + 5;
      rect.w = sq - 2;
      rect.h = sq - 2;
      if (game_is_lightbulb(g, i, j)) {  // there is a lightbulb on this square
        if (game_has_error(g, i, j)) {
          SDL_RenderCopy(ren, env->error_lightbulb, NULL, &rect);
        } else {
          SDL_RenderCopy(ren, env->lightbulb, NULL, &rect);
        }
      } else if (game_is_lighted(g, i, j) &&
                 !game_is_marked(g, i, j)) {  // square is lighted
        SDL_RenderCopy(ren, env->lighted, NULL, &rect);
      } else if (game_is_black(g, i, j) &&
                 !game_has_error(
                     g, i,
                     j)) {  // square is a black square without errors
        if (game_get_black_number(g, i, j) == 0) {
          SDL_RenderCopy(ren, env->wall_0, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 1) {
          SDL_RenderCopy(ren, env->wall_1, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 2) {
          SDL_RenderCopy(ren, env->wall_2, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 3) {
          SDL_RenderCopy(ren, env->wall_3, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 4) {
          SDL_RenderCopy(ren, env->wall_4, NULL, &rect);
        } else {
          SDL_RenderCopy(ren, env->wall_u, NULL, &rect);
        }
      } else if (game_is_black(g, i, j) &&
                 game_has_error(g, i,
                                j)) {  // square is a black square with errors
        if (game_get_black_number(g, i, j) == 0) {
          SDL_RenderCopy(ren, env->error_wall_0, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 1) {
          SDL_RenderCopy(ren, env->error_wall_1, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 2) {
          SDL_RenderCopy(ren, env->error_wall_2, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 3) {
          SDL_RenderCopy(ren, env->error_wall_3, NULL, &rect);
        } else if (game_get_black_number(g, i, j) == 4) {
          SDL_RenderCopy(ren, env->error_wall_4, NULL, &rect);
        } else {
          SDL_RenderCopy(ren, env->error_wall_u, NULL, &rect);
        }
      } else if (game_is_marked(g, i, j)) {  // square is marked
        SDL_RenderCopy(ren, env->mark, NULL, &rect);
      }
    }
  }


  /* buttons next to te grid */
  /* button restart */
  SDL_Rect button;
  button.x = edge.x;
  env->coord_x_restart = button.x;
  button.y = edge.y + h1 + 10;
  env->coord_y_buttons = button.y;
  button.w = w1 / 6;  // width of window divided by the number of buttons
  env->width_buttons = button.w;
  button.h = w1 / 6;
  env->height_buttons = button.h;
  SDL_RenderCopy(ren, env->restart_button, NULL, &button);
  /*  solve button */
  button.x = edge.x + 2 + w1 / 6;
  env->coord_x_solve = button.x;
  SDL_RenderCopy(ren, env->solve_button, NULL, &button);
  /* undo button */
  button.x = edge.x + 2 * (2 + w1 / 6);
  env->coord_x_undo = button.x;
  SDL_RenderCopy(ren, env->undo_button, NULL, &button);
  /* redo button */
  button.x = edge.x + 3 * (2 + w1 / 6);
  env->coord_x_redo = button.x;
  SDL_RenderCopy(ren, env->redo_button, NULL, &button);
  /* save button */
  button.x = edge.x + 4 * (2 + w1 / 6);
  env->coord_x_save = button.x;
  SDL_RenderCopy(ren, env->save_button, NULL, &button);
  /* random game button */
  button.x = edge.x + 5 * (2 + w1 / 6);
  env->coord_x_rando_game = button.x;
  SDL_RenderCopy(ren, env->rando_game_button, NULL,
                 &button);  // icon same as restart for now

  /*Prints an image when the game is won. It will be printed in middle of the
   * grid*/
  if (game_is_over(env->g)) {
    SDL_Rect vic;
    vic.x = 0;
    vic.y = h / 2 - sq / 2;
    vic.w = w;
    vic.h = sq;
    // SDL_RenderCopy(ren, env->lightbulb, NULL, NULL);
    SDL_RenderCopy(ren, env->game_is_won, NULL, &vic);
  }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int w1, h1;  // same as in the render (for window size changes)
  uint rows = game_nb_rows(env->g), cols = game_nb_cols(env->g);
  uint sq;  // size of a square based on the size of the grid
  if (((w * 8 / 10) / cols) < ((h * 8 / 10) / rows)) {
    sq = (w * 8 / 10) / cols;
  } else {
    sq = (h * 8 / 10) / rows;
  }
  w1 = sq * cols;  // width necessary for the game
  h1 = sq * rows;  // heigth necessary for the game

  if (e->type == SDL_QUIT) {
    return true;
  }

  if (e->type == SDL_KEYDOWN) {
    if (e->key.keysym.sym == SDLK_ESCAPE) {
      return true;
    }
  }

  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    char action_type;

    if (mouse.x > ((w - w1) / 2) && mouse.x < ((w - w1) / 2) + w1 &&
        mouse.y > (((h - sq - 10) - h1) / 2) &&
        mouse.y < (((h - sq - 10) - h1) / 2) + h1) {
      action_type = 'p';  // for play move (if in the grid)
    } else if (mouse.x > env->coord_x_restart &&
               mouse.x < env->coord_x_restart + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 'r';  // for restart
    } else if (mouse.x > env->coord_x_solve &&
               mouse.x < env->coord_x_solve + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 's';  // for solve
    } else if (mouse.x > env->coord_x_undo &&
               mouse.x < env->coord_x_undo + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 'z';  // for undo
    } else if (mouse.x > env->coord_x_redo &&
               mouse.x < env->coord_x_redo + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 'y';  // for redo
    } else if (mouse.x > env->coord_x_save &&
               mouse.x < env->coord_x_save + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 'w';  // for save
    } else if (mouse.x > env->coord_x_rando_game &&
               mouse.x < env->coord_x_rando_game + env->width_buttons &&
               mouse.y > env->coord_y_buttons &&
               mouse.y < env->coord_y_buttons + env->height_buttons) {
      action_type = 'g';  // for new random game
    }

    if (e->button.button == SDL_BUTTON_LEFT) {
      if (action_type == 'p') {
        // getting the game coordinates i and j of the square
        uint i = 0, j = 0;

        /* FINDING THE RIGHT SQUARE
        going backwards on the grid until x and y are in the first square
        then we know our i and j (number of iterations)
        (((h-sq-10)-h1)/2) = space between window edge and the grid
        the (-2) represents the thin space between each square */
        for (uint y = mouse.y; y > (((h - sq - 10) - h1) / 2) + sq;
             y = y - sq - 2) {
          i++;
        }
        for (uint x = mouse.x; x > ((w - w1) / 2) + sq; x = x - sq - 2) {
          j++;
        }

        if (game_is_blank(env->g, i, j) &&
            game_check_move(env->g, i, j, S_LIGHTBULB)) {
          game_play_move(env->g, i, j, S_LIGHTBULB);
        }

        else if (game_is_lightbulb(env->g, i, j) &&
                 game_check_move(env->g, i, j, S_BLANK)) {
          game_play_move(env->g, i, j, S_BLANK);
        }
      }

      else {
        if (action_type == 'r') {
          game_restart(env->g);
        } else if (action_type == 's') {
          game_solve(env->g);
        } else if (action_type == 'z') {
          game_undo(env->g);
        } else if (action_type == 'y') {
          game_redo(env->g);
        } else if (action_type == 'w') {
          game_save(env->g, "saved_game.txt");
        } else if (action_type == 'g') {
          srand(time(NULL));
          uint r = (rand() % (10 - 4 + 1)) + 4,
               c = (rand() % (10 - 4 + 1)) + 4;  // random sizes from 4 to 10
          uint n_w;
          if((r*c)%2 == 0){
            n_w = (r*c)/2;
          }else{
            n_w = (r*c)/2+1;
          }
          // uint n_w =
          //     (rand() % ((r * c) - (r * c) * 5 / 100) - 1) +
          //     ((r * c) * 5 / 100);  // number of walls between 5% of number of
                                    // squares and number of squares on the grid

          env->g = game_random(r, c, false, n_w, false);
        }
      }
    }

    if (e->button.button == SDL_BUTTON_RIGHT) {
      if (action_type == 'p') {
        // same thing as left button
        uint i = 0, j = 0;

        for (uint y = mouse.y; y > (((h - sq - 10) - h1) / 2) + sq;
             y = y - sq - 2) {
          i++;
        }
        for (uint x = mouse.x; x > ((w - w1) / 2) + sq; x = x - sq - 2) {
          j++;
        }

        if (game_is_blank(env->g, i, j) &&
            game_check_move(env->g, i, j, S_MARK)) {
          game_play_move(env->g, i, j, S_MARK);
        }

        else if (game_is_marked(env->g, i, j) &&
                 game_check_move(env->g, i, j, S_BLANK)) {
          game_play_move(env->g, i, j, S_BLANK);
        }
      }
    }
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* CLEANING THE MEMORY */
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->lightbulb);
  SDL_DestroyTexture(env->mark);
  SDL_DestroyTexture(env->lighted);
  SDL_DestroyTexture(env->wall_u);
  SDL_DestroyTexture(env->wall_0);
  SDL_DestroyTexture(env->wall_1);
  SDL_DestroyTexture(env->wall_2);
  SDL_DestroyTexture(env->wall_3);
  SDL_DestroyTexture(env->wall_4);

  SDL_DestroyTexture(env->error_lightbulb);
  SDL_DestroyTexture(env->error_wall_u);
  SDL_DestroyTexture(env->error_wall_0);
  SDL_DestroyTexture(env->error_wall_1);
  SDL_DestroyTexture(env->error_wall_2);
  SDL_DestroyTexture(env->error_wall_3);
  SDL_DestroyTexture(env->error_wall_4);

  SDL_DestroyTexture(env->restart_button);
  SDL_DestroyTexture(env->solve_button);
  SDL_DestroyTexture(env->undo_button);
  SDL_DestroyTexture(env->redo_button);
  SDL_DestroyTexture(env->save_button);
  SDL_DestroyTexture(env->rando_game_button);

  game_delete(env->g);

  SDL_DestroyTexture(env->grid);
  SDL_DestroyTexture(env->rules);

  SDL_DestroyTexture(env->game_is_won);

  free(env);
}

/* **************************************************************** */
