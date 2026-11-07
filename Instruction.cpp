#include "Instruction.h"

#include <cctype>

namespace {
int hexValue(char ch) noexcept {
    const unsigned char value = static_cast<unsigned char>(ch);
    if (value >= '0' && value <= '9') return value - '0';
    if (value >= 'a' && value <= 'f') return value - 'a' + 10;
    if (value >= 'A' && value <= 'F') return value - 'A' + 10;
    return -1;
}

void setError(std::string* output, const std::string& message) {
    if (output != nullptr) *output = message;
}
}

Instruction::Instruction(std::uint16_t rawWord) noexcept : rawWord_(rawWord) {}

bool Instruction::parseHexWord(const std::string& line,
                               Instruction& result,
                               std::string* errorMessage) {
    std::size_t pos = 0;
    while (pos < line.size() &&
           std::isspace(static_cast<unsigned char>(line[pos])) != 0) {
        ++pos;
    }

    if (pos == line.size() || line[pos] == ';') {
        setError(errorMessage, "The line does not contain an instruction.");
        return false;
    }
    if (line.size() - pos < 4) {
        setError(errorMessage, "An instruction must begin with four hex digits.");
        return false;
    }

    std::uint16_t word = 0;
    for (std::size_t i = 0; i < 4; ++i) {
        const int digit = hexValue(line[pos + i]);
        if (digit < 0) {
            setError(errorMessage, "An instruction must begin with four hex digits.");
            return false;
        }
        word = static_cast<std::uint16_t>((word << 4) | digit);
    }

    result = Instruction(word);
    if (errorMessage != nullptr) errorMessage->clear();
    return true;
}

std::uint16_t Instruction::rawWord() const noexcept {
    return rawWord_;
}

std::uint8_t Instruction::opcode() const noexcept {
    return static_cast<std::uint8_t>((rawWord_ >> 12) & 0x0F);
}

std::uint8_t Instruction::r() const noexcept {
    return static_cast<std::uint8_t>((rawWord_ >> 8) & 0x0F);
}

std::uint8_t Instruction::s() const noexcept {
    return static_cast<std::uint8_t>((rawWord_ >> 4) & 0x0F);
}

std::uint8_t Instruction::t() const noexcept {
    return static_cast<std::uint8_t>(rawWord_ & 0x0F);
}

std::uint8_t Instruction::address() const noexcept {
    return static_cast<std::uint8_t>(rawWord_ & 0x00FF);
}

bool Instruction::isValid() const noexcept {
    switch (opcode()) {
        case 0x1: 
        case 0x2: 
        case 0x3: 
        case 0x5: 
        case 0x6: 
        case 0x7: 
        case 0x8: 
        case 0x9: 
        case 0xB: 
            return true;
        case 0x4: 
            return r() == 0;
        case 0xA: 
            return s() == 0;
        case 0xC: 
            return r() == 0 && s() == 0 && t() == 0;
        default:
            return false;
    }
}
