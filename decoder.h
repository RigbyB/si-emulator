#pragma once

#include <optional>
#include "instruction.h"

std::optional<Instruction> decode_opcode(uint8_t opcode);