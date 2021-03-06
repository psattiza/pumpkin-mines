
#include "options.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "limits.h"

/* Options:
 *  -e -> easy
 *  -i -> intermediate
 *  -h -> expert
 *  -d -> debug
 *  -r=INT -> rows
 *  -c=INT -> cols
 *  -m=INT -> mines
 */

extern char *optarg;

enum ERROR_CODES {
    NO_ERROR,
    INVALID_ARGS,
    TOO_MANY_MINES
};

const char USAGE[] = 
    "Usage: pumpkin-mines [args]\n"
    "Play minesweeper in your terminal\n"
    "\n"
    "Mandatory arguments for long options are mandatory for short ones too\n"
    " -b,   --beginner..........play beginner board\n"
    " -B,   --border............do not draw the pretty border\n"
    " -c,   --cols=SIZE.........specify the number of columns\n"
    " -d,   --debug.............turn debug mode on\n"
    " -e,   --expert............play expert board\n"
    " -h,   --help..............Print this help and exit\n"
    " -i,   --intermediate......play intermediate baord\n"
    " -m,   --mines=SIZE........specify the number of mines\n"
    " -r,   --rows=SIZE.........specify the number of rows\n"
    "\n"
    "The SIZE argument is a postive, non-zero integer\n"
    "If a difficulty is specified with -b, -i, or -e, then \n"
    "    -c, -m, and -r arguments will be ignored.\n"
    "If no options are specified, then an expert board will be generated\n"
    "    That is, \"pumpkin-mines\" is equivalent to \"pumpkin-mines -e\"\n"
    "\n"
    "Once the game is launched use the following controls\n"
    " wasd, hjkl, arrow keys....move the cursor\n"
    "   SHIFT + movement........move the cursor in larger incrememnts\n"
    "                           note: SHIFT does not work with arrow keys\n"
    " e, <RETURN>...............reveal a tile\n"
    " f, <SPACE>................flag a tile\n"
    " F3........................toggle Debug mode\n"
    " q.........................quit\n";


int parse_int(const char *arg, const char flag_char);

struct options get_cmdline(int argc, char **argv) {
    
    static struct option long_options[] = {
        {"beginner",     no_argument,       NULL, 'b'},
        {"border",       no_argument,       NULL, 'B'},
        {"cols",         required_argument, NULL, 'c'},
        {"debug",        no_argument,       NULL, 'd'},
        {"expert",       no_argument,       NULL, 'e'},
        {"intermediate", no_argument,       NULL, 'i'},
        {"help",         no_argument,       NULL, 'h'},
        {"mines",        required_argument, NULL, 'm'},
        {"rows",         required_argument, NULL, 'r'},
        {0,              0,                 0,    0}
    };

    opts.cols = 30;
    opts.rows = 16;
    opts.mines = 99;
    opts.debug = 0;
    opts.error = 0;
    opts.border = 1;

    int have_set_difficulty = 0;
    int flag_char = 0;
    int option_index = 0;
    while ((flag_char = getopt_long(argc, argv, "bBc:deihm:r:", long_options,
                                    &option_index)) != EOF) {
        switch (flag_char) {
        case '\0':
            break;
        case 'b':
            have_set_difficulty = 1;
            opts.cols = 8;
            opts.rows = 8;
            opts.mines = 10;
            break;
        case 'B':
            opts.border = 0;
            break;
        case 'i':
            have_set_difficulty = 1;
            opts.cols = 16;
            opts.rows = 16;
            opts.mines = 40;
            break;
        case 'e':
            have_set_difficulty = 1;
            opts.cols = 30;
            opts.rows = 16;
            opts.mines = 99;
            break;
        case 'd':
            opts.debug = 1;
            break;
        case 'r':
            if (!have_set_difficulty) {
                int result = parse_int(optarg, flag_char);
                if (result == INT_MAX) {
                    // An error during the parse
                    fprintf(stderr,
                            "%s: -%c only takes positive integer arguments\n",
                            argv[0], flag_char);
                    opts.error = 1;
                } else {
                    opts.rows = result;
                }
            }
            break;
        case 'c':
            if (!have_set_difficulty) {
                int result = parse_int(optarg, flag_char);
                if (result == INT_MAX) {
                    // An error during the parse
                    fprintf(stderr,
                            "%s: -%c only takes positive integer arguments\n",
                            argv[0], flag_char);
                    opts.error = 1;
                } else {
                    opts.cols = result;
                }
            }
            break;
        case 'm':
            if (!have_set_difficulty) {
                int result = parse_int(optarg, flag_char);
                if (result == INT_MAX) {
                    // An error during the parse
                    fprintf(stderr,
                            "%s: -%c only takes positive integer arguments\n",
                            argv[0], flag_char);
                    opts.error = 1;
                } else {
                    opts.mines = result;
                }
            }
            break;
        case 'h':
            fputs(USAGE, stdout);
            exit(EXIT_SUCCESS); 
        case '?':
            opts.error = 1;
            break;
        default:
            exit(1);
            break;
        }
    }

    // TODO: Move this constant somewhere else
    const int NUM_TILES_CANT_BE_MINE = 5*5;
    if (opts.rows * opts.cols - NUM_TILES_CANT_BE_MINE < opts.mines) {
        opts.error = 2;
    }

    return opts;
}

int parse_int(const char *optarg, const char flag_char) {
    for (const char *c = optarg; *c != '\0'; c++) {
        if (!isdigit(*c)) {
            return INT_MAX;
        }
    }

    return atoi(optarg);
}
