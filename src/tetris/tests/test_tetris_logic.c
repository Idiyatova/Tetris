#include "tests.h"

START_TEST(test_clear_four_lines) {
  GameInfo_t *info = create_test_game_info();

  for (int i = FIELD_HEIGHT - 5; i < FIELD_HEIGHT - 1; i++) {
    fill_line(info, i);
  }

  clear_lines(info);

  for (int i = FIELD_HEIGHT - 5; i < FIELD_HEIGHT - 1; i++) {
    ck_assert_int_eq(is_line_empty(info, i), true);
  }

  ck_assert_int_eq(info->score, 1500);
  ck_assert_int_eq(info->level, 3);

  free_game_info(info);
}
END_TEST

START_TEST(test_level_increase_after_clearing) {
  GameInfo_t *info = create_test_game_info();
  info->score = 500;

  fill_line(info, FIELD_HEIGHT - 2);
  fill_line(info, FIELD_HEIGHT - 3);
  clear_lines(info);

  ck_assert_int_eq(info->level, 2);

  free_game_info(info);
}
END_TEST

START_TEST(test_level_max_10) {
  GameInfo_t *info = create_test_game_info();
  info->score = 5999;
  info->level = 10;

  fill_line(info, FIELD_HEIGHT - 2);
  clear_lines(info);

  ck_assert_int_eq(info->level, 10);
  ck_assert_int_eq(info->score, 6099);

  free_game_info(info);
}
END_TEST

START_TEST(test_speed_min) {
  GameInfo_t *info = create_test_game_info();
  info->score = 5400;
  info->level = 9;
  info->speed = MIN_SPEED + SPEED_STEP;

  for (int i = FIELD_HEIGHT - 5; i < FIELD_HEIGHT - 1; i++) {
    fill_line(info, i);
  }
  clear_lines(info);

  ck_assert_int_eq(info->speed, MIN_SPEED);
  ck_assert_int_eq(info->level, 10);

  free_game_info(info);
}
END_TEST

START_TEST(test_spawn_figure_normal) {
  GameInfo_t *info = create_test_game_info();

  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};

  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      info->next[y][x] = 0;
    }
  }

  bool result = spawn_next_figure(info, &figure_data);

  ck_assert_int_eq(result, true);

  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 0);

  ck_assert_ptr_nonnull(figure_data.current_figure);

  bool next_empty = true;
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (info->next[y][x] != 0) {
        next_empty = false;
        break;
      }
    }
  }
  ck_assert_int_eq(next_empty, false);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  free_game_info(info);
}
END_TEST

START_TEST(test_spawn_figure_no_space) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};

  for (int i = 0; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      info->field[i][j] = CELL_FIXED;
    }
  }

  int test_next[FIGURE_SIZE][FIGURE_SIZE] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  for (int y = 0; y < FIGURE_SIZE; ++y)
    for (int x = 0; x < FIGURE_SIZE; ++x)
      info->next[y][x] = test_next[y][x];

  bool result = spawn_next_figure(info, &figure_data);
  ck_assert_int_eq(result, false);
  ck_assert_ptr_null(figure_data.current_figure);

  free_game_info(info);
}
END_TEST

START_TEST(test_spawn_with_NULL_current_figure) {
  GameInfo_t *info = create_test_game_info();
  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);
  ck_assert_ptr_nonnull(info->next);

  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = -1, .current_y = -1};

  bool result = spawn_next_figure(info, &figure_data);

  ck_assert_int_eq(result, true);
  ck_assert_ptr_nonnull(figure_data.current_figure);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 0);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  free_game_info(info);
}
END_TEST

START_TEST(test_spawn_figure_blocked) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);
  ck_assert_ptr_nonnull(info->next);

  generate_figure(info->next);

  for (int y = 0; y <= FIELD_HEIGHT - 1; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      info->field[y][x] = CELL_FIXED;
    }
  }

  bool result = spawn_next_figure(info, &figure_data);
  ck_assert_int_eq(result, false);
  ck_assert_ptr_null(figure_data.current_figure);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  free_game_info(info);
}
END_TEST

START_TEST(test_generate_figure_initialization) {
  int **figure = create_filled_matrix(FIGURE_SIZE, FIGURE_SIZE, CELL_CURRENT);

  generate_figure(figure);

  int has_non_zero = 0;
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (figure[y][x] != 0) {
        has_non_zero = 1;
        break;
      }
    }
    if (has_non_zero)
      break;
  }
  ck_assert_int_eq(has_non_zero, 1);

  free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_generate_figure_clear_before_fill) {
  int **figure = create_filled_matrix(FIGURE_SIZE, FIGURE_SIZE, CELL_CURRENT);

  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      figure[y][x] = 9;
    }
  }

  generate_figure(figure);

  int has_non_zero_outside = 0;
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (figure[y][x] != 0 && figure[y][x] != 1) {
        has_non_zero_outside = 1;
        break;
      }
    }
    if (has_non_zero_outside)
      break;
  }
  ck_assert_int_eq(has_non_zero_outside, 0);

  free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_cannot_place_outside_left) {

  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  int figure_pattern[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result = can_place_figure(field, figure, -1, 0);
  ck_assert_int_eq(result, false);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_cannot_place_outside_right) {
  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  int figure_pattern[4][4] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result = can_place_figure(field, figure, FIELD_WIDTH - 3, 0);
  ck_assert_int_eq(result, false);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_cannot_place_outside_bottom) {
  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  int figure_pattern[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result = can_place_figure(field, figure, 0, FIELD_HEIGHT - 1);
  ck_assert_int_eq(result, false);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_cannot_place_over_fixed_blocks) {
  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  field[5][3] = CELL_FIXED;
  field[5][4] = CELL_FIXED;

  int figure_pattern[4][4] = {
      {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result = can_place_figure(field, figure, 3, 4);
  ck_assert_int_eq(result, false);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_can_place_near_fixed_blocks) {
  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  field[5][5] = CELL_FIXED;
  field[5][6] = CELL_FIXED;

  int figure_pattern[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result = can_place_figure(field, figure, 2, 5);
  ck_assert_int_eq(result, true);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

START_TEST(test_can_place_at_corners) {
  int **field = create_filled_matrix(FIELD_HEIGHT, FIELD_WIDTH, 0);

  int figure_pattern[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  int **figure = create_test_figure_pattern(figure_pattern);

  bool result1 = can_place_figure(field, figure, 0, 0);
  bool result2 = can_place_figure(field, figure, FIELD_WIDTH - 2, 0);
  bool result3 = can_place_figure(field, figure, 0, FIELD_HEIGHT - 2);
  bool result4 =
      can_place_figure(field, figure, FIELD_WIDTH - 2, FIELD_HEIGHT - 2);

  ck_assert_int_eq(result1, true);
  ck_assert_int_eq(result2, true);
  ck_assert_int_eq(result3, true);
  ck_assert_int_eq(result4, true);

  test_free_matrix(field, FIELD_HEIGHT);
  test_free_matrix(figure, FIGURE_SIZE);
}
END_TEST

Suite *spawn_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Spawn Next Figure");

  tc_core = tcase_create("Core of spawn");
  tcase_add_test(tc_core, test_spawn_figure_normal);
  tcase_add_test(tc_core, test_spawn_figure_no_space);
  tcase_add_test(tc_core, test_spawn_figure_blocked);
  tcase_add_test(tc_core, test_spawn_with_NULL_current_figure);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *generate_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Generate Figure");

  tc_core = tcase_create("Core 0f generate figure");
  tcase_add_test(tc_core, test_generate_figure_initialization);
  tcase_add_test(tc_core, test_generate_figure_clear_before_fill);

  suite_add_tcase(s, tc_core);

  return s;
}

Suite *can_place_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Generate Figure");

  tc_core = tcase_create("Core of place figure");
  tcase_add_test(tc_core, test_clear_four_lines);
  tcase_add_test(tc_core, test_level_increase_after_clearing);
  tcase_add_test(tc_core, test_level_max_10);
  tcase_add_test(tc_core, test_speed_min);
  tcase_add_test(tc_core, test_cannot_place_outside_left);
  tcase_add_test(tc_core, test_cannot_place_outside_right);
  tcase_add_test(tc_core, test_cannot_place_outside_bottom);
  tcase_add_test(tc_core, test_cannot_place_over_fixed_blocks);
  tcase_add_test(tc_core, test_can_place_near_fixed_blocks);
  tcase_add_test(tc_core, test_can_place_at_corners);

  suite_add_tcase(s, tc_core);

  return s;
}
