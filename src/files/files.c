%:include <sys/types.h> // ssize_t, size_t
%:include <stdlib.h>    // malloc()
%:include <string.h>    // memcpy()
%:include <stdio.h>     // fopen(), getline()

%:include "../terminal/terminal.h" // E
%:include "../rowops/rowops.h"     // editor_append_row()

void editor_open(char *filename) { /* pub */
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        die("fopen");
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        while (linelen > 0 &&
               (line[linelen - 1] == '\n' || line[linelen - 1] == '\r')) {
            linelen--;
        }
        editor_append_row(line, linelen);
    }
    free(line);
    fclose(fp);
}
