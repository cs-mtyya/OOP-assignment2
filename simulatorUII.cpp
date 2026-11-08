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
while (true) {
        int choice;
        displayMenu();

        if (!(std::cin >> choice)) {
            return;
        }

        if (choice == 1) {
            std::string name;
            int address;
            std::cout << "Enter program name: ";
            std::cin >> name;
            std::cout << "Enter memory address (Hex, even number): ";
            std::cin >> std::hex >> address >> std::dec;

            machine.loadProgram(name, address);
        } else if (choice == 2) {
            std::cout << "Overwrite is not implemented yet.\n";
        } else if (choice == 3) {
            startExecution();
        } else if (choice == 4) {
            return;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}
void SimulatorUI::showMachineState() {
    std::cout<< "\nCPU & Memory State\n";
    machine.showStatus();
}
void SimulatorUI::startExecution() {
    int mode;
    std::cout<< "Mode (1-Run All, 2-Step by Step): ";
    std::cin>> mode;
    if (mode == 1) {
        machine.run();
        showMachineState(); 
    } else if (mode == 2) {
        machine.step();
        showMachineState();
        std::cout<< "Press enter to execute";
        std::cin.ignore();
        std::cin.get();
    }
}