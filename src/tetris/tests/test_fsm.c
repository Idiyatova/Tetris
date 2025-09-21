#include "tests.h"

const int TEST_L[4][4] = {
    {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};

START_TEST(test_start_to_spawn) {
  GameState state = GAME_START;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Start, info);

  ck_assert_int_eq(state, GAME_SPAWN);
  free_game_info(info);
}
END_TEST

START_TEST(test_moving_to_attached_at_bottom) {
  GameState state = GAME_START;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Start, info);
  handle_state_machine(&state, Start, info);

  ck_assert_int_eq(state, GAME_MOVING);

  for (int i = 0; i < FIELD_HEIGHT + FIGURE_SIZE; i++) {
    UserAction_t action = Down;
    handle_state_machine(&state, action, info);

    if (state != GAME_MOVING)
      break;
  }

  ck_assert_int_eq(state, GAME_ATTACHED);

  free_game_info(info);
}
END_TEST

START_TEST(test_pause_unpause) {
  GameState state = GAME_MOVING;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Pause, info);
  ck_assert_int_eq(state, GAME_PAUSED);

  handle_state_machine(&state, Pause, info);
  ck_assert_int_eq(state, GAME_MOVING);

  free_game_info(info);
}
END_TEST

START_TEST(test_game_over_condition) {
  GameState initial_states[] = {GAME_SPAWN, GAME_MOVING, GAME_PAUSED};

  for (size_t i = 0; i < sizeof(initial_states) / sizeof(initial_states[0]);
       i++) {
    GameState state = initial_states[i];
    GameInfo_t *info = create_test_game_info();

    handle_state_machine(&state, Terminate, info);

    ck_assert_int_eq(state, GAME_OVER);

    free_game_info(info);
  }
}
END_TEST

START_TEST(test_spawn_to_moving) {
  GameState state = GAME_START;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_SPAWN);

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_MOVING);

  free_game_info(info);
}
END_TEST

START_TEST(test_attached_to_spawn) {
  GameState state = GAME_ATTACHED;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Start, info);

  ck_assert_int_eq(state, GAME_SPAWN);
  free_game_info(info);
}
END_TEST

START_TEST(test_handle_state_machine_null_pointers) {
  GameState state = GAME_START;
  GameInfo_t game_info = {0};

  handle_state_machine(NULL, Start, &game_info);
  ck_assert_int_eq(state, GAME_START);

  handle_state_machine(&state, Start, NULL);
  ck_assert_int_eq(state, GAME_START);
}
END_TEST

START_TEST(test_movement_actions) {
  GameState state = GAME_MOVING;
  GameInfo_t *info = create_test_game_info();
  ck_assert_ptr_nonnull(info);

  GameFigureData figure_data = {.current_figure =
                                    create_test_figure_pattern(TEST_L),
                                .current_x = START_X,
                                .current_y = 5};
  ck_assert_ptr_nonnull(figure_data.current_figure);

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      info->field[y][x] = CELL_EMPTY;
    }
  }

  draw_figure(info, &figure_data);

  int initial_x = figure_data.current_x;
  int initial_y = figure_data.current_y;

  handle_movement(Left, &state, info, &figure_data);
  ck_assert_int_eq(state, GAME_MOVING);
  ck_assert_int_lt(figure_data.current_x, initial_x);

  handle_movement(Right, &state, info, &figure_data);
  ck_assert_int_eq(state, GAME_MOVING);
  ck_assert_int_gt(figure_data.current_x, initial_x - 1);

  handle_movement(Action, &state, info, &figure_data);
  ck_assert_int_eq(state, GAME_MOVING);

  handle_movement(Down, &state, info, &figure_data);
  ck_assert(state == GAME_MOVING || state == GAME_ATTACHED);
  ck_assert_int_gt(figure_data.current_y, initial_y - 1);

  test_free_matrix(figure_data.current_figure, FIGURE_SIZE);
  test_free_game_info(info);
}
END_TEST

START_TEST(test_check_game_over_sets_game_over) {
  GameInfo_t *info = malloc(sizeof(GameInfo_t));
  info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);

  for (int x = 0; x < FIELD_WIDTH; ++x) {
    info->field[0][x] = CELL_FIXED;
  }

  GameState state = GAME_SPAWN;
  handle_state_machine(&state, -1, info);

  ck_assert_int_eq(state, GAME_OVER);

  free_game_info(info);
}
END_TEST

START_TEST(test_game_spawn_stays_on_valid_field) {
  GameInfo_t *info = malloc(sizeof(GameInfo_t));
  info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  info->next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);

  GameState state = GAME_SPAWN;
  handle_state_machine(&state, -1, info);

  ck_assert_int_eq(state, GAME_MOVING);

  free_game_info(info);
}
END_TEST

START_TEST(test_game_over_to_start_reset) {
  GameState state = GAME_OVER;
  GameInfo_t *info = create_test_game_info();

  handle_state_machine(&state, Start, info);
  ck_assert_int_eq(state, GAME_START);

  ck_assert_int_eq(info->score, 0);
  ck_assert_int_eq(info->level, 1);
  ck_assert_int_eq(info->speed, INITIAL_TIME);

  test_free_game_info(info);
}
END_TEST

START_TEST(test_game_over_sets_new_high_score) {
  init_database();

  GameInfo_t *info = malloc(sizeof(GameInfo_t));
  info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  info->next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  info->score = 500;
  info->high_score = get_high_score();

  GameState state = GAME_OVER;
  handle_state_machine(&state, -1, info);

  ck_assert_int_eq(state, GAME_OVER);
  ck_assert_int_gt(get_high_score(), 0);

  free_game_info(info);
}
END_TEST

START_TEST(test_game_over_does_not_update_high_score) {
  init_database();
  update_high_score(1000);
  GameInfo_t *info = malloc(sizeof(GameInfo_t));
  info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  info->next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  info->score = 500;
  info->high_score = get_high_score();

  GameState state = GAME_OVER;
  handle_state_machine(&state, -1, info);
  ck_assert_int_eq(state, GAME_OVER);
  ck_assert_int_eq(get_high_score(), 1000);
  free_game_info(info);
}
END_TEST

Suite *state_machine_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("State Machine");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_start_to_spawn);
  tcase_add_test(tc_core, test_spawn_to_moving);
  tcase_add_test(tc_core, test_moving_to_attached_at_bottom);
  tcase_add_test(tc_core, test_attached_to_spawn);
  tcase_add_test(tc_core, test_pause_unpause);
  tcase_add_test(tc_core, test_game_over_condition);
  tcase_add_test(tc_core, test_handle_state_machine_null_pointers);
  tcase_add_test(tc_core, test_movement_actions);
  tcase_add_test(tc_core, test_check_game_over_sets_game_over);
  tcase_add_test(tc_core, test_game_spawn_stays_on_valid_field);
  tcase_add_test(tc_core, test_game_over_to_start_reset);
  tcase_add_test(tc_core, test_game_over_sets_new_high_score);
  tcase_add_test(tc_core, test_game_over_does_not_update_high_score);
  suite_add_tcase(s, tc_core);

  return s;
}