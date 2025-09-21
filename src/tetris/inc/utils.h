#ifndef UTILS_H
#define UTILS_H

#include "defines.h"
#include "tetris_logic.h"
#include "types.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Инициализирует игровые структуры
 * @param game_info Указатель на структуру для инициализации
 * @param current_figure_ptr Указатель на указатель текущей фигуры
 * @return true если инициализация успешна, false при ошибке
 */
bool init_game(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Создает новую матрицу заданного размера
 * @param rows Количество строк
 * @param cols Количество столбцов
 * @return Указатель на созданную матрицу или NULL при ошибке
 */
int **create_matrix(int rows, int cols);

/**
 * @brief Проверяет условие завершения игры
 * @param game_info Указатель на структуру с игровой информацией
 * @return true если игра должна завершиться, false если можно продолжать
 */
bool check_game_over(GameInfo_t *game_info);

/**
 * @brief Освобождает память, занятую матрицей
 * @param matrix Указатель на матрицу
 * @param rows Количество строк в матрице
 */
void free_matrix(int **matrix, int rows);

/**
 * @brief Освобождает ресурсы, связанные с игровой информацией
 * @param game_info Указатель на структуру для очистки
 */
void free_game_info(GameInfo_t *game_info);

#endif // UTILS_H