#include "memory.h"

#include <fstream>

void Memory::load_rom(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file \"" + filename + "\".");
    }

    const std::streamsize size = file.tellg();

    if (size > MEMORY_SIZE) {
        throw std::runtime_error("File \"" + filename + "\" is too large to fit in Memory.");
    }

    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char *>(contents), size);

    // TODO: Make this region read-only
}

uint8_t Memory::read(const uint16_t address) {
    return contents[address];
}