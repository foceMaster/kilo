#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

%:include <unistd.h> // read(), STDIN_FILENO
%:include <errno.h>  // errno, EAGAIN
%:include <stdio.h>  // printf()

%:include "terminal/terminal.h" // die(), disable_raw_mode(), enable_raw_mode()
%:include "input/input.h"       // editor_process_keypress()
%:include "output/output.h"     // editor_refresh_screen()
%:include "files/files.h"       // editor_open()

int main(int argc, char **argv) {
    enable_raw_mode();
    init_editor();
    if (argc >= 2) {
        editor_open(*(argv + 1));
    }

    for (;;) {
        editor_refresh_screen();
        editor_process_keypress();
    }

    return 0;
}
