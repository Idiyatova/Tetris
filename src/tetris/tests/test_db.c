#include "tests.h"

START_TEST(test_init_database_creates_table) {
  remove(DB_PATH);

  init_database();
  ck_assert(table_exists());
}
END_TEST

START_TEST(test_get_high_score_returns_initial_value) {
  remove(DB_PATH);
  init_database();

  int score = get_high_score();
  ck_assert_int_eq(score, 0);
}
END_TEST

START_TEST(test_update_high_score_sets_new_value) {
  remove(DB_PATH);
  init_database();

  update_high_score(500);
  int score = get_high_score();
  ck_assert_int_eq(score, 500);
}
END_TEST

START_TEST(test_update_high_score_large_value) {
  remove(DB_PATH);
  init_database();

  update_high_score(1000);
  int score = get_high_score();
  ck_assert_int_eq(score, 1000);
}
END_TEST

START_TEST(test_update_high_score_ignores_errors) {
  remove(DB_PATH);

  update_high_score(700);
  ck_assert(true);
}
END_TEST

Suite *db_suite(void) {
  Suite *s = suite_create("Tetris DB");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_init_database_creates_table);
  tcase_add_test(tc_core, test_get_high_score_returns_initial_value);
  tcase_add_test(tc_core, test_update_high_score_sets_new_value);
  tcase_add_test(tc_core, test_update_high_score_large_value);
  tcase_add_test(tc_core, test_update_high_score_ignores_errors);

  suite_add_tcase(s, tc_core);

  return s;
}
