#include "interface.h"

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(100);
  init_database();
  game_loop();
  cleanup_game();
  endwin();

  return 0;
}

void game_loop(void) {
  bool running = true;
  bool in_game = false;
  time_t last_update = time(NULL);
  while (running) {
    if (!in_game) {
      draw_welcome_screen();
      UserAction_t action = handle_input(false);
      if (action == Start) {
        in_game = true;
        userInput(Start, false);
        clear();
      } else if (action == Terminate) {
        running = false;
      }
      continue;
    }

    GameInfo_t info = updateCurrentState();
    if (check_game_over(&info)) {
      if (show_game_over(&info)) {
        in_game = false;
        userInput(Terminate, false);
        continue;
      }
    }

    UserAction_t action = handle_input(true);
    if (action == Terminate) {
      running = false;
      continue;
    } else if (action != (UserAction_t)-1) {
      userInput(action, false);
      info = updateCurrentState();
    }

    time_t current = time(NULL);
    if (difftime(current, last_update) >= (1.0 / info.speed)) {
      userInput(Down, true);
      last_update = current;
    }

    info = updateCurrentState();
    draw_game(&info);

    if (info.pause) {
      int old_y, old_x;
      getyx(stdscr, old_y, old_x);
      clrtoeol();
      move(old_y, old_x);
    }
    draw_game(&info);
    refresh();
    napms(50);
  }
}

void draw_game(const GameInfo_t *info) {
  if (!info || !info->field || !info->next)
    return;
  clear();

  for (int y = 0; y < FIELD_HEIGHT + 2; y++) {
    mvaddch(y, 0, '|');
    mvaddch(y, FIELD_WIDTH * CELL_WIDTH + 1, '|');
  }
  for (int x = 0; x < FIELD_WIDTH * CELL_WIDTH + 2; x++) {
    mvaddch(FIELD_HEIGHT + 1, x, '-');
  }

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      int display_y = y + 1;
      int display_x = x * CELL_WIDTH + 1;

      if (info->field[y][x] == CELL_FIXED) {
        mvaddch(display_y, display_x, '[');
        mvaddch(display_y, display_x + 1, ']');
      } else if (info->field[y][x] == CELL_CURRENT) {
        mvaddch(display_y, display_x, '{');
        mvaddch(display_y, display_x + 1, '}');
      } else {

        mvaddch(display_y, display_x, ' ');
        mvaddch(display_y, display_x + 1, ' ');
      }
    }
  }

  mvprintw(0, INFO_PANEL_OFFSET, "Next:");
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (info->next[y][x]) {
        mvaddch(y + 2, INFO_PANEL_OFFSET + x * CELL_WIDTH, '[');
        mvaddch(y + 2, INFO_PANEL_OFFSET + x * CELL_WIDTH + 1, ']');
      }
    }
  }

  mvprintw(FIGURE_SIZE + 3, INFO_PANEL_OFFSET, "Score: %d", info->score);
  mvprintw(FIGURE_SIZE + 5, INFO_PANEL_OFFSET, "High score: %d",
           info->high_score);
  mvprintw(FIGURE_SIZE + 4, INFO_PANEL_OFFSET, "Level: %d", info->level);
  mvprintw(FIGURE_SIZE + 6, INFO_PANEL_OFFSET, "Speed: %d ms", info->speed);
}

UserAction_t handle_input(bool in_game) {
  int ch = getch();

  if (in_game) {
    switch (tolower(ch)) {
    case 'q':
      return Terminate;
    case 'p':
      return Pause;
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_UP:
      return Up;
    case KEY_DOWN:
      return Down;
    case ' ':
      return Action;
    default:
      return -1;
    }
  } else {
    switch (tolower(ch)) {
    case 's':
      return Start;
    case 'q':
      return Terminate;
    default:
      return -1;
    }
  }
}

void draw_welcome_screen() {
  clear();
  mvprintw(5, 10, "TETRIS");
  mvprintw(7, 5, "Controls:");
  mvprintw(8, 5, " - Move");
  mvprintw(9, 5, "Space - Rotate");
  mvprintw(12, 5, "P - Pause");
  mvprintw(14, 5, "Press S to start");
  mvprintw(15, 5, "Press Q to quit");
  refresh();
}

bool show_game_over(const GameInfo_t *info) {
  clear();
  mvprintw(5, 10, "GAME OVER");
  mvprintw(7, 5, "Final Score: %d", info->score);
  mvprintw(8, 5, "High Score: %d", info->high_score);
  mvprintw(10, 5, "Press Q to exit");

  refresh();

  timeout(-1);
  getch();
  timeout(100);

  return true;
}