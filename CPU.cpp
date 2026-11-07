#include "CPU.h"

#include <cmath>
#include <cstdint>
#include <sstream>

namespace {
double decodeFloatByte(std::uint8_t bits) noexcept {
    const bool negative = (bits & 0x80U) != 0;
    const int exponentBits = (bits >> 4) & 0x07U;
    const int exponent = exponentBits - 4; 
    const int mantissaBits = bits & 0x0FU;
    const double magnitude = std::ldexp(static_cast<double>(mantissaBits), exponent - 4);
    return negative ? -magnitude : magnitude;
}

std::uint8_t encodeFloatByte(double value) noexcept {
    if (value == 0.0) return 0;

    const bool negative = value < 0.0;
    const double magnitude = std::fabs(value);
    const std::uint8_t sign = negative ? 0x80U : 0x00U;

    
    for (int exponent = -4; exponent <= 3; ++exponent) {
        const double scaled = std::ldexp(magnitude, 4 - exponent);
        const int mantissa = static_cast<int>(std::floor(scaled + 1e-12));
        if (mantissa >= 8 && mantissa <= 15) {
            const int exponentBits = exponent + 4;
            return static_cast<std::uint8_t>(sign | (exponentBits << 4) | mantissa);
        }
    }


    const int smallMantissa = static_cast<int>(std::floor(std::ldexp(magnitude, 8) + 1e-12));
    if (smallMantissa > 0 && smallMantissa < 8) {
        return static_cast<std::uint8_t>(sign | smallMantissa);
    }
    if (smallMantissa == 0) return 0;

    
    return static_cast<std::uint8_t>(sign | 0x7FU);
}

std::string invalidMessage(std::uint16_t word, std::uint8_t address) {
    std::ostringstream out;
    out << "Invalid instruction 0x" << std::hex << word
        << " at address 0x" << static_cast<unsigned>(address) << '.';
    return out.str();
}
}

CPU::CPU() noexcept : pc_(0), ir_(0), halted_(false) {}

StepResult CPU::step(Memory& memory, RegisterFile& registers) {
    if (halted_) {
        return {StepStatus::Halted, "CPU is already halted.", pc_, ir_};
    }

    const std::uint8_t instructionAddress = pc_;
    const std::uint8_t lowByteAddress = static_cast<std::uint8_t>(pc_ + 1U);

    const std::uint16_t highByte = memory.read(instructionAddress);
    const std::uint16_t lowByte = memory.read(lowByteAddress);
    ir_ = static_cast<std::uint16_t>((highByte << 8) | lowByte);
    pc_ = static_cast<std::uint8_t>(pc_ + 2U);

    const Instruction instruction(ir_);
    if (!instruction.isValid()) {
        halted_ = true;
        return {StepStatus::InvalidInstruction,
                invalidMessage(ir_, instructionAddress), instructionAddress, ir_};
    }

    if (instruction.opcode() == 0xC) {
        halted_ = true;
        return {StepStatus::Halted, "HALT instruction executed.", instructionAddress, ir_};
    }

    execute(instruction, memory, registers);
    return {StepStatus::Executed, "Instruction executed.", instructionAddress, ir_};
}

void CPU::execute(const Instruction& instruction,
                  Memory& memory,
                  RegisterFile& registers) {
    const std::uint8_t opcode = instruction.opcode();
    const std::uint8_t r = instruction.r();
    const std::uint8_t s = instruction.s();
    const std::uint8_t t = instruction.t();
    const std::uint8_t address = instruction.address();

    switch (opcode) {
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
            const unsigned sum = static_cast<unsigned>(registers.read(s)) +
                                 static_cast<unsigned>(registers.read(t));
            registers.write(r, static_cast<std::uint8_t>(sum & 0xFFU));
            break;
        }
        case 0x6: { 
            const double left = decodeFloatByte(registers.read(s));
            const double right = decodeFloatByte(registers.read(t));
            registers.write(r, encodeFloatByte(left + right));
            break;
        }
        case 0x7: 
            registers.write(r, static_cast<std::uint8_t>(registers.read(s) | registers.read(t)));
            break;
        case 0x8: 
            registers.write(r, static_cast<std::uint8_t>(registers.read(s) & registers.read(t)));
            break;
        case 0x9: 
            registers.write(r, static_cast<std::uint8_t>(registers.read(s) ^ registers.read(t)));
            break;
        case 0xA: { 
            const unsigned count = instruction.t() % 8U;
            const std::uint8_t value = registers.read(r);
            if (count == 0U) {
                registers.write(r, value);
            } else {
                const unsigned rotated = (static_cast<unsigned>(value) >> count) |
                    (static_cast<unsigned>(value) << (8U - count));
                registers.write(r, static_cast<std::uint8_t>(rotated & 0xFFU));
            }
            break;
        }
        case 0xB: 
            if (registers.read(r) == registers.read(0)) {
                pc_ = address;
            }
            break;
        default:

            break;
    }
}

void CPU::reset(std::uint8_t startAddress) noexcept {
    pc_ = startAddress;
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
