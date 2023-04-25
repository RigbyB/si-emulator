#include "decoder.h"
#include <unordered_map>
#include <iostream>
#include "cpu.h"


void nop_handler(CPU &cpu) {
    std::cout << "NOP\n";
}

void lxi_b_handler(CPU &cpu) {
    const auto low = cpu.get_next();
    const auto high = cpu.get_next();
    cpu.b = high;
    cpu.c = low;
    const auto val = (high << 8) | low;
    std::cout << "LXI B, " << std::hex << static_cast<int>(val) << "\n";
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

void dad_b_handler(CPU &cpu) {
    const auto hl = (cpu.h << 8) | cpu.l;
    const auto bc = (cpu.b << 8) | cpu.c;
    const auto result = hl + bc;
    cpu.h = (result >> 8) & 0xFF;
    cpu.l = result & 0xFF;

    // TODO: Set flags

    std::cout << "DAD B\n";
}

void ldax_b_handler(CPU &cpu) {
    const auto addr = (cpu.b << 8) | cpu.c;
    cpu.a = cpu.memory.read(addr);
    std::cout << "LDAX B\n";
}

void mvi_c_handler(CPU &cpu) {
    const auto val = cpu.get_next();
    cpu.c = val;
    std::cout << "MVI C, " << std::hex << static_cast<int>(val) << "\n";
}

void lxi_d_handler(CPU &cpu) {
    const auto low = cpu.get_next();
    const auto high = cpu.get_next();
    cpu.d = high;
    cpu.e = low;
    const auto val = (high << 8) | low;
    std::cout << "LXI D, " << std::hex << static_cast<int>(val) << "\n";
}

void inx_d_handler(CPU &cpu) {
    cpu.e++;

    if (cpu.e == 0)
        cpu.d++;

    std::cout << "INX D\n";
}

void dad_d_handler(CPU &cpu) {
    const auto hl = (cpu.h << 8) | cpu.l;
    const auto de = (cpu.d << 8) | cpu.e;
    const auto result = hl + de;
    cpu.h = (result >> 8) & 0xFF;
    cpu.l = result & 0xFF;

    // TODO: Set flags

    std::cout << "DAD D\n";
}

void ldax_d_handler(CPU &cpu) {
    const auto addr = (cpu.d << 8) | cpu.e;
    cpu.a = cpu.memory.read(addr);
    std::cout << "LDAX D\n";
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

void mvi_h_handler(CPU &cpu) {
    const auto val = cpu.get_next();
    cpu.h = val;
    std::cout << "MVI H, " << std::hex << static_cast<int>(val) << "\n";
}

void dad_h_handler(CPU &cpu) {
    const auto hl = (cpu.h << 8) | cpu.l;
    const auto result = hl + hl;
    cpu.h = (result >> 8) & 0xFF;
    cpu.l = result & 0xFF;

    // TODO: Set flags

    std::cout << "DAD H\n";
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

void lda_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.a = cpu.memory.read(addr);
    std::cout << "LDA " << std::hex << static_cast<int>(addr) << "\n";
}

void mvi_a_handler(CPU &cpu) {
    const auto val = cpu.get_next();
    cpu.a = val;
    std::cout << "MVI A, " << std::hex << static_cast<int>(val) << "\n";
}

void mov_l_a_handler(CPU &cpu) {
    cpu.l = cpu.a;
    std::cout << "MOV L, A\n";
}

void mov_m_a_handler(CPU &cpu) {
    const auto addr = (cpu.h << 8) | cpu.l;
    cpu.memory.write(addr, cpu.a);
    std::cout << "MOV M, A\n";
}

void ana_a_handler(CPU &cpu) {
    cpu.a &= cpu.a;

    // TOOD: Set flags

    std::cout << "ANA A\n";
}

void xra_a_handler(CPU &cpu) {
    cpu.a ^= cpu.a;

    // TODO: Set flags

    std::cout << "XRA A\n";
}

void ora_c_handler(CPU &cpu) {
    cpu.a |= cpu.c;

    // TODO: Set flags

    std::cout << "ORA C\n";
}

void pop_b_handler(CPU &cpu) {
    const auto val = cpu.memory.read_word(cpu.sp);
    cpu.sp += 2;
    cpu.b = (val >> 8) & 0xFF;
    cpu.c = val & 0xFF;
    std::cout << "POP B\n";
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

void push_b_handler(CPU &cpu) {
    cpu.sp -= 2;
    cpu.memory.write_word(cpu.sp, (cpu.b << 8) | cpu.c);
    std::cout << "PUSH B\n";
}

void ret_handler(CPU &cpu) {
    cpu.pc = cpu.memory.read_word(cpu.sp);
    cpu.sp += 2;
    std::cout << "RET\n";
}

void call_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();
    cpu.sp -= 2;
    // This is the location after the call instruction and arguments
    cpu.memory.write_word(cpu.sp, cpu.pc + 2);
    cpu.pc = addr;
    std::cout << "CALL " << std::hex << static_cast<int>(addr) << "\n";
}

void out_handler(CPU &cpu) {
    const auto port = cpu.get_next();

    // TODO: Implement output

    std::cout << "OUT " << std::hex << static_cast<int>(port) << "\n";
}

void push_d_handler(CPU &cpu) {
    cpu.sp -= 2;
    cpu.memory.write_word(cpu.sp, (cpu.d << 8) | cpu.e);
    std::cout << "PUSH D\n";
}

void pop_h_handler(CPU &cpu) {
    const auto val = cpu.memory.read_word(cpu.sp);
    cpu.sp += 2;
    cpu.h = (val >> 8) & 0xFF;
    cpu.l = val & 0xFF;
    std::cout << "POP H\n";
}

void push_h_handler(CPU &cpu) {
    cpu.sp -= 2;
    cpu.memory.write_word(cpu.sp, (cpu.h << 8) | cpu.l);
    std::cout << "PUSH H\n";
}

void xchg_handler(CPU &cpu) {
    const auto hl = (cpu.h << 8) | cpu.l;
    const auto de = (cpu.d << 8) | cpu.e;
    cpu.h = (de >> 8) & 0xFF;
    cpu.l = de & 0xFF;
    cpu.d = (hl >> 8) & 0xFF;
    cpu.e = hl & 0xFF;
    std::cout << "XCHG\n";
}

const std::unordered_map<uint8_t, InstructionHandler> instructions = {
        {0x00, nop_handler},
        {0x01, lxi_b_handler},
        {0x05, dcr_b_handler},
        {0x06, mvi_b_handler},
        {0x09, dad_b_handler},
        {0x0A, ldax_b_handler},
        {0x0E, mvi_c_handler},
        {0x11, lxi_d_handler},
        {0x13, inx_d_handler},
        {0x19, dad_d_handler},
        {0x1A, ldax_d_handler},
        {0x21, lxi_h_handler},
        {0x23, inx_h_handler},
        {0x26, mvi_h_handler},
        {0x29, dad_h_handler},
        {0x31, lxi_sp_handler},
        {0x32, sta_handler},
        {0x3A, lda_handler},
        {0x3E, mvi_a_handler},
        {0x6F, mov_l_a_handler},
        {0x77, mov_m_a_handler},
        {0xA7, ana_a_handler},
        {0xAF, xra_a_handler},
        {0xB1, ora_c_handler},
        {0xC1, pop_b_handler},
        // Although JMP, CALL, etc. are 3 byte instruction, we're going to be overwriting the PC
        {0xC2, jnz_handler},
        {0xC3, jmp_handler},
        {0xC5, push_b_handler},
        {0xC9, ret_handler},
        {0xCD, call_handler},
        {0xD3, out_handler},
        {0xD5, push_d_handler},
        {0xE1, pop_h_handler},
        {0xE5, push_h_handler},
        {0xEB, xchg_handler},
};

std::optional<InstructionHandler> decode_opcode(const uint8_t opcode) {
    const auto instr_iter = instructions.find(opcode);

    if (instr_iter == instructions.end())
        return std::nullopt;

    return instr_iter->second;
}