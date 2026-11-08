#include "ALU.h"
uint8_t ALU::executeOperation(uint8_t opcode, uint8_t a, uint8_t b) {
    if (opcode == 0x5) {
        return a + b;
    }
    else if (opcode == 0x6) {
        return a + b;
    }
    else if (opcode == 0x7) {
        return a | b;
    }
    else if (opcode == 0x8) {
        return a & b;
    }
    else if (opcode == 0x9) {
        return a ^ b;
    }
    else if (opcode == 0xA) {
        uint8_t steps = b % 8;
        return (a >> steps) | (a << (8 - steps));
    }
    return 0;
}