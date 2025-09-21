
#ifndef TETRIS_LOGIC_H
#define TETRIS_LOGIC_H

#include "db.h"
#include "defines.h"
#include "game_controller.h"
#include "types.h"
#include "utils.h"
// #include <stdbool.h>
// #include <stdlib.h>
#include <time.h>

typedef enum {
  FIGURE_I,
  FIGURE_O,
  FIGURE_T,
  FIGURE_L,
  FIGURE_J,
  FIGURE_S,
  FIGURE_Z
} FigureType;

typedef struct {
  const int pattern[FIGURE_SIZE][FIGURE_SIZE];
} FigurePattern;

void generate_figure(int **next);

bool spawn_next_figure(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Проверяет возможность размещения фигуры
 * @param field Игровое поле
 * @param figure Проверяемая фигура
 * @param pos_x X-координата для проверки
 * @param pos_y Y-координата для проверки
 * @return true - можно разместить, false - нельзя
 */
bool can_place_figure(int **field, int **figure, int pos_x, int pos_y);

/**
 * @brief Очищает заполненные линии и сдвигает поле вниз
 * @param game_info Указатель на структуру с игровой информацией
 */
void clear_lines(GameInfo_t *game_info);

/**
 * @brief Вычисляет количество очков за очищенные линии
 * @param lines_clear Количество очищенных линий
 * @return Количество начисленных очков
 */
int calculate_score(int lines_clear);

/**
 * @brief Проверка значения high_score
 * @param game_info Указатель на структуру с игровой информацией
 */
void check_high_score(GameInfo_t *game_info);

/**
 * @brief Обновление уровня
 * @param game_info Указатель на структуру с игровой информацией
 */
void update_level(GameInfo_t *game_info);

/**
 * @brief Обновление скорости падения фигур
 * @param game_info Указатель на структуру с игровой информацией
 */
void update_speed(GameInfo_t *game_info);

#endif // TETRIS_LOGIC_H