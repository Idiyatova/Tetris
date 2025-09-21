#ifndef S21_TEST_H
#define S21_TEST_H

#include <check.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/db.h"
#include "../inc/defines.h"
#include "../inc/game_controller.h"
#include "../inc/tetris.h"
#include "../inc/tetris_logic.h"
#include "../inc/types.h"

extern GameInfo_t *game_info;

Suite *can_place_figure_suite(void);
Suite *generate_figure_suite(void);
Suite *spawn_figure_suite(void);
Suite *movement_suite(void);
Suite *rotation_suite(void);
Suite *attach_figure_suite(void);
Suite *state_machine_suite(void);
Suite *integration_suite(void);
Suite *db_suite(void);

// utils
GameInfo_t *create_test_game_info();
void test_free_game_info(GameInfo_t *info);
void fill_line(GameInfo_t *info, int line);
bool is_line_empty(GameInfo_t *info, int line);
GameInfo_t *create_game_over_test_case();
void create_obstacles(GameInfo_t *info, int x, int y);
bool is_figure_fixed(GameInfo_t *info, int x, int y, int **figure);
int **create_test_figure_pattern(const int pattern[FIGURE_SIZE][FIGURE_SIZE]);
void test_free_matrix(int **matrix, int rows);
bool table_exists();
int **create_filled_matrix(int height, int width, int fill_value);

#endif