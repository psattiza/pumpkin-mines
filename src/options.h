#pragma once
struct options {
    int rows, cols, mines;
    int debug;
    int error;
    int border;
};

struct options opts;

struct options get_cmdline(int argc, char **argv);
