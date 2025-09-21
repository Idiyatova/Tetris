#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define FIGURE_SIZE 4
#define INITIAL_TIME 150
#define SPEED_STEP 20
#define MAX_LEVEL 10
#define MIN_SPEED 40
#define CELL_EMPTY 0
#define CELL_CURRENT 1
#define CELL_FIXED 2
#define START_X (FIELD_WIDTH / 2 - 2)

// состояния игры
typedef enum {
  GAME_START,
  GAME_SPAWN,
  GAME_MOVING,
  GAME_ATTACHED,
  GAME_PAUSED,
  GAME_OVER
} GameState;

typedef struct {
  bool initialized;
  int **current_figure;
  int current_x;
  int current_y;
} GameFigureData;

#endif // DEFINES_H