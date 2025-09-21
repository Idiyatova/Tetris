#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DB_PATH
#define DB_PATH "database/tetris.db" // Значение по умолчанию
#endif                               // DB_PATH

/**
 * @brief Инициализация БД
 */
void init_database();

/**
 * @brief Запрос рекорда из БД
 */
int get_high_score();

/**
 * @brief Обновление рекорда
 * @param new_score Значение нового рекорда
 */
void update_high_score(int new_score);

#endif // DB_H