#include "decoder.h"
#include <unordered_map>
#include <iostream>
#include "cpu.h"


void nop_handler(CPU &cpu) {
    std::cout << "NOP\n";
}

void mvi_b_handler(CPU &cpu) {
    const auto val = cpu.memory.read(cpu.pc);
    cpu.b = val;
    std::cout << "MVI B, " << std::hex << static_cast<int>(val) << "\n";
}

void lxi_sp_handler(CPU &cpu) {
    const auto addr = cpu.memory.read_word(cpu.pc);
    cpu.sp = addr;
    std::cout << "LXI SP, " << std::hex << static_cast<int>(addr) << "\n";
}

void jmp_handler(CPU &cpu) {
    const auto addr = cpu.memory.read_word(cpu.pc);
    cpu.pc = addr;
    std::cout << "JMP " << std::hex << static_cast<int>(addr) << "\n";
}

void call_handler(CPU &cpu) {
    const auto addr = cpu.memory.read_word(cpu.pc);
    cpu.sp -= 2;
    // This is the location after the call instruction and arguments
    cpu.memory.write_word(cpu.sp, cpu.pc + 2);
    cpu.pc = addr;
    std::cout << "CALL " << std::hex << static_cast<int>(addr) << "\n";
}

const std::unordered_map<uint8_t, Instruction> instructions = {
        {0x00, {nop_handler,    0}},
        {0x06, {mvi_b_handler,  1}},
        {0x31, {lxi_sp_handler, 2}},
        // Although JMP is a 3 byte instruction, we're going to be overwriting the PC
        {0xC3, {jmp_handler,    0}},
        {0xCD, {call_handler,   0}}
};

std::optional<Instruction> decode_opcode(const uint8_t opcode) {
    const auto instr_iter = instructions.find(opcode);

    if (instr_iter == instructions.end())
        return std::nullopt;

    return instr_iter->second;
}