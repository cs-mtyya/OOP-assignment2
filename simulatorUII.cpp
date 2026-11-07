#include "SimulatorUI.h"
#include <iostream>
#include <string>

void SimulatorUI::displayMenu() {
    std::cout << "\n VOLE MACHINE SIMULATOR\n";
    std::cout << "1. Load Program from File\n";
    std::cout << "2. Overwrite Register or Memory Cell\n";
    std::cout << "3. Run Program\n";
    std::cout << "4. Exit\n";
    std::cout << "Select an option: ";
}

void SimulatorUI::getUserInput() {
    int choice;
    displayMenu();
    std::cin >> choice;

    if (choice == 1) {
        std::string filename;
        int address;
        std::cout << "Enter program filename: ";
        std::cin >> filename;
        std::cout << "Enter start memory address (Hex, even number): ";
        std::cin >> std::hex >> address;
        
        
        machine.loadProgram(filename, address);
    } else if (choice == 3) {
        startExecution();
    }
}

void SimulatorUI::showMachineState() {
    std::cout << "\n CPU & Memory State\n";
    std::cout << "PC: " << std::hex << machine.getPC() 
              << " | IR: " << machine.getIR() << "\n";
    
   
}

void SimulatorUI::startExecution() {
    int mode;
    std::cout << "Execution Mode (1: Run All, 2: Step-by-Step): ";
    std::cin >> mode;

    while (machine.isRunning()) {
        machine.executeStep();
        
        if (mode == 2) {
            showMachineState(); 
            std::cout << "Press Enter to execute next instruction...";
            std::cin.ignore();
            std::cin.get();
        }
    }

    if (mode == 1) {
        showMachineState(); 
    }
}