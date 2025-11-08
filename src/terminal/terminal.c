%:include <unistd.h>    // STDIN_FILENO, read()
%:include <errno.h>     //errno, EAGAIN
%:include <termios.h>   //tcsetattr(), tcgetattr(), TCSAFLUSH, various flags
%:include <stdlib.h>    //exit(), atexit()
%:include <stdio.h>     //perror(), printf(), sscanf()
%:include <sys/ioctl.h> //ioctl(), struct winsize, TIOCGWINSZ

%:include "terminal.h" //E, editor_config, editor_key
struct editor_config E;

void die(const char *s) { /* pub */
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1B[H", 3);

    perror(s);
    exit(1);
}

void disable_raw_mode() { /* pub */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enable_raw_mode() { /* pub */
    // Get original mode and reset after exit
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
        die("tcsetattr");
    }
    atexit(disable_raw_mode);

    struct termios raw = E.orig_termios;
    raw.c_lflag &= ~(ECHO);   // Turn of echo
    raw.c_lflag &= ~(ICANON); // Turn on cooked mode
    raw.c_lflag &= ~(ISIG);   // Don't listen to ctrl + c or ctrl + z
    raw.c_lflag &= ~(IEXTEN); // Don't listen to ctrl + v
    raw.c_lflag &= ~(CS8);    // Set character size to 8, probably already done
    raw.c_iflag &= ~(IXON);   // Don't listen to ctrl + q or ctrl + s
    raw.c_iflag &= ~(ICRNL);  // Don't make a carriage return into a newline
    raw.c_iflag &=
        ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // Raw-mode tradition
    raw.c_oflag &= ~(OPOST);                       // Don't replace \n with \r\n
    raw.c_cc[VMIN] = 0;  // Return directly when over 0
    raw.c_cc[VTIME] = 1; // Timeout

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

int editor_read_key() { /* pub; input.c */
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) {
            return '\x1b';
        }
        if (read(STDIN_FILENO, &seq[1], 1) != 1) {
            return '\x1b';
        }

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) {
                    return '\x1b';
                }
                if (seq[2] == '~') {
                    switch (seq[1]) {
                    case '1':
                    case '7':
                        return HOME;
                    case '3':
                        return DEL;
                    case '4':
                    case '8':
                        return END;
                    case '5':
                        return PAGE_UP;
                    case '6':
                        return PAGE_DOWN;
                    }
                }
            } else {
                switch (seq[1]) {
                case 'A':
                    return ARROW_UP;
                case 'B':
                    return ARROW_DOWN;
                case 'C':
                    return ARROW_RIGHT;
                case 'D':
                    return ARROW_LEFT;
                case 'H':
                    return HOME;
                case 'F':
                    return END;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
            case 'H':
                return HOME;
            case 'F':
                return END;
            }
        }

        return '\x1b';
    } else {
        return c;
    }
}

int get_window_size(int *rows, int *cols) { /* private */
    int get_cursor_position(int *rows, int *cols);
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
            return -1;
        }
        return get_cursor_position(rows, cols);
        editor_read_key();
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

int get_cursor_position(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1B[6n", 4) != 4) {
        return -1;
    }

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) {
            break;
        }
        if (buf[i] == 'R') {
            break;
        }
        i++;
    }
    buf[i] = '\0';

    if (buf[0] != '\x1B' || buf[1] != '[') {
        return -1;
    }
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
        return -1;
    }

    return 0;
}

void init_editor() { /* pub */
    E.cx = 0;
    E.cy = 0;
    if (get_window_size(&E.screenrows, &E.screencols) == -1) {
        die("get_window_size");
    }
}
