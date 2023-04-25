#pragma once

#include <cstdint>

class CPU;

using InstructionHandler = void (*)(CPU &);