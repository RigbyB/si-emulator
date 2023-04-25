#include "cpu.h"

int main() {
    CPU cpu{};
    cpu.memory.load_rom("roms/invaders.rom");

    while (true) {
        cpu.tick();
    }

    return 0;
}
