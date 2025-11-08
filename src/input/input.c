%:include <stdlib.h> // exit()
%:include <unistd.h> // write(), STDOUT_FILENO

%:include "../terminal/terminal.h" // editor_read_key(), editor_key()

%:include "../macros.h" // CTRL_KEY()

void editor_process_keypress() { /* pub */
    void editor_move_cursor(int key);
    int c = editor_read_key();

    switch (c) {
    case CTRL_KEY('q'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1B[H", 3);
        exit(0);
        break;
    case ARROW_LEFT:
    case ARROW_RIGHT:
    case ARROW_UP:
    case ARROW_DOWN:
        editor_move_cursor(c);
        break;
    case PAGE_UP:
    case PAGE_DOWN: {
        int times =
            E.screenrows; // Can't declare variable instide switch statement
        while (times--) {
            editor_move_cursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
        }
        break;
    }
    case HOME:
        E.cx = 0;
        break;
    case END:
        E.cx = E.screencols - 1;
        break;
    }
}

void editor_move_cursor(int key) {
    switch (key) {
    case ARROW_LEFT:
        if (E.cx != 0) {
            E.cx--;
        }
        break;
    case ARROW_RIGHT:
        if (E.cx != E.screencols) {
            E.cx++;
        }
        break;
    case ARROW_UP:
        if (E.cy != 0) {
            E.cy--;
        }
        break;
    case ARROW_DOWN:
        if (E.cy != E.screenrows) {
            E.cy++;
        }
        break;
    }
}
