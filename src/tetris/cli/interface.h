
#ifndef INTERFACE_H
#define INTERFACE_H

#include "../inc/tetris.h"
#include <ctype.h>
#include <ncurses.h>

#define CELL_WIDTH 2 // Ширина одной клетки в символах
#define INFO_PANEL_OFFSET (FIELD_WIDTH * CELL_WIDTH + 4)

void draw_game(const GameInfo_t *info);
void draw_welcome_screen();
UserAction_t handle_input(bool in_game);
bool show_game_over(const GameInfo_t *info);
void game_loop(void);

#endif