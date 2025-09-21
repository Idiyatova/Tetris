#include "tests.h"

const int TEST_FIGURE_I[4][4] = {
    {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_O[4][4] = {
    {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_T[4][4] = {
    {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_L[FIGURE_SIZE][FIGURE_SIZE] = {
    {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_J[4][4] = {
    {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_S[4][4] = {
    {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

const int TEST_FIGURE_Z[4][4] = {
    {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

bool figures_equal(int **a, const int b[4][4]) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (a[y][x] != b[y][x]) {
        return false;
      }
    }
  }
  return true;
}

START_TEST(test_rotate_I_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_I);

  const int expected[4][4][4] = {
      {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}},
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
      {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_T_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_T);

  const int expected_T[4][4][4] = {
      {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}, // 90°
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}}, // 180°
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}}, // 270°
      {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected_T[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_L_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  const int expected[4][4][4] = {
      {{0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}},
      {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_J_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_J);

  const int expected[4][4][4] = {
      {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 1, 0}},
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_S_full_cycle) {
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  GameInfo_t *info = create_test_game_info();
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_S);

  const int expected[4][4][4] = {
      {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}},
      {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}},
      {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_Z_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_Z);

  const int expected[4][4][4] = {
      {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}},
      {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}},
      {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_O_full_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_O);

  const int expected[4][4][4] = {

      {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 0, 1, 1}},
      {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}},
      {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  for (int i = 0; i < 4; i++) {
    bool result = try_rotate(info, &figure_data);
    ck_assert_int_eq(result, true);

    for (int y = 0; y < FIGURE_SIZE; y++) {
      for (int x = 0; x < FIGURE_SIZE; x++) {
        ck_assert_int_eq(figure_data.current_figure[y][x], expected[i][y][x]);
      }
    }
  }

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_near_left_boundary) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = 1, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  const int original_L[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

  bool result = try_rotate(info, &figure_data);

  ck_assert_int_eq(result, true);
  ck_assert(!figures_equal(figure_data.current_figure, original_L));

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_near_right_boundary) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = FIELD_WIDTH - 3, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_J);

  const int original_J[4][4] = {
      {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

  bool result = try_rotate(info, &figure_data);

  ck_assert_int_eq(result, true);
  ck_assert(!figures_equal(figure_data.current_figure, original_J));

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_rotate_blocked_by_obstacle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_T);

  info->field[figure_data.current_y][figure_data.current_x + 2] = CELL_FIXED;
  info->field[figure_data.current_y + 1][figure_data.current_x + 3] =
      CELL_FIXED;
  info->field[figure_data.current_y + 2][figure_data.current_x + 1] =
      CELL_FIXED;
  info->field[figure_data.current_y + 2][figure_data.current_x + 2] =
      CELL_FIXED;

  const int original_T[4][4] = {
      {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  bool result = try_rotate(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert(figures_equal(figure_data.current_figure, original_T));

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_left_normal) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  bool result = move_figure_left(info, &figure_data);

  ck_assert_int_eq(result, true);
  ck_assert_int_eq(figure_data.current_x, START_X - 1);
  ck_assert_int_eq(figure_data.current_y, 0);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_left_boundary) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = 0, .current_y = 0};

  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  bool result = move_figure_left(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, 0);
  ck_assert_int_eq(figure_data.current_y, 0);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_left_obstacle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  create_obstacles(info, figure_data.current_x - 1, figure_data.current_y + 2);

  bool result = move_figure_left(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 5);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_right_normal) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  bool result = move_figure_right(info, &figure_data);

  ck_assert_int_eq(result, true);
  ck_assert_int_eq(figure_data.current_x, START_X + 1);
  ck_assert_int_eq(figure_data.current_y, 0);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_right_boundary) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = FIELD_WIDTH - 2, .current_y = 0};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  bool result = move_figure_right(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, FIELD_WIDTH - 2);
  ck_assert_int_eq(figure_data.current_y, 0);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_right_obstacle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  create_obstacles(info, figure_data.current_x + 2, figure_data.current_y + 2);

  bool result = move_figure_right(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 5);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_down_normal) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {0};

  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);
  figure_data.current_x = START_X;
  figure_data.current_y = 0;

  draw_figure(info, &figure_data);

  bool result = move_figure_down(info, &figure_data);
  ck_assert_int_eq(result, true);
  ck_assert_int_eq(figure_data.current_y, 1);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_down_boundary) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {.current_figure = NULL,
                                .current_x = START_X,
                                .current_y = FIELD_HEIGHT - 3};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  bool result = move_figure_down(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, FIELD_HEIGHT - 3);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_down_obstacle) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  // препятствие снизу
  create_obstacles(info, figure_data.current_x, figure_data.current_y + 3);
  create_obstacles(info, figure_data.current_x + 1, figure_data.current_y + 3);

  bool result = move_figure_down(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 5);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_move_down_fixed_figure) {
  GameInfo_t *info = create_test_game_info();
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure_data.current_figure[i][j])
        figure_data.current_figure[i][j] = CELL_FIXED;
    }
  }

  bool result = move_figure_down(info, &figure_data);

  ck_assert_int_eq(result, false);
  ck_assert_int_eq(figure_data.current_x, START_X);
  ck_assert_int_eq(figure_data.current_y, 5);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_attach_figure_normal) {
  GameInfo_t *info = create_test_game_info();
  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);
  ck_assert_ptr_nonnull(info->next);

  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 5};

  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);
  ck_assert_ptr_nonnull(figure_data.current_figure);

  int figure_positions[FIGURE_SIZE * FIGURE_SIZE][2];
  int pos_count = 0;

  for (int fy = 0; fy < FIGURE_SIZE; fy++) {
    for (int fx = 0; fx < FIGURE_SIZE; fx++) {
      if (figure_data.current_figure[fy][fx]) {
        int check_y = figure_data.current_y + fy;
        int check_x = figure_data.current_x + fx;

        if (check_y >= 0 && check_y < FIELD_HEIGHT && check_x >= 0 &&
            check_x < FIELD_WIDTH) {
          figure_positions[pos_count][0] = check_y;
          figure_positions[pos_count][1] = check_x;
          pos_count++;

          ck_assert_int_eq(info->field[check_y][check_x], CELL_EMPTY);
        }
      }
    }
  }

  attach_figure(info, &figure_data);
  ck_assert_ptr_null(figure_data.current_figure);

  for (int i = 0; i < pos_count; i++) {
    int y = figure_positions[i][0];
    int x = figure_positions[i][1];
    ck_assert_int_eq(info->field[y][x], CELL_FIXED);
  }

  test_free_game_info(info);
}
END_TEST

START_TEST(test_attach_figure_bottom) {
  GameInfo_t *info = create_test_game_info();
  ck_assert_ptr_nonnull(info);
  ck_assert_ptr_nonnull(info->field);

  GameFigureData figure_data = {.current_figure = NULL,
                                .current_x = START_X,
                                .current_y = FIELD_HEIGHT - 3};

  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_L);
  ck_assert_ptr_nonnull(figure_data.current_figure);

  typedef struct {
    int y;
    int x;
  } Position;
  Position figure_cells[FIGURE_SIZE * FIGURE_SIZE];
  int cell_count = 0;

  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (figure_data.current_figure[y][x]) {
        int field_y = figure_data.current_y + y;
        int field_x = figure_data.current_x + x;

        if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
            field_x < FIELD_WIDTH) {
          figure_cells[cell_count].y = field_y;
          figure_cells[cell_count].x = field_x;
          cell_count++;
          ck_assert_int_eq(info->field[field_y][field_x], CELL_EMPTY);
        }
      }
    }
  }
  attach_figure(info, &figure_data);

  ck_assert_ptr_null(figure_data.current_figure);

  for (int i = 0; i < cell_count; i++) {
    Position pos = figure_cells[i];
    ck_assert_int_eq(info->field[pos.y][pos.x], CELL_FIXED);
  }

  test_free_game_info(info);
}
END_TEST

Suite *movement_suite(void) {
  Suite *s;
  TCase *tc_left, *tc_right, *tc_down;

  s = suite_create("Figure Movement");

  tc_left = tcase_create("Move Left");
  tcase_add_test(tc_left, test_move_left_normal);
  tcase_add_test(tc_left, test_move_left_boundary);
  tcase_add_test(tc_left, test_move_left_obstacle);
  suite_add_tcase(s, tc_left);

  tc_right = tcase_create("Move Right");
  tcase_add_test(tc_right, test_move_right_normal);
  tcase_add_test(tc_right, test_move_right_boundary);
  tcase_add_test(tc_right, test_move_right_obstacle);
  suite_add_tcase(s, tc_right);

  tc_down = tcase_create("Move Down");
  tcase_add_test(tc_down, test_move_down_normal);
  tcase_add_test(tc_down, test_move_down_boundary);
  tcase_add_test(tc_down, test_move_down_obstacle);
  tcase_add_test(tc_down, test_move_down_fixed_figure);
  suite_add_tcase(s, tc_down);

  return s;
}

Suite *attach_figure_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Attach Figure");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_attach_figure_normal);
  tcase_add_test(tc_core, test_attach_figure_bottom);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *rotation_suite(void) {
  Suite *s;
  TCase *tc_basic, *tc_boundary, *tc_obstacles;

  s = suite_create("Figure Rotation");

  // Базовые повороты
  tc_basic = tcase_create("Basic Rotation");
  tcase_add_test(tc_basic, test_rotate_I_full_cycle);
  tcase_add_test(tc_basic, test_rotate_O_full_cycle);
  tcase_add_test(tc_basic, test_rotate_T_full_cycle);
  tcase_add_test(tc_basic, test_rotate_L_full_cycle);
  tcase_add_test(tc_basic, test_rotate_J_full_cycle);
  tcase_add_test(tc_basic, test_rotate_S_full_cycle);
  tcase_add_test(tc_basic, test_rotate_Z_full_cycle);
  suite_add_tcase(s, tc_basic);

  tc_boundary = tcase_create("Boundary Cases");
  tcase_add_test(tc_boundary, test_rotate_near_left_boundary);
  tcase_add_test(tc_boundary, test_rotate_near_right_boundary);
  suite_add_tcase(s, tc_boundary);

  tc_obstacles = tcase_create("With Obstacles");
  tcase_add_test(tc_obstacles, test_rotate_blocked_by_obstacle);
  suite_add_tcase(s, tc_obstacles);

  return s;
}