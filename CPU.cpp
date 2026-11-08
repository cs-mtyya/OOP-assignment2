#include "CPU.h"

#include <cmath>
#include <sstream>

namespace {
double toNumber(std::uint8_t value) {
    bool negative = (value & 0x80) != 0;
    int exponent = ((value >> 4) & 0x07) - 4;
    int mantissa = value & 0x0F;
    double number = std::ldexp(static_cast<double>(mantissa), exponent - 4);
    return negative ? -number : number;
}

std::uint8_t toFloatByte(double number) {
    if (number == 0) return 0;

    bool negative = number < 0;
    double magnitude = std::fabs(number);
    std::uint8_t sign = negative ? 0x80 : 0;

    for (int exponent = -4; exponent <= 3; ++exponent) {
        int mantissa = static_cast<int>(
            std::floor(std::ldexp(magnitude, 4 - exponent) + 1e-12));

        if (mantissa >= 8 && mantissa <= 15) {
            int exponentBits = exponent + 4;
            return static_cast<std::uint8_t>(
                sign | (exponentBits << 4) | mantissa);
        }
    }

    int mantissa = static_cast<int>(std::floor(std::ldexp(magnitude, 8) + 1e-12));
    if (mantissa > 0 && mantissa < 8) {
        return static_cast<std::uint8_t>(sign | mantissa);
    }
    if (mantissa == 0) return 0;

    return static_cast<std::uint8_t>(sign | 0x7F);
}

std::string invalidMessage(std::uint16_t word, std::uint8_t address) {
    std::ostringstream message;
    message << "Invalid instruction 0x" << std::hex << word
            << " at address 0x" << static_cast<unsigned>(address) << '.';
    return message.str();
}
}

CPU::CPU() noexcept : pc_(0), ir_(0), halted_(false) {}

StepResult CPU::step(Memory& memory, RegisterFile& registers) {
    if (halted_) {
        return {StepStatus::Halted, "CPU is already halted.", pc_, ir_};
    }

    std::uint8_t instructionAddress = pc_;
    std::uint8_t nextByteAddress = static_cast<std::uint8_t>(pc_ + 1);
    std::uint16_t highByte = memory.read(instructionAddress);
    std::uint16_t lowByte = memory.read(nextByteAddress);

    ir_ = static_cast<std::uint16_t>((highByte << 8) | lowByte);
    pc_ = static_cast<std::uint8_t>(pc_ + 2);

    Instruction instruction(ir_);
        if (!instruction.isValid()) {
        halted_ = true;
        errorMessage = invalidMessage(ir_, instructionAddress);   // <-- ADD THIS LINE
        return {StepStatus::InvalidInstruction,
                invalidMessage(ir_, instructionAddress), instructionAddress, ir_};
    }

    if (instruction.opcode() == 0xC) {
        halted_ = true;
        return {StepStatus::Halted, "HALT instruction executed.",
                instructionAddress, ir_};
    }

    execute(instruction, memory, registers);
    return {StepStatus::Executed, "Instruction executed.", instructionAddress, ir_};
}

void CPU::execute(const Instruction& instruction,
    Memory& memory,
    RegisterFile& registers) {
        std::uint8_t r = instruction.r();
        std::uint8_t s = instruction.s();
        std::uint8_t t = instruction.t();
        std::uint8_t address = instruction.address();

    switch (instruction.opcode()) {
        case 0x1:
            registers.write(r, memory.read(address));
            break;
        case 0x2:
            registers.write(r, address);
            break;
        case 0x3:
            memory.write(address, registers.read(r));
            break;
        case 0x4:
            registers.write(t, registers.read(s));
            break;
        case 0x5: {
            unsigned sum = registers.read(s) + registers.read(t);
            registers.write(r, static_cast<std::uint8_t>(sum));
            break;
        }
        case 0x6: {
            double sum = toNumber(registers.read(s)) + toNumber(registers.read(t));
            registers.write(r, toFloatByte(sum));
            break;
        }
        case 0x7:
            registers.write(r, static_cast<std::uint8_t>(
                registers.read(s) | registers.read(t)));
            break;
        case 0x8:
            registers.write(r, static_cast<std::uint8_t>(
                registers.read(s) & registers.read(t)));
            break;
        case 0x9:
            registers.write(r, static_cast<std::uint8_t>(
                registers.read(s) ^ registers.read(t)));
            break;
        case 0xA: {
            unsigned count = t % 8;
            std::uint8_t value = registers.read(r);
            unsigned rotated = (value >> count) | (value << (8 - count));
            registers.write(r, static_cast<std::uint8_t>(rotated));
            break;
        }
        case 0xB:
            if (registers.read(r) == registers.read(0)) {
                pc_ = address;
            }
            break;
    }
}

void CPU::reset(std::uint8_t startAddress) noexcept {
    pc_ = startAddress;
    errorMessage.clear();   
    ir_ = 0;
    halted_ = false;
}

std::uint8_t CPU::getPC() const noexcept {
    return pc_;
}

std::uint16_t CPU::getIR() const noexcept {
    return ir_;
}

bool CPU::isHalted() const noexcept {
    return halted_;
}
