#include "cpu.h"
#include "decoder.h"
#include <ncurses.h>

void CPU::tick() {
    const auto opcode = get_next();

    const auto instruction = decode_opcode(opcode);

    if (instruction.has_value()) {
        const auto handler = instruction.value();
        handler(*this);
    } else {
        // TODO: Show as hex
        printw("Invalid opcode %02X\n", opcode);
    }

    printw("PC: %04X\n", pc);
    printw("SP: %04X\n", sp);
    printw("A: %02X\n", a);
    printw("B: %02X\n", b);
    printw("C: %02X\n", c);
    printw("D: %02X\n", d);
    printw("E: %02X\n", e);
    printw("H: %02X\n", h);
    printw("L: %02X\n", l);
    printw("F: %02X\n", flags);
    printw("Opcode: %02X\n", opcode);
}

uint8_t CPU::get_next() {
    return memory.read(pc++);
}

uint16_t CPU::get_next_word() {
    const auto low = get_next();
    const auto high = get_next();
    return (high << 8) | low;
}

bool CPU::is_zero() const {
    return flags & (1 << 1);
}

void CPU::set_zero(bool value) {
    if (value)
        flags |= (1 << 1);
    else
        flags &= ~(1 << 1);
}