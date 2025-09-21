#ifndef FSM_H
#define FSM_H

#include "defines.h"
#include "game_controller.h"
#include "tetris_logic.h"
#include "types.h"
// #include <stdlib.h>
#include "utils.h"

/**
 * @brief Обрабатывает машину состояний игры
 * @param state Указатель на текущее состояние игры
 * @param action Действие пользователя (ввод)
 * @param game_info Указатель на структуру с игровой информацией
 */
void handle_state_machine(GameState *state, UserAction_t action,
                          GameInfo_t *game_info);

#endif // FSM_H