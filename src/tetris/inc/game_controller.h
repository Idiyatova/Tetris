#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "defines.h"
#include "types.h"
// #include <stdbool.h>
#include "tetris_logic.h"
#include "utils.h"

/**
 * @brief Обрабатывает движение фигуры
 * @param action Действие пользователя
 * @param state Указатель на текущее состояние игры
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 */
void handle_movement(UserAction_t action, GameState *state,
                     GameInfo_t *game_info, GameFigureData *figure_data);
/**
 * @brief Двигает фигуру влево
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 * @return true - движение успешно, false - движение невозможно
 */
bool move_figure_left(GameInfo_t *game_info, GameFigureData *figure_data);
/**
 * @brief Двигает фигуру вправо
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 * @return true - движение успешно, false - движение невозможно
 */
bool move_figure_right(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Двигает фигуру вниз
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 * @return true - движение успешно, false - движение невозможно
 */
bool move_figure_down(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Поворот фигуры
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 * @return true - поворот успешен, false - поворот невозможен
 */
bool try_rotate(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Фиксирует фигуру на игровом поле
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 */
void attach_figure(GameInfo_t *game_info, GameFigureData *figure_data);

/**
 * @brief Отрисовывает фигуру на поле
 * @param game_info Указатель на структуру с игровой информацией
 * @param figure_data Указатель на текущую фигуру и ее координаты
 * @return true - отрисовка успешна, false - ошибка
 */
bool draw_figure(GameInfo_t *game_info, GameFigureData *figure_data);

// /**
//  * @brief Убирает фигуру с поля
//  * @param game_info Указатель на структуру с игровой информацией
//  * @param figure_data Указатель на текущую фигуру и ее координаты
//  * @return true - удаление успешно, false - ошибка
//  */
// bool free_figure(GameInfo_t *game_info, GameFigureData *figure_data);

// /**
//  * @brief Поворачивает матрицу 4x4 на 90 градусов по часовой стрелке
//  * @param src Исходная матрица для поворота
//  * @param dst Матрица для сохранения результата поворота
//  */
// void rotate_matrix(int **src, int **dst);

// /**
//  * @brief Копирует содержимое одной матрицы в другую
//  * @param src Исходная матрица
//  * @param dst Целевая матрица
//  */
// void copy_matrix(int **src, int **dst);

// bool is_figure_active(const GameFigureData *figure_data);
#endif // GAME_CONTROLLER_H