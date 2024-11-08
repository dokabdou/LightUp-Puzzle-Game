/**
 * @file game_tools.h
 * @brief Game Tools.
 * @details See @ref index for further details.
 * @copyright University of Bordeaux. All rights reserved, 2021.
 *
 **/

#ifndef __GAME_TOOLS_H__
#define __GAME_TOOLS_H__
#include <stdbool.h>
#include <stdio.h>

#include "game.h"

/**
 * @name Game Tools
 * @{
 */

/**
 * @brief Creates a game by loading its description from a text file.
 * @details See the file format description in @ref index.
 * @param filename input file
 * @return the loaded game
 **/
game game_load(char* filename);

/**
 * @brief Saves a game in a text file.
 * @details See the file format description in @ref index.
 * @param g game to save
 * @param filename output file
 **/
void game_save(cgame g, char* filename);

/**
 * @brief Computes the solution of a given game
 * @param g the game to solve
 * @details The game @p g is updated with the first solution found. If there are
 * no solution for this game, @p g must be unchanged.
 * @return true if a solution is found, false otherwise
 */
bool game_solve(game g);

/**
 * @brief Computes the total number of solutions of a given game.
 * @param g the game
 * @details The game @p g must be unchanged.
 * @return the number of solutions
 */
uint game_nb_solutions(cgame g);

/**
 * @brief Finds a first solution for the given game, using an optimized method.
 * @param s_rows the row we are currently on
 * @param s_cols the column we are currently on
 *
 * @return a solution to the given game g
 */
void g_solve_rec(game g, int s_rows, int s_cols);

/**
 * @brief Checks if there are no errors on the grid of the game
 *
 * @return true if there are no errors
 * @return false if there is an error
 */
bool no_error_whole_game(game g);

/**
 * @brief Finds the number of solutions of a given game.
 * @param count is a pointer that counts the number of solutions found
 * @param s_rows the row we are currently on
 * @param s_cols the column we are currently on
 *
 * @return the number of solution of to the given game g
 */
void game_nb_solutions_aux(uint* count, game g, int s_rows, int s_cols);

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution);



/**
 * @}
 */

#endif  // __GAME_TOOLS_H__