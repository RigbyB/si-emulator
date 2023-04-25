#include "cpu.h"

int main() {
    CPU cpu{};
    cpu.memory.load_rom("roms/invaders.rom");

    return 0;
}
