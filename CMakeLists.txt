cmake_minimum_required(VERSION 3.0)
project(lightup C)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall --coverage")

#folder copy

File(GLOB MY_PUBLIC_HEADERS
  "png/*"
)
file(COPY ${MY_PUBLIC_HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Game_files/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/build/)

# foreach(file "asteroid_0.png" "asteroid_1.png" "asteroid_2.png" "asteroid_3.png" "asteroid_4.png" "asteroid_u.png" "Babysun.png" "button_R.png" "button_S.png" "button_save.png" "button_Y.png" "button_Z.png" "mark_satellite.png" "space.png" "star.png" "rules.png" "grid.png" "arial.ttf")
#   file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# endforeach(data_file)

## use a CMake3 package to solve SDL2 dependencies
find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries

message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

include_directories(${SDL2_ALL_INC})

#Executable tests:
add_executable(game_text game_text.c)

add_executable(game_test_maaudrain game_test_maaudrain.c)
add_executable(game_test_abdouldiallo game_test_abdouldiallo.c)
add_executable(game_test_arledoux game_test_arledoux.c)
add_executable(game_tests_game_tools game_tests_game_tools.c)

add_executable(game_solve game_solve.c)

add_executable(game_sdl game_sdl.c main_sdl.c)

#Libraries:
add_library(game game.c game_aux.c queue.c game_tools.c)

target_link_libraries(game_text game) 
target_link_libraries(game_test_abdouldiallo game) 
target_link_libraries(game_test_maaudrain game) 
target_link_libraries(game_test_arledoux game)
target_link_libraries(game_tests_game_tools game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m game)


#Tests dummy
add_test(test_maaudrain_dummy ./game_test_maaudrain dummy)
add_test(test_abdouldiallo_dummy ./game_test_abdouldiallo dummy)
add_test(test_arledoux_dummy ./game_test_arledoux dummy)


#Tests des fonctions du groupe au S3(Audrain, Diallo, Ledoux)
add_test(test_maaudrain_game_new ./game_test_maaudrain game_new)
add_test(test_maaudrain_game_new_ext ./game_test_maaudrain game_new_ext)
add_test(test_maaudrain_game_new_empty ./game_test_maaudrain game_new_empty)
add_test(test_maaudrain_game_new_empty_ext ./game_test_maaudrain game_new_empty_ext)
add_test(test_maaudrain_game_copy ./game_test_maaudrain game_copy)
add_test(test_maaudrain_game_equal ./game_test_maaudrain game_equal)
add_test(test_maaudrain_game_delete ./game_test_maaudrain game_delete)
add_test(test_maaudrain_game_set_square ./game_test_maaudrain game_set_square)
add_test(test_maaudrain_game_get_square ./game_test_maaudrain game_get_square)
add_test(test_maaudrain_game_print ./game_test_maaudrain game_print)

add_test(test_abdouldiallo_game_sol ./game_test_abdouldiallo test_game_solution)
add_test(test_abdouldiallo_game_state ./game_test_abdouldiallo test_get_state)
add_test(test_abdouldiallo_game_flags ./game_test_abdouldiallo test_get_flags)
add_test(test_abdouldiallo_game_is_blank ./game_test_abdouldiallo test_is_blank)
add_test(test_abdouldiallo_game_is_lightbulb ./game_test_abdouldiallo test_is_lightbulb)
add_test(test_abdouldiallo_game_is_black ./game_test_abdouldiallo test_is_black)
add_test(test_abdouldiallo_game_black_number ./game_test_abdouldiallo test_get_black_number)
add_test(test_abdouldiallo_game_is_marked ./game_test_abdouldiallo test_is_marked)
add_test(test_abdouldiallo_game_undo ./game_test_abdouldiallo test_undo)
add_test(test_abdouldiallo_game_redo ./game_test_abdouldiallo test_redo)

add_test(test_arledoux_game_is_lighted ./game_test_arledoux game_is_lighted)
add_test(test_arledoux_game_has_error ./game_test_arledoux game_has_error)
add_test(test_arledoux_game_check_move ./game_test_arledoux game_check_move)
add_test(test_arledoux_game_play_move ./game_test_arledoux game_play_move)
add_test(test_arledoux_game_update_flags ./game_test_arledoux game_update_flags)
add_test(test_arledoux_game_is_over ./game_test_arledoux game_is_over)
add_test(test_arledoux_game_restart ./game_test_arledoux game_restart)
add_test(test_arledoux_game_default ./game_test_arledoux game_default)
add_test(test_arledoux_game_nb_rows ./game_test_arledoux game_nb_rows)
add_test(test_arledoux_game_nb_cols ./game_test_arledoux game_nb_cols)
add_test(test_arledoux_game_is_wrapping ./game_test_arledoux game_is_wrapping)

#Tests fonctions S4
add_test(test_game_load ./game_tests_game_tools game_load)
add_test(test_game_save ./game_tests_game_tools game_save)
add_test(test_game_solve ./game_tests_game_tools game_solve)
add_test(test_game_nb_sols ./game_tests_game_tools game_nb_sols)
