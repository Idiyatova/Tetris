#include "tests.h"

GameInfo_t *create_test_game_info() {
  GameInfo_t *info = malloc(sizeof(GameInfo_t));
  if (!info)
    return NULL;

  info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  info->next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);

  if (!info->field || !info->next) {
    test_free_game_info(info);
    return NULL;
  }

  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      info->field[y][x] = CELL_EMPTY;
    }
  }

  info->score = 0;
  info->high_score = 0;
  info->level = 1;
  info->speed = INITIAL_TIME;
  info->pause = false;

  return info;
}

void test_free_game_info(GameInfo_t *info) {
  if (!info)
    return;

  if (info->field) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
      if (info->field[i]) {
        free(info->field[i]);
        info->field[i] = NULL;
      }
    }
    free(info->field);
    info->field = NULL;
  }

  if (info->next) {
    for (int i = 0; i < FIGURE_SIZE; ++i) {
      if (info->next[i]) {
        free(info->next[i]);
        info->next[i] = NULL;
      }
    }
    free(info->next);
    info->next = NULL;
  }

  free(info);
}

void fill_line(GameInfo_t *info, int line) {
  for (int j = 0; j < FIELD_WIDTH; j++) {
    info->field[line][j] = CELL_FIXED;
  }
}

bool is_line_empty(GameInfo_t *info, int line) {
  for (int j = 0; j < FIELD_WIDTH; j++) {
    if (info->field[line][j] != CELL_EMPTY) {
      return false;
    }
  }
  return true;
}

void create_obstacles(GameInfo_t *info, int x, int y) {
  info->field[y][x] = CELL_FIXED;
}

int **create_filled_matrix(int rows, int cols, int fill_value) {
  int **matrix = create_matrix(rows, cols);
  if (!matrix)
    return NULL;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      matrix[i][j] = fill_value;
    }
  }

  return matrix;
}

int **create_test_figure_pattern(const int pattern[FIGURE_SIZE][FIGURE_SIZE]) {
  int **figure = create_filled_matrix(FIGURE_SIZE, FIGURE_SIZE, 0);
  if (!figure)
    return NULL;

  for (int i = 0; i < FIGURE_SIZE; ++i) {
    memcpy(figure[i], pattern[i], FIGURE_SIZE * sizeof(int));
  }

  return figure;
}

void test_free_matrix(int **matrix, int rows) {
  if (!matrix)
    return;
  for (int i = 0; i < rows; ++i) {
    if (matrix[i])
      free(matrix[i]);
  }
  free(matrix);
}

bool table_exists() {
  sqlite3 *db;
  int rc = sqlite3_open(DB_PATH, &db);
  if (rc != SQLITE_OK)
    return false;

  const char *sql =
      "SELECT name FROM sqlite_master WHERE type='table' AND name='scores';";
  sqlite3_stmt *stmt;

  bool exists = false;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      exists = true;
    }
    sqlite3_finalize(stmt);
  }

  sqlite3_close(db);
  return exists;
}
