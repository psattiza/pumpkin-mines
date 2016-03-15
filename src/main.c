

#include "board.h"
#include "ncurses_interface.h"

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");

    struct minesweeper_board *game_board = generate_board(16, 30, 99);

    struct board_window boardwin;
    struct info_window infowin;

    setup(&boardwin, &infowin, game_board);

    int ch;
    while (1) {
        if ((ch = getch()) > 0) {
            // process key
            switch (ch) {
                case KEY_UP:
                case 'w':
                    shift_cursor(&boardwin, UP);
                    break;
                case KEY_DOWN:
                case 's':
                    shift_cursor(&boardwin, DOWN);
                    break;
                case KEY_LEFT:
                case 'a':
                    shift_cursor(&boardwin, LEFT);
                    break;
                case KEY_RIGHT:
                case 'd':
                    shift_cursor(&boardwin, RIGHT);
                    break;
                case KEY_ENTER:
                case 'e':
                    reveal(&boardwin);
                    break;
                case ' ':
                case 'f':
                    flag(&boardwin);
                    break;
                case 'q':
                    goto end;
            }
            draw_board(&boardwin);
        }

        draw_info(&infowin);
        usleep(20000);
    }
    /*
    while ((ch = getch()) != 'q') {
        //refresh();

        switch (ch) {
            case KEY_UP:
            case 'w':
                shift_cursor(&boardwin, UP);
                break;
            case KEY_DOWN:
            case 's':
                shift_cursor(&boardwin, DOWN);
                break;
            case KEY_LEFT:
            case 'a':
                shift_cursor(&boardwin, LEFT);
                break;
            case KEY_RIGHT:
            case 'd':
                shift_cursor(&boardwin, RIGHT);
                break;
            case KEY_ENTER:
            case 'e':
                reveal(&boardwin);
                break;
            case ' ':
            case 'f':
                flag(&boardwin);
                break;
        }

        draw_board(&boardwin);
        draw_info(&infowin);
    }
    */
    end:

    delboard(game_board);

    endwin();

    exit(EXIT_SUCCESS);
}
