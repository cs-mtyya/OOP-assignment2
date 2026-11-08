#include "Vole_machine.h"
#include <iomanip>
#include <iostream>
#include <vector>

void VoleMachine::runStepMode() {
    std::cin.ignore(256, '\n'); 

    while (!cpu.isHalted()) {
        step();         
        showStatus();   

        if (cpu.isHalted()) break;
        
        std::cout << "Press Enter to execute the next step..." << std::endl;
        std::cin.get(); 
    }
    
    if (!cpu.getErrorMessage().empty()) {
        std::cerr << "CPU Error: " << cpu.getErrorMessage() << std::endl;
    } else {
        std::cout << "Program finished (Halt instruction reached)." << std::endl;
    }
}

VoleMachine::VoleMachine() {
    memory.mapDevice(0xFF, &screen);
}

bool VoleMachine::loadProgram(const std::string& fileName, uint8_t startAddress) {
    const std::vector<uint8_t> program = loader.loadFromFile(fileName);

    if (program.empty() ||
        !loader.validateStartAddress(startAddress, program.size())) {
        return false;
    }

    memory.clear();
    registers.reset();
    screen.clear();
    memory.loadProgram(startAddress, program);
    cpu.reset(startAddress);
    return true;
}

void VoleMachine::run() {
    const int MAX_STEPS = 10000;
    int stepCount = 0;

    while (!cpu.isHalted() && stepCount < MAX_STEPS) {
        step();
        stepCount++;
    }

    if (stepCount >= MAX_STEPS) {
        std::cout << "Warning: Maximum step limit (" << MAX_STEPS 
                  << ") reached. Execution halted to prevent an infinite loop.\n";
    } else if (!cpu.getErrorMessage().empty()) {
        std::cerr << "CPU Error: " << cpu.getErrorMessage() << "\n";
    }
}

void VoleMachine::step() {
    cpu.step(memory, registers);
}

void VoleMachine::reset() {
    cpu.reset();
    memory.clear();
    registers.reset();
    screen.clear();
}

void VoleMachine::showStatus() {
    std::cout << "CPU\n"
              << "PC: 0x" << std::uppercase << std::hex
              << std::setw(2) << std::setfill('0')
              << static_cast<unsigned>(cpu.getPC()) << '\n'
              << "IR: 0x" << std::setw(4)
              << static_cast<unsigned>(cpu.getIR()) << '\n'
              << "Halted: " << std::boolalpha << cpu.isHalted() << "\n\n";

    std::cout << "Registers\n";
    const auto registerValues = registers.getAllRegisters();
    for (std::size_t i = 0; i < registerValues.size(); ++i) {
        std::cout << 'R' << std::dec << i << ": 0x"
                  << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0')
                  << static_cast<unsigned>(registerValues[i]) << '\n';
    }

    std::cout << "\nMemory\n";
    for (unsigned address = 0; address < 256; ++address) {
        if (address % 16 == 0) {
            std::cout << "\n0x" << std::uppercase << std::hex
                      << std::setw(2) << std::setfill('0') << address << ": ";
        }

        std::cout << std::setw(2) << std::setfill('0')
                  << static_cast<unsigned>(
                         memory.read(static_cast<uint8_t>(address)))
                  << ' ';
    }

    std::cout << std::dec << "\n\nScreen\n"
              << screen.getOutput() << '\n';
}

void VoleMachine::editMemory(uint8_t address, uint8_t value) {
    memory.write(address, value);
}

void VoleMachine::editRegister(uint8_t id, uint8_t value) {
    registers.write(id, value);
}