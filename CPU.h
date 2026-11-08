#ifndef VOLE_CPU_H
#define VOLE_CPU_H

#include "Instruction.h"
#include "Memory.h"
#include "RegisterFile.h"

#include <cstdint>
#include <string>

enum class StepStatus {
    Executed,
    Halted,
    InvalidInstruction
};

struct StepResult {
    StepStatus status;
    std::string message;
    std::uint8_t instructionAddress;
    std::uint16_t instructionWord;
};

class CPU {
public:
    CPU() noexcept;
    std::string getErrorMessage() const { return errorMessage; };
    StepResult step(Memory& memory, RegisterFile& registers);
    void reset(std::uint8_t startAddress = 0) noexcept;

    std::uint8_t getPC() const noexcept;
    std::uint16_t getIR() const noexcept;
    bool isHalted() const noexcept;

private:
std::string errorMessage;
    std::uint8_t pc_;
    std::uint16_t ir_;
    bool halted_;

    void execute(const Instruction& instruction,
                 Memory& memory,
                 RegisterFile& registers);
};

#endif
