#include "inc/game_controller.h"

void handle_movement(UserAction_t action, GameState *state,
                     GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data || !state)
    return;

  switch (action) {
  case Left:
    move_figure_left(game_info, figure_data);
    break;
  case Right:
    move_figure_right(game_info, figure_data);
    break;
  case Down:
    if (!move_figure_down(game_info, figure_data)) {
      *state = GAME_ATTACHED;
    }
    break;
  case Action:
    try_rotate(game_info, figure_data);
    break;
  default:
    break;
  }
}

void rotate_matrix(int **src, int **dst) {

  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      dst[x][FIGURE_SIZE - 1 - y] = src[y][x];
    }
  }
}

void copy_matrix(int **src, int **dst) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      dst[y][x] = src[y][x];
    }
  }
}

static bool process_figure_cell(GameInfo_t *game_info,
                                const GameFigureData *figure_data, int y, int x,
                                int cell_value, int target_value,
                                bool check_fixed) {
  int field_y = figure_data->current_y + y;
  int field_x = figure_data->current_x + x;

  if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
      field_x < FIELD_WIDTH) {

    if (check_fixed && game_info->field[field_y][field_x] == CELL_FIXED) {
      return false;
    }

    if (cell_value) {
      game_info->field[field_y][field_x] = target_value;
      return true;
    }
  }
  return false;
}

static bool process_figure(GameInfo_t *game_info,
                           const GameFigureData *figure_data, int target_value,
                           bool check_fixed) {
  if (!game_info || !figure_data || !figure_data->current_figure ||
      !game_info->field)
    return false;

  bool status = false;
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      status = process_figure_cell(game_info, figure_data, y, x,
                                   figure_data->current_figure[y][x],
                                   target_value, check_fixed);
    }
  }
  return status;
}

void attach_figure(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data || !figure_data->current_figure ||
      !game_info->field) {
    return;
  }

  process_figure(game_info, figure_data, CELL_FIXED, false);

  if (figure_data->current_figure) {
    free_matrix(figure_data->current_figure, FIGURE_SIZE);
    figure_data->current_figure = NULL;
  }
}

bool draw_figure(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data || !figure_data->current_figure)
    return false;

  return process_figure(game_info, figure_data, CELL_CURRENT, true);
}

bool free_figure(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data || !figure_data->current_figure)
    return false;

  return process_figure(game_info, figure_data, CELL_EMPTY, false);
}

bool is_figure_active(const GameFigureData *figure_data) {
  if (!figure_data || !figure_data->current_figure)
    return false;

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure_data->current_figure[i][j] != 0 &&
          figure_data->current_figure[i][j] != CELL_CURRENT) {
        return false;
      }
    }
  }
  return true;
}

bool move_figure_left(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!is_figure_active(figure_data))
    return false;

  bool can_move = false;
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (figure_data->current_figure[y][x] &&
          (figure_data->current_x + x - 1) >= 0) {
        can_move = true;
        break;
      }
    }
    if (can_move)
      break;
  }

  if (!can_move)
    return false;

  if (can_place_figure(game_info->field, figure_data->current_figure,
                       figure_data->current_x - 1, figure_data->current_y)) {
    free_figure(game_info, figure_data);
    (figure_data->current_x)--;
    draw_figure(game_info, figure_data);
    return true;
  }
  return false;
}

bool move_figure_right(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!is_figure_active(figure_data))
    return false;

  int new_x = figure_data->current_x + 1;
  int new_y = figure_data->current_y;

  if (new_x >= FIELD_WIDTH)
    return false;

  if (can_place_figure(game_info->field, figure_data->current_figure, new_x,
                       new_y)) {
    free_figure(game_info, figure_data);
    figure_data->current_x = new_x;
    figure_data->current_y = new_y;
    draw_figure(game_info, figure_data);
    return true;
  }

  return false;
}

bool move_figure_down(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!is_figure_active(figure_data))
    return false;

  int new_x = figure_data->current_x;
  int new_y = figure_data->current_y + 1;

  if (new_y >= FIELD_HEIGHT)
    return false;

  if (can_place_figure(game_info->field, figure_data->current_figure, new_x,
                       new_y)) {
    free_figure(game_info, figure_data);
    figure_data->current_x = new_x;
    figure_data->current_y = new_y;
    draw_figure(game_info, figure_data);
    return true;
  }

  return false;
}

bool try_rotate(GameInfo_t *game_info, GameFigureData *figure_data) {

  int **temp_figure = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  if (!temp_figure)
    return false;

  rotate_matrix(figure_data->current_figure, temp_figure);

  const int kick_table[5][2] = {
      {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1},
  };

  for (int i = 0; i < 5; i++) {
    int new_x = figure_data->current_x + kick_table[i][0];
    int new_y = figure_data->current_y + kick_table[i][1];

    if (can_place_figure(game_info->field, temp_figure, new_x, new_y)) {

      free_figure(game_info, figure_data);
      copy_matrix(temp_figure, figure_data->current_figure);
      figure_data->current_x = new_x;
      figure_data->current_y = new_y;
      draw_figure(game_info, figure_data);

      free_matrix(temp_figure, FIGURE_SIZE);
      return true;
    }
  }

  free_matrix(temp_figure, FIGURE_SIZE);
  return false;
}
