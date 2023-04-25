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

    file.read(reinterpret_cast<char *>(contents.data()), size);

    // TODO: Make this region read-only
}

uint8_t Memory::read(uint16_t address) {
    // Mirrored memory
    if (address >= 0x4000) {
        address -= 0x4000;
    }

    if (address < 0 || address >= MEMORY_SIZE) {
        // TODO: Show as hex
        throw std::runtime_error("Reading address out of bounds " + std::to_string(address));
    }

    return contents[address];
}

uint16_t Memory::read_word(const uint16_t address) {
    return read(address) | (read(address + 1) << 8);
}

void Memory::write(uint16_t address, const uint8_t value) {
    // Mirrored memory
    if (address >= 0x4000) {
        address -= 0x4000;
    }

    // TODO: Show as hex
    if (address < 0 || address >= MEMORY_SIZE) {
        throw std::runtime_error("Writing address out of bounds " + std::to_string(address));
    }

    contents[address] = value;
}

void Memory::write_word(const uint16_t address, const uint16_t value) {
    write(address, value & 0xFF);
    write(address + 1, (value >> 8) & 0xFF);
}