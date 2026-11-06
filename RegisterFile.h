#ifndef RegisterFile_H
#define RegisterFile_H
#include <cstdint>

class RegisterFile {
    private:

    uint8_t registers[16];
    
    public:

    uint8_t read(uint8_t index);
    void write(uint8_t index, uint8_t value);
};

#endif