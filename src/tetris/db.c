#include "inc/db.h"

void init_database() {
  sqlite3 *db;
  char *err_msg = NULL;

  int rc = sqlite3_open(DB_PATH, &db);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    return;
  }

  const char *sql = "CREATE TABLE IF NOT EXISTS scores ("
                    "id INTEGER PRIMARY KEY CHECK(id = 1), "
                    "score INTEGER NOT NULL DEFAULT 0"
                    ");";

  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  sqlite3_free(err_msg);

  const char *insert_sql = "INSERT OR IGNORE INTO scores (score) VALUES (0);";
  rc = sqlite3_exec(db, insert_sql, 0, 0, &err_msg);
  sqlite3_free(err_msg);

  sqlite3_close(db);
}

int get_high_score() {
  sqlite3 *db;
  int high_score = 0;

  int rc = sqlite3_open(DB_PATH, &db);
  if (rc != SQLITE_OK) {
    return 0;
  }

  const char *sql = "SELECT score FROM scores WHERE id = 1;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
      high_score = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
  }

  sqlite3_close(db);
  return high_score;
}

void update_high_score(int new_score) {
  sqlite3 *db;

  int rc = sqlite3_open(DB_PATH, &db);
  if (rc != SQLITE_OK) {
    return;
  }

  const char *sql = "UPDATE scores SET score = ? WHERE id = 1;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, new_score);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }

  sqlite3_close(db);
}
