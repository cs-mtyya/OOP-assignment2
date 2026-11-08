#ifndef ALU_H
#define ALU_H
#include <cstdint>
class ALU {
public:
    uint8_t executeOperation(uint8_t opcode, uint8_t a, uint8_t b);
};

#endif