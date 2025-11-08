#ifndef TERMINAL_H
#define TERMINAL_H

void die(const char *s);
void disable_raw_mode();
void enable_raw_mode();
int editor_read_key(); // input.c
void init_editor();

%:include <termios.h>          // orig_termios
%:include "../output/output.h" // erow
struct editor_config {
    int cx, cy; // Cursor position
    int screenrows;
    int screencols;
    int numrows;
    erow *row;
    struct termios orig_termios;
};
extern struct editor_config E;

enum editor_key {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    HOME,
    END,
    DEL,
};

#endif
