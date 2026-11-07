#include "SimulatorUI.h"
#include <iostream>
#include <string>

void SimulatorUI::displayMenu() {
    std::cout<< "\n Vole Machine Sim\n";
    std::cout<< "1- Load Program\n";
    std::cout<< "2- Overwrite\n";
    std::cout<< "3- Run Program\n";
    std::cout<< "4- Exit\n";
    std::cout<< "Choose:";
}
void SimulatorUI::getUserInput() {
    int choice;
    displayMenu();
    std::cin >> choice;
    if (choice == 1) {
        std::string name;
        int address;
        std::cout<< "Enter program name:";
        std::cin>> name;
        std::cout<< "Enter memory address (Hex,even number):";
        std::cin>> std::hex >> address;
        
        
        machine.loadProgram(name, address);
    } else if (choice == 3) {
        startExecution();
    }
}
void SimulatorUI::showMachineState() {
    std::cout<< "\nCPU & Memory State\n";
    std::cout<< "PC: " << std::hex << machine.getPC() 
            << " | IR: " << machine.getIR() << "\n";
    
   
}
void SimulatorUI::startExecution() {
    int mode;
    std::cout<< "Mode (1-Run All, 2-Step by Step): ";
    std::cin>> mode;
    while (machine.isRunning()) {
        machine.executeStep();
        
        if (mode == 2) {
            showMachineState(); 
            std::cout<< "Press enter to execute";
            std::cin.ignore();
            std::cin.get();
        }
    }
    if (mode == 1) {
        showMachineState(); 
    }
}