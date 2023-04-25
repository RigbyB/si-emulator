#include "cpu.h"
#include <iostream>
#include "decoder.h"

void CPU::tick() {
    const auto opcode = get_next_instruction();

    const auto opt_instr = decode_opcode(opcode);
    if (!opt_instr.has_value()) {
        std::cout << "Invalid opcode: " << std::hex << static_cast<int>(opcode) << "\n";
        return;
    }

    const auto instr = opt_instr.value();
    instr.handler(*this);
    pc += instr.arg_size;
}

uint8_t CPU::get_next_instruction() {
    return memory.read(pc++);
}