#include "tests.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, spawn_figure_suite());
  srunner_add_suite(sr, generate_figure_suite());
  srunner_add_suite(sr, can_place_figure_suite());
  srunner_add_suite(sr, spawn_figure_suite());
  srunner_add_suite(sr, movement_suite());
  srunner_add_suite(sr, rotation_suite());
  srunner_add_suite(sr, attach_figure_suite());
  srunner_add_suite(sr, state_machine_suite());
  srunner_add_suite(sr, integration_suite());
  srunner_add_suite(sr, db_suite());
  srunner_run_all(sr, CK_NORMAL);

  int failed_count = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
