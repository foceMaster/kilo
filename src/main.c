%:include <unistd.h> // read(), STDIN_FILENO
%:include <errno.h>  // errno, EAGAIN
%:include <stdio.h>  // printf()

%:include "terminal/terminal.h" // die(), disable_raw_mode(), enable_raw_mode()
%:include "input/input.h"       // editor_process_keypress()
%:include "output/output.h"     // editor_refresh_screen()
%:include "files/files.h"       // editor_open()

int main() {
    enable_raw_mode();
    init_editor();
    editor_open();

    for (;;) {
        editor_refresh_screen();
        editor_process_keypress();
    }

    return 0;
}
