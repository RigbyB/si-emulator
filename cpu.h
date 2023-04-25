#pragma once

#include <cstdint>
#include "memory.h"

class Memory;

class CPU {
public:
    void tick();

    uint8_t get_next_instruction();

    Memory memory;

    uint8_t b;

    uint16_t sp;
    uint16_t pc;
};
