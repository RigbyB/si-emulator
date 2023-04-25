#include "cpu.h"
#include <curses.h>

int main() {
    initscr();
    noecho();

    CPU cpu{};
    cpu.memory.load_rom("roms/invaders.rom");

    while (true) {
        clear();

        cpu.tick();

        refresh();
    }

    return 0;
}
