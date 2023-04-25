#include "cpu.h"
#include <iostream>
#include "decoder.h"

void CPU::tick() {
    const auto opcode = get_next();

    const auto instruction = decode_opcode(opcode);
    if (!instruction.has_value()) {
        std::cout << "Invalid opcode: " << std::hex << static_cast<int>(opcode) << "\n";
        return;
    }

    const auto handler = instruction.value();
    handler(*this);
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