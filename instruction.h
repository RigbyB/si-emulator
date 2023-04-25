#pragma once

#include <cstdint>

class CPU;

using InstructionHandler = void (*)(CPU &);

struct Instruction {
    InstructionHandler handler;
    uint8_t arg_size;
};