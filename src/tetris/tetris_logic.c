#include "inc/tetris_logic.h"

const FigurePattern figure_template[] = {
    [FIGURE_I] = {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_O] = {{{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_T] = {{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_L] = {{{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_J] = {{{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_S] = {{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    [FIGURE_Z] = {{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}};

bool spawn_next_figure(GameInfo_t *game_info, GameFigureData *figure_data) {
  if (!game_info || !figure_data || !game_info->next)
    return false;

  if (figure_data->current_figure) {
    free_matrix(figure_data->current_figure, FIGURE_SIZE);
    figure_data->current_figure = NULL;
  }

  figure_data->current_figure = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
  if (!figure_data->current_figure)
    return false;

  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      figure_data->current_figure[y][x] = game_info->next[y][x];
    }
  }

  figure_data->current_x = START_X;
  figure_data->current_y = 0;

  if (!can_place_figure(game_info->field, figure_data->current_figure,
                        figure_data->current_x, figure_data->current_y)) {
    free_matrix(figure_data->current_figure, FIGURE_SIZE);
    figure_data->current_figure = NULL;
    return false;
  }

  draw_figure(game_info, figure_data);
  generate_figure(game_info->next);
  return true;
}

void generate_figure(int **next) {
  if (!next)
    return;

  static bool initialized = false;
  if (!initialized) {
    srand(time(NULL));
    initialized = true;
  }

  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      next[y][x] = 0;
    }
  }

  FigureType type = rand() % 7;

  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      next[y][x] = figure_template[type].pattern[y][x];
    }
  }
}

void clear_lines(GameInfo_t *game_info) {
  if (!game_info)
    return;

  int lines_cleared = 0;
  int write_y = FIELD_HEIGHT - 1;

  for (int read_y = FIELD_HEIGHT - 1; read_y >= 0; read_y--) {
    bool line_full = true;

    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (game_info->field[read_y][x] != CELL_FIXED) {
        line_full = false;
        break;
      }
    }

    if (!line_full) {
      if (write_y != read_y) {
        memcpy(game_info->field[write_y], game_info->field[read_y],
               FIELD_WIDTH * sizeof(int));
      }
      write_y--;
    } else {
      lines_cleared++;
    }
  }

  for (int y = 0; y <= write_y; y++) {
    memset(game_info->field[y], CELL_EMPTY, FIELD_WIDTH * sizeof(int));
  }

  if (lines_cleared > 0) {
    game_info->score += calculate_score(lines_cleared);
    update_level(game_info);
  }
}

static bool is_cell_placable(int **field, int fx, int fy) {
  if (fx < 0 || fx >= FIELD_WIDTH || fy >= FIELD_HEIGHT)
    return false;

  return !(fy >= 0 && field[fy][fx] == CELL_FIXED);
}

bool can_place_figure(int **field, int **figure, int pos_x, int pos_y) {
  if (!field || !figure)
    return false;

  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (figure[y][x] && !is_cell_placable(field, pos_x + x, pos_y + y)) {
        return false;
      }
    }
  }
  return true;
}

int calculate_score(int lines_clear) {
  int score = 0;
  switch (lines_clear) {
  case 1:
    score = 100;
    break;
  case 2:
    score = 300;
    break;
  case 3:
    score = 700;
    break;
  case 4:
    score = 1500;
    break;
  default:
    score = 0;
  }
  return score;
}

void update_level(GameInfo_t *game_info) {
  int new_level = game_info->score / 600 + 1;

  if (new_level > MAX_LEVEL) {
    new_level = MAX_LEVEL;
  }

  if (new_level > game_info->level) {
    game_info->level = new_level;
    update_speed(game_info);
  }
}

void update_speed(GameInfo_t *game_info) {
  int new_speed = INITIAL_TIME - (game_info->level - 1) * SPEED_STEP;
  game_info->speed = (new_speed < MIN_SPEED) ? MIN_SPEED : new_speed;
}

void check_high_score(GameInfo_t *game_info) {
  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;
    update_high_score(game_info->high_score);
  }
}