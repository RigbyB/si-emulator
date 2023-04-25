#pragma once

#include <string>

// TODO: This is temporary
#define MEMORY_SIZE 8192

class Memory {
public:
    void load_rom(const std::string &filename);

    uint8_t read(uint16_t address);

    uint16_t read_word(uint16_t address);

private:
    std::array<uint8_t, MEMORY_SIZE> contents;
};