#include "decoder.h"
#include <unordered_map>
#include <iostream>
#include "cpu.h"

void nop_handler(CPU &cpu) {
    std::cout << "NOP\n";
}

void jmp_handler(CPU &cpu) {
    const auto addr = cpu.memory.read_word(cpu.pc);
    cpu.pc = addr;
    std::cout << "JMP " << std::hex << static_cast<int>(addr) << "\n";
}

const std::unordered_map<uint8_t, Instruction> instructions = {
        {0x00, {nop_handler, 0}},
        // Although JMP is a 3 byte instruction, we're going to be overwriting the PC
        {0xC3, {jmp_handler, 0}}
};

std::optional<Instruction> decode_opcode(const uint8_t opcode) {
    const auto instr_iter = instructions.find(opcode);

    if (instr_iter == instructions.end())
        return std::nullopt;

    return instr_iter->second;
}