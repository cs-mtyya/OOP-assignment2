#include "RegisterFile.h"


RegisterFile::RegisterFile() {
    reset();
}

void RegisterFile::reset() {
    registers.fill(0);
}

uint8_t RegisterFile::read(uint8_t reg) {
    if (reg < 16) {
        return registers[reg];
    }
    return 0; 
}

void RegisterFile::write(uint8_t reg, uint8_t value) {
    if (reg < 16) {
        registers[reg] = value;
    }
}

std::array<uint8_t,16> RegisterFile::getAllRegisters() const {
    return registers;
}