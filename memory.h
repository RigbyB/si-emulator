#pragma once

#include <string>

// TODO: This is temporary
#define MEMORY_SIZE 8192

class Memory {
public:
    void load_rom(const std::string &filename);

    uint8_t read(uint16_t address);

private:
    uint8_t contents[MEMORY_SIZE];
};