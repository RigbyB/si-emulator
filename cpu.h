#pragma once

#include <cstdint>
#include "memory.h"

class Memory;

class CPU {
public:
    void tick();

    uint8_t get_next();

    uint16_t get_next_word();

    [[nodiscard]] bool is_zero() const;

    void set_zero(bool value);

    Memory memory;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;

    uint8_t flags;
};
