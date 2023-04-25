#include "decoder.h"
#include <unordered_map>
#include "cpu.h"

// Generic functions
void mvi(CPU &cpu, uint8_t &reg) {
    reg = cpu.get_next();
}

void lxi(CPU &cpu, uint8_t &reg_a, uint8_t &reg_b) {
    // Little endian
    reg_b = cpu.get_next();
    reg_a = cpu.get_next();
}

void lxi(CPU &cpu, uint16_t &reg) {
    // Little endian
    reg = cpu.get_next_word();
}

void inx(CPU &cpu, uint8_t &reg_a, uint8_t &reg_b) {
    reg_b++;

    if (reg_b == 0)
        reg_a++;
}

void dad(CPU &cpu, const uint8_t reg_a, const uint8_t reg_b) {
    const uint16_t hl = cpu.h << 8 | cpu.l;
    const uint16_t ab = reg_a << 8 | reg_b;

    const uint16_t result = hl + ab;

    cpu.h = (result >> 8) & 0xFF;
    cpu.l = result & 0xFF;

    // TODO: Set flag
}

void mov_m(CPU &cpu, const uint8_t &reg) {
    cpu.memory.write(cpu.h << 8 | cpu.l, reg);
}

void ldax(CPU &cpu, const uint8_t reg_a, const uint8_t reg_b) {
    cpu.a = cpu.memory.read(reg_a << 8 | reg_b);
}

void push(CPU &cpu, const uint8_t reg_a, const uint8_t reg_b) {
    cpu.sp -= 2;

    cpu.memory.write_word(cpu.sp, reg_a << 8 | reg_b);
}

void pop(CPU &cpu, uint8_t &reg_a, uint8_t &reg_b) {
    const uint16_t val = cpu.memory.read_word(cpu.sp);

    cpu.sp += 2;

    reg_a = (val >> 8) & 0xFF;
    reg_b = val & 0xFF;
}

void dcr(CPU &cpu, uint8_t &reg) {
    reg--;

    // TODO: Set flags
}

void ana(CPU &cpu, const uint8_t &reg) {
    cpu.a &= reg;

    // TOOD: Set flags
}

void xra(CPU &cpu, const uint8_t &reg) {
    cpu.a ^= reg;

    // TODO: Set flags
}

void ora(CPU &cpu, uint8_t &reg) {
    cpu.a |= cpu.c;

    // TODO: Set flags
}

// Handlers
void nop_handler(CPU &cpu) {}


#define MVI_HANDLER(reg) void mvi_##reg##_handler(CPU &cpu) { mvi(cpu, cpu.reg); }

MVI_HANDLER(a)

MVI_HANDLER(b)

MVI_HANDLER(c)

MVI_HANDLER(d)

MVI_HANDLER(e)

MVI_HANDLER(h)

MVI_HANDLER(l)


#define LXI_HANDLER(reg_a, reg_b) void lxi_##reg_a##_handler(CPU &cpu) { lxi(cpu, cpu.reg_a, cpu.reg_b); }

LXI_HANDLER(b, c)

LXI_HANDLER(d, e)

LXI_HANDLER(h, l)

void lxi_sp_handler(CPU &cpu) {
    lxi(cpu, cpu.sp);
}


void ldax_b_handler(CPU &cpu) {
    ldax(cpu, cpu.b, cpu.c);
}

void ldax_d_handler(CPU &cpu) {
    ldax(cpu, cpu.d, cpu.e);
}


void lda_handler(CPU &cpu) {
    cpu.a = cpu.memory.read(cpu.get_next_word());
}


#define INX_HANDLER(reg_a, reg_b) void inx_##reg_a##_handler(CPU &cpu) { inx(cpu, cpu.reg_a, cpu.reg_b); }

INX_HANDLER(d, e)

INX_HANDLER(h, l)


#define DAD_HANDLER(reg_a, reg_b) void dad_##reg_a##_handler(CPU &cpu) { dad(cpu, cpu.reg_a, cpu.reg_b); }

DAD_HANDLER(b, c)

DAD_HANDLER(d, e)

DAD_HANDLER(h, l)


#define MOV_HANDLER(reg_a, reg_b) void mov_##reg_a##_##reg_b##_handler(CPU &cpu) { cpu.reg_a = cpu.reg_b; }

MOV_HANDLER(l, a)


#define MOV_M_HANDLER(reg) void mov_m_##reg##_handler(CPU &cpu) { mov_m(cpu, cpu.reg); }

MOV_M_HANDLER(a)


#define PUSH_HANDLER(reg_a, reg_b) void push_##reg_a##_handler(CPU &cpu) { push(cpu, cpu.reg_a, cpu.reg_b); }

PUSH_HANDLER(b, c)

PUSH_HANDLER(d, e)

PUSH_HANDLER(h, l)


#define POP_HANDLER(reg_a, reg_b) void pop_##reg_a##_handler(CPU &cpu) { pop(cpu, cpu.reg_a, cpu.reg_b); }

POP_HANDLER(b, c)

POP_HANDLER(d, e)

POP_HANDLER(h, l)


#define DCR_HANDLER(reg) void dcr_##reg##_handler(CPU &cpu) { dcr(cpu, cpu.reg); }

DCR_HANDLER(b)


#define ANA_HANDLER(reg) void ana_##reg##_handler(CPU &cpu) { ana(cpu, cpu.reg); }

ANA_HANDLER(a)


#define XRA_HANDLER(reg) void xra_##reg##_handler(CPU &cpu) { xra(cpu, cpu.reg); }

XRA_HANDLER(a)


#define ORA_HANDLER(reg) void ora_##reg##_handler(CPU &cpu) { ora(cpu, cpu.reg); }

ORA_HANDLER(c)

void sta_handler(CPU &cpu) {
    cpu.memory.write(cpu.get_next_word(), cpu.a);
}


void jmp_handler(CPU &cpu) {
    cpu.pc = cpu.get_next_word();
}

void jnz_handler(CPU &cpu) {
    if (!cpu.is_zero())
        cpu.pc = cpu.get_next_word();
}

void call_handler(CPU &cpu) {
    const auto addr = cpu.get_next_word();

    cpu.sp -= 2;

    // The program counter is now after the arguments because of get_next_word
    cpu.memory.write_word(cpu.sp, cpu.pc);

    cpu.pc = addr;
}

void ret_handler(CPU &cpu) {
    cpu.pc = cpu.memory.read_word(cpu.sp);
    cpu.sp += 2;
}

void out_handler(CPU &cpu) {
    const auto port = cpu.get_next();

    // TODO: Implement output
}

void xchg_handler(CPU &cpu) {
    const auto hl = (cpu.h << 8) | cpu.l;
    const auto de = (cpu.d << 8) | cpu.e;

    cpu.h = (de >> 8) & 0xFF;
    cpu.l = de & 0xFF;

    cpu.d = (hl >> 8) & 0xFF;
    cpu.e = hl & 0xFF;
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