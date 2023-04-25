#pragma once

#include <string>

// TODO: This is temporary
#define MEMORY_SIZE 8192

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