#include "tests.h"

START_TEST(test_full_game_cycle) {
  GameInfo_t *info = create_test_game_info();
  GameState state = GAME_START;

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_SPAWN);

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_MOVING);

  handle_state_machine(&state, Right, info);
  ck_assert_int_eq(state, GAME_MOVING);

  handle_state_machine(&state, Action, info);
  ck_assert_int_eq(state, GAME_MOVING);

  while (state == GAME_MOVING) {
    handle_state_machine(&state, Down, info);
  }

  ck_assert_int_eq(state, GAME_ATTACHED);

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_SPAWN);

  test_free_game_info(info);
}
END_TEST

START_TEST(test_line_clear_integration) {
  GameInfo_t *info = create_test_game_info();
  int TEST_FIGURE_I[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};

  for (int y = 5; y <= 8; y++) {
    for (int x = 0; x < FIELD_WIDTH - 1; x++) {
      info->field[y][x] = CELL_FIXED;
    }
  }
  GameFigureData figure_data = {
      .current_figure = NULL, .current_x = 9, .current_y = 2};
  figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_I);

  attach_figure(info, &figure_data);

  clear_lines(info);

  ck_assert_int_eq(info->score, 100);
  ck_assert_int_eq(info->level, 1);

  bool has_fixed_cells = false;
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (info->field[y][x] == CELL_FIXED) {
        has_fixed_cells = true;
        break;
      }
    }
    if (has_fixed_cells)
      break;
  }
  ck_assert_int_eq(has_fixed_cells, true);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_game) {
  GameInfo_t *info = create_test_game_info();
  int TEST_FIGURE_I[4][4] = {
      {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}};

  for (int lines = 0; lines < 6; lines++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      info->field[FIELD_HEIGHT - 2][x] = CELL_FIXED;
    }

    GameFigureData figure_data = {
        .current_figure = NULL, .current_x = 0, .current_y = FIELD_HEIGHT - 3};
    figure_data.current_figure = create_test_figure_pattern(TEST_FIGURE_I);

    attach_figure(info, &figure_data);
    clear_lines(info);
    update_level(info);
    update_speed(info);

    test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  }

  ck_assert_int_eq(info->score, 600);
  ck_assert_int_eq(info->level, 2);
  ck_assert_int_lt(info->speed, INITIAL_TIME);

  test_free_game_info(info);
}
END_TEST

START_TEST(test_userInput_init) {

  userInput(Start, false);

  GameInfo_t info = updateCurrentState();
  ck_assert_ptr_nonnull(info.field);
  ck_assert_ptr_nonnull(info.next);

  cleanup_game();
}
END_TEST

START_TEST(test_userInput_multiple_calls) {
  userInput(Start, false);
  userInput(Left, false);
  userInput(Right, false);
  GameInfo_t info = updateCurrentState();
  ck_assert_int_eq(info.pause, false);

  cleanup_game();
}
END_TEST

START_TEST(test_cleanup_game) {
  userInput(Start, false);

  cleanup_game();

  GameInfo_t info = updateCurrentState();
  ck_assert_ptr_null(info.field);
  ck_assert_ptr_null(info.next);
}
END_TEST

START_TEST(test_updateCurrentState_uninitialized) {
  cleanup_game();
  GameInfo_t info = updateCurrentState();
  ck_assert_ptr_null(info.field);
  ck_assert_ptr_null(info.next);
  ck_assert_int_eq(info.score, 0);
}
END_TEST

Suite *integration_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Integration Tests");

  tc_core = tcase_create("Core Gameplay");
  tcase_add_test(tc_core, test_full_game_cycle);
  tcase_add_test(tc_core, test_line_clear_integration);
  tcase_add_test(tc_core, test_userInput_init);
  tcase_add_test(tc_core, test_game);
  tcase_add_test(tc_core, test_userInput_multiple_calls);
  tcase_add_test(tc_core, test_cleanup_game);
  tcase_add_test(tc_core, test_updateCurrentState_uninitialized);
  suite_add_tcase(s, tc_core);
  return s;
}