#include "inc/tetris.h"
#include "inc/defines.h"

static GameInfo_t *game_info = NULL;

void userInput(UserAction_t action, bool hold) {
  static GameState state = GAME_START;

  if (!game_info) {
    game_info = (GameInfo_t *)malloc(sizeof(GameInfo_t));
    if (!game_info)
      return;
    memset(game_info, 0, sizeof(GameInfo_t));
  }
  handle_state_machine(&state, action, game_info);
  (void)hold;
}

GameInfo_t updateCurrentState() {
  if (!game_info) {
    GameInfo_t empty = {0};
    return empty;
  }
  return *game_info;
}

void cleanup_game() {
  if (game_info) {
    if (game_info->field) {
      for (int i = 0; i < FIELD_HEIGHT; i++) {
        free(game_info->field[i]);
      }
      free(game_info->field);
      game_info->field = NULL;
    }

    if (game_info->next) {
      for (int i = 0; i < FIGURE_SIZE; i++) {
        free(game_info->next[i]);
      }
      free(game_info->next);
      game_info->next = NULL;
    }

    free(game_info);
    game_info = NULL;
  }
}