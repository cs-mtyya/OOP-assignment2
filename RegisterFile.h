#ifndef RegisterFile_H
#define RegisterFile_H
#include <cstdint>
#include <array>



class RegisterFile {
    private:
        std::array<uint8_t,16> registers;
    
    public:
    RegisterFile();

    uint8_t read(uint8_t index);
    void write(uint8_t index, uint8_t value);

    void reset();
    std::array<uint8_t, 16> getAllRegisters() const;
};

#endif