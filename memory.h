#pragma once

#include <string>

#define MEMORY_SIZE 0x4000

class Memory {
public:
    void load_rom(const std::string &filename);

    uint8_t read(uint16_t address);

    uint16_t read_word(uint16_t address);

    void write(uint16_t address, uint8_t value);

    void write_word(uint16_t address, uint16_t value);

private:
    std::array<uint8_t, MEMORY_SIZE> contents;
};