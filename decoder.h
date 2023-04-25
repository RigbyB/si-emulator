#pragma once

#include <optional>
#include "instruction.h"

std::optional<InstructionHandler> decode_opcode(uint8_t opcode);