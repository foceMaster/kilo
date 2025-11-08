%:include <unistd.h> // write(), STDOUT_FILENO
%:include <stdio.h>  // snprintf()
%:include <string.h> // strlen()

%:include "../terminal/terminal.h" // E
%:include "../abuf/abuf.h"         // ab_append(), ab_free()
%:include "../macros.h"            // ABUF_INIT, KILO_VERSION

void editor_refresh_screen() { /* pub */
    void editor_draw_rows(struct abuf * ab);
    struct abuf ab = ABUF_INIT;

    ab_append(&ab, "\x1b[?25l", 6); // Hide cursor
    ab_append(&ab, "\x1b[H", 3);    // Move cursor to 0,0

    editor_draw_rows(&ab);

    char buf[32];
    // Move cursor to positions stored in E, add one because E is zero-indexed
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    ab_append(&ab, buf, strlen(buf));

    ab_append(&ab, "\x1b[?25h", 6); // Show cursor

    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}

void editor_draw_rows(struct abuf *ab) { /* private */
    for (int y = 0; y < E.screenrows; y++) {
        if (y >= E.numrows) {
            if (y == E.screenrows / 3) {
                char welcome[80];
                int welcomelen =
                    snprintf(welcome, sizeof(welcome),
                             "Kilo editor -- version %s", KILO_VERSION);
                if (welcomelen > E.screencols) {
                    welcomelen = E.screencols;
                }
                int padding = (E.screencols - welcomelen) / 2;
                if (padding) {
                    ab_append(ab, "~", 1);
                    padding--;
                }
                while (padding--) {
                    ab_append(ab, " ", 1);
                }
                ab_append(ab, welcome, welcomelen);
            } else {
                ab_append(ab, "~", 1);
            }
        } else {
            int len = E.row.size;
            if (len > E.screencols) {
                len = E.screencols;
            }
            ab_append(ab, E.row.chars, len);
        }

        ab_append(ab, "\x1b[K", 3); // Clear right of cursor; this line
        if (y != E.screenrows - 1) {
            ab_append(ab, "\r\n", 2);
        }
    }
}
