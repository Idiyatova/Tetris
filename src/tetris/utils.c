#include "inc/utils.h"

bool init_game(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data)
    return false;

  if (!game_info->field) {
    game_info->field = create_matrix(FIELD_HEIGHT, FIELD_WIDTH);
    if (!game_info->field)
      return false;
  }

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      game_info->field[y][x] = CELL_EMPTY;
    }
  }

  if (!game_info->next) {
    game_info->next = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
    if (!game_info->next) {
      if (!game_info->field) {
        free_matrix(game_info->field, FIELD_HEIGHT);
      }
      return false;
    }
  }

  figure_data->current_figure = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  // if (!*current_figure_ptr) {
  //   free_matrix(game_info->field, FIELD_HEIGHT);
  //   free_matrix(game_info->next, FIGURE_SIZE);
  //   return false;
  // }

  generate_figure(game_info->next);

  game_info->score = 0;
  game_info->level = 1;
  game_info->speed = INITIAL_TIME;
  game_info->pause = false;
  game_info->high_score = get_high_score();

  return true;
}

void free_game_info(GameInfo_t *game_info) {
  if (!game_info)
    return;

  if (game_info->field) {
    free_matrix(game_info->field, FIELD_HEIGHT);
    game_info->field = NULL;
  }

  if (game_info->next) {
    free_matrix(game_info->next, FIGURE_SIZE);
    game_info->next = NULL;
  }
}

int **create_matrix(int rows, int cols) {
  if (rows <= 0 || cols <= 0)
    return NULL;

  int **matrix = malloc(rows * sizeof(int *));
  if (!matrix)
    return NULL;

  for (int i = 0; i < rows; ++i) {
    matrix[i] = calloc(cols, sizeof(int));
    // if (!matrix[i]) {
    //      for (int j = 0; j < i; ++j) {
    //     free(matrix[j]);
    //   }
    //   free(matrix);
    //   return NULL;
    // }
  }

  return matrix;
}

void free_matrix(int **matrix, int rows) {
  if (!matrix)
    return;

  for (int i = 0; i < rows; ++i) {
    if (matrix[i]) {
      free(matrix[i]);
      matrix[i] = NULL;
    }
  }

  free(matrix);
}

bool check_game_over(GameInfo_t *game_info) {
  if (!game_info || !game_info->field) {
    return true;
  }

  for (int col = 0; col < FIELD_WIDTH; col++) {
    if (game_info->field[0][col] == CELL_FIXED) {
      return true;
    }
  }
  return false;
}