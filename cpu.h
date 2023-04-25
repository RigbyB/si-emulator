#pragma once

#include "memory.h"

class CPU {
public:
    void tick();

    Memory memory;

    uint16_t pc;
};