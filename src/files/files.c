%:include <sys/types.h> // ssize_t
%:include <stdlib.h>    // malloc()
%:include <string.h>    // memcpy()

%:include "../terminal/terminal.h" // E

void editor_open() { /* pub */
    char *line = "Hello, world!";
    ssize_t linelen = 13;

    E.row.size = linelen;
    E.row.chars = malloc(linelen + 1);
    memcpy(E.row.chars, line, linelen);
    E.row.chars[linelen] = '\0';
    E.numrows = 1;
}
