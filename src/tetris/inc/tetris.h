#ifndef TETRIS_H
#define TETRIS_H

#include "fsm.h"
#include "types.h"
#include <string.h>
// #include <stdbool.h>
// #include <stdio.h>

/**
 * @brief Обрабатывает пользовательский ввод
 * @param action Действие пользователя
 * @param hold Флаг удержания клавиши
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Обновляет текущее состояние игры
 * @return Структура с текущим состоянием игры
 */
GameInfo_t updateCurrentState();

/**
 * @brief Освобождает все ресурсы игры
 */
void cleanup_game();
#endif // TETRIS_H
