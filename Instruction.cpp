#include "Instruction.h"

#include <cctype>

namespace {
int hexDigit(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    return -1;
}
}

Instruction::Instruction(std::uint16_t rawWord) noexcept : rawWord_(rawWord) {}

bool Instruction::parseHexWord(const std::string& line,
                               Instruction& result,
                               std::string* errorMessage) {
    std::size_t start = 0;
    while (start < line.size() &&
           std::isspace(static_cast<unsigned char>(line[start]))) {
        ++start;
    }

    if (start == line.size() || line[start] == ';') {
        if (errorMessage) *errorMessage = "No instruction found.";
        return false;
    }

    if (line.size() - start < 4) {
        if (errorMessage) *errorMessage = "Instruction needs four hex digits.";
        return false;
    }

    std::uint16_t word = 0;
    for (int i = 0; i < 4; ++i) {
        int digit = hexDigit(line[start + i]);
        if (digit == -1) {
            if (errorMessage) *errorMessage = "Instruction needs four hex digits.";
            return false;
        }
        word = static_cast<std::uint16_t>((word << 4) | digit);
    }

    result = Instruction(word);
    if (errorMessage) errorMessage->clear();
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
    return static_cast<std::uint8_t>(rawWord_ & 0xFF);
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
            return (rawWord_ & 0x0FFF) == 0;
        default:
            return false;
    }
}
