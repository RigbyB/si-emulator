#include "decoder.h"
#include <unordered_map>
#include <iostream>
#include "cpu.h"


void nop_handler(CPU &cpu) {
    std::cout << "NOP\n";
}

void dcr_b_handler(CPU &cpu) {
    cpu.b--;

    cpu.set_zero(cpu.b == 0);
    // TODO: Set rest of flags

    std::cout << "DCR B\n";
}

void mvi_b_handler(CPU &cpu) {
    const auto val = cpu.get_next();
    cpu.b = val;
    std::cout << "MVI B, " << std::hex << static_cast<int>(val) << "\n";
}

void lxi_d_handler(CPU &cpu) {
    const auto low = cpu.get_next();
    const auto high = cpu.get_next();
    cpu.d = high;
    cpu.e = low;
    const auto val = (high << 8) | low;
    std::cout << "LXI D, " << std::hex << static_cast<int>(val) << "\n";
}

void ldax_d_handler(CPU &cpu) {
    const auto addr = (cpu.d << 8) | cpu.e;
    cpu.a = cpu.memory.read(addr);
    std::cout << "LDAX D\n";
}

void inx_d_handler(CPU &cpu) {
    cpu.e++;

    if (cpu.e == 0)
        cpu.d++;

    std::cout << "INX D\n";
}

void lxi_h_handler(CPU &cpu) {
    const auto low = cpu.get_next();
    const auto high = cpu.get_next();
    cpu.h = high;
    cpu.l = low;
    const auto val = (high << 8) | low;
    std::cout << "LXI H, " << std::hex << static_cast<int>(val) << "\n";
}

void inx_h_handler(CPU &cpu) {
    cpu.l++;

    if (cpu.l == 0)
        cpu.h++;

    std::cout << "INX H\n";
}

void lxi_sp_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.sp = addr;
    std::cout << "LXI SP, " << std::hex << static_cast<int>(addr) << "\n";
}

void sta_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.memory.write(addr, cpu.a);
    std::cout << "STA " << std::hex << static_cast<int>(addr) << "\n";
}

void mov_m_a_handler(CPU &cpu) {
    const auto addr = (cpu.h << 8) | cpu.l;
    cpu.memory.write(addr, cpu.a);
    std::cout << "MOV M, A\n";
}

void jnz_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();

    if (cpu.is_zero() == 0)
        cpu.pc = addr;

    std::cout << "JNZ " << std::hex << static_cast<int>(addr) << "\n";
}

void jmp_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.pc = addr;
    std::cout << "JMP " << std::hex << static_cast<int>(addr) << "\n";
}

void call_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.sp -= 2;
    // This is the location after the call instruction and arguments
    cpu.memory.write_word(cpu.sp, cpu.pc + 2);
    cpu.pc = addr;
    std::cout << "CALL " << std::hex << static_cast<int>(addr) << "\n";
}

const std::unordered_map<uint8_t, InstructionHandler> instructions = {
        {0x00, nop_handler},
        {0x05, dcr_b_handler},
        {0x06, mvi_b_handler},
        {0x11, lxi_d_handler},
        {0x1a, ldax_d_handler},
        {0x13, inx_d_handler},
        {0x21, lxi_h_handler},
        {0x23, inx_h_handler},
        {0x31, lxi_sp_handler},
        {0x32, sta_handler},
        {0x77, mov_m_a_handler},
        // Although JMP, CALL, etc. are 3 byte instruction, we're going to be overwriting the PC
        {0xC2, jnz_handler},
        {0xC3, jmp_handler},
        {0xCD, call_handler}
};

std::optional<InstructionHandler> decode_opcode(const uint8_t opcode) {
    const auto instr_iter = instructions.find(opcode);

    if (instr_iter == instructions.end())
        return std::nullopt;

    return instr_iter->second;
}