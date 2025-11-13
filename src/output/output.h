#ifndef OUTPUT_H
#define OUTPUT_H

typedef struct {
    int size;
    char *chars;
} erow;

void editor_refresh_screen();
void editor_scroll();

#endif
