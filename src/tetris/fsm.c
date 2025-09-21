#include "inc/fsm.h"

void reset_game_figure(GameFigureData *figure_data) {
  if (figure_data->current_figure) {
    free_matrix(figure_data->current_figure, FIGURE_SIZE);
    figure_data->current_figure = NULL;
  }
  figure_data->current_x = START_X;
  figure_data->current_y = 0;
  figure_data->initialized = false;
}

bool handle_terminate_action(GameState *state, GameFigureData *figure_data) {
  reset_game_figure(figure_data);
  *state = GAME_OVER;
  return true;
}

void handle_start(UserAction_t action, GameState *state, GameInfo_t *game_info,
                  GameFigureData *figure_data) {
  if (action == Start && (!figure_data->initialized)) {
    if (init_game(game_info, figure_data)) {
      figure_data->initialized = true;
      *state = GAME_SPAWN;
    }
  }
}

void handle_spawn_state(GameState *state, GameInfo_t *game_info,
                        GameFigureData *figure_data) {

  if (!figure_data->current_figure) {
    figure_data->current_figure = create_matrix(FIGURE_SIZE, FIGURE_SIZE);
    if (!figure_data->current_figure) {
      *state = GAME_OVER;
      return;
    }
  }

  if (!spawn_next_figure(game_info, figure_data) ||
      check_game_over(game_info)) {
    *state = GAME_OVER;
  } else {
    *state = GAME_MOVING;
  }
}

void handle_moving_state(UserAction_t action, GameState *state,
                         GameInfo_t *game_info, GameFigureData *figure_data) {
  if (action == Pause) {
    game_info->pause = true;
    *state = GAME_PAUSED;
  } else if (action == Terminate) {
    *state = GAME_OVER;
  } else {
    handle_movement(action, state, game_info, figure_data);
  }
}

void handle_game_attached(GameState *state, GameInfo_t *game_info,
                          GameFigureData *figure_data) {
  attach_figure(game_info, figure_data);
  clear_lines(game_info);
  *state = GAME_SPAWN;
}

void handle_pause_state(UserAction_t action, GameInfo_t *game_info,
                        GameState *state) {
  if (action == Pause) {
    game_info->pause = false;
    *state = GAME_MOVING;
  }
}

void handle_state_machine(GameState *state, UserAction_t action,
                          GameInfo_t *game_info) {
  if (!state || !game_info)
    return;
  bool state_status = false;

  static GameFigureData figure_data = {
      .current_figure = NULL, .current_x = START_X, .current_y = 0};

  if (action == Terminate) {
    state_status = handle_terminate_action(state, &figure_data);
  }
  if (*state == GAME_OVER && action == Start) {
    figure_data.initialized = false;
    figure_data.current_figure = NULL;
    figure_data.current_x = START_X;
    figure_data.current_y = 0;
    free_game_info(game_info);
    *state = GAME_START;
    state_status = 1;
  }

  if (!state_status) {
    switch (*state) {
    case GAME_START:
      handle_start(action, state, game_info, &figure_data);
      break;

    case GAME_SPAWN:
      handle_spawn_state(state, game_info, &figure_data);
      break;

    case GAME_MOVING:
      handle_moving_state(action, state, game_info, &figure_data);
      break;

    case GAME_ATTACHED:
      handle_game_attached(state, game_info, &figure_data);
      break;

    case GAME_PAUSED:
      handle_pause_state(action, game_info, state);
      break;

    case GAME_OVER:
      check_high_score(game_info);
      free_game_info(game_info);
      break;
    }
  }
}
