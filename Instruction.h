#ifndef VOLE_INSTRUCTION_H
#define VOLE_INSTRUCTION_H

#include <cstdint>
#include <string>

class Instruction {
public:
    explicit Instruction(std::uint16_t rawWord = 0) noexcept;

    static bool parseHexWord(const std::string& line,
                             Instruction& result,
                             std::string* errorMessage = nullptr);

    std::uint16_t rawWord() const noexcept;
    std::uint8_t opcode() const noexcept;
    std::uint8_t r() const noexcept; 
    std::uint8_t s() const noexcept; 
    std::uint8_t t() const noexcept; 
    std::uint8_t address() const noexcept; 
    bool isValid() const noexcept;

private:
    std::uint16_t rawWord_;
};

#endif
