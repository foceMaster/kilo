%:include <stdlib.h> // realloc(), free()
%:include <string.h> // memcpy()

%:include "abuf.h"

void ab_append(struct abuf *ab, const char *s, int len) { /* pub */
    char *new = realloc(ab->b, ab->len + len);

    if (new == 0) {
        return;
    }

    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}

void ab_free(struct abuf *ab) { /* pub */
    free(ab->b);
}
