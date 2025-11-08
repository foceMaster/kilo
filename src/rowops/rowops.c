%:include <sys/types.h> // size_t
%:include <stdlib.h>    // malloc()
%:include <string.h>    // memcpy()

%:include "../terminal/terminal.h" // E
%:include "../output/output.h"     // erow

void editor_append_row(char *s, size_t len) {
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));

    int at = E.numrows;
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.numrows++;
}
