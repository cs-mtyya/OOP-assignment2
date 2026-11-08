#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Vole_machine.h"

#include <QMainWindow>
#include <QString>

class QLineEdit;
class QLabel;
class QPushButton;
class QSpinBox;
class QTextEdit;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void chooseProgram();
    void loadProgram();
    void runProgram();
    void stepProgram();
    void resetMachine();
    void editRegister();
    void editMemory();
    void refreshStatus(const QString& message = QString());
    void setMachineLoaded(bool loaded);

    VoleMachine machine_;
    QLineEdit* programPath_;
    QLineEdit* startAddress_;
    QLineEdit* registerIndex_;
    QLineEdit* registerValue_;
    QLineEdit* memoryAddress_;
    QLineEdit* memoryValue_;
    QPushButton* runButton_;
    QPushButton* stepButton_;
    QPushButton* resetButton_;
    QPushButton* editRegisterButton_;
    QPushButton* editMemoryButton_;
    QLabel* stateBadge_;
    QTextEdit* statusView_;
    bool programLoaded_;
};

#endif