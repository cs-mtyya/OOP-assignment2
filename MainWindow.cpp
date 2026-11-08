#include "MainWindow.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

namespace {
class ConsoleCapture {
public:
    explicit ConsoleCapture(bool captureOutput = true, bool captureErrors = true)
        : oldOutput_(captureOutput ? std::cout.rdbuf(buffer_.rdbuf()) : nullptr),
          oldErrors_(captureErrors ? std::cerr.rdbuf(buffer_.rdbuf()) : nullptr) {}

    ~ConsoleCapture() {
        if (oldOutput_) {
            std::cout.rdbuf(oldOutput_);
        }
        if (oldErrors_) {
            std::cerr.rdbuf(oldErrors_);
        }
    }

    std::string text() const {
        return buffer_.str();
    }

private:
    std::ostringstream buffer_;
    std::streambuf* oldOutput_;
    std::streambuf* oldErrors_;
};

bool parseHexValue(const QLineEdit* input, unsigned maximum, unsigned& value) {
    bool valid = false;
    const unsigned parsed = input->text().trimmed().toUInt(&valid, 16);
    if (!valid || parsed > maximum) {
        return false;
    }
    value = parsed;
    return true;
}

QLineEdit* makeHexInput(const QString& initialValue, int maxLength) {
    auto* input = new QLineEdit(initialValue);
    input->setMaxLength(maxLength);
    input->setFixedWidth(76);
    input->setAlignment(Qt::AlignRight);
    return input;
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      programPath_(nullptr),
      startAddress_(nullptr),
      registerIndex_(nullptr),
      registerValue_(nullptr),
      memoryAddress_(nullptr),
      memoryValue_(nullptr),
      runButton_(nullptr),
      stepButton_(nullptr),
      resetButton_(nullptr),
      editRegisterButton_(nullptr),
      editMemoryButton_(nullptr),
      stateBadge_(nullptr),
      statusView_(nullptr),
      programLoaded_(false) {
    setWindowTitle(QStringLiteral("Vole Machine Simulator"));
    setMinimumSize(1040, 700);
    resize(1280, 820);

    auto* central = new QWidget(this);
    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(24, 20, 24, 22);
    root->setSpacing(18);

    auto* topBar = new QHBoxLayout;
    auto* logo = new QLabel(QStringLiteral("V"), central);
    logo->setObjectName(QStringLiteral("logoMark"));
    logo->setAlignment(Qt::AlignCenter);
    logo->setFixedSize(48, 48);
    auto* brandLayout = new QVBoxLayout;
    brandLayout->setSpacing(1);
    auto* heading = new QLabel(QStringLiteral("VOLE MACHINE"), central);
    heading->setObjectName(QStringLiteral("heading"));
    auto* subtitle = new QLabel(
        QStringLiteral("16-REGISTER MICROPROCESSOR  /  SIMULATOR CONSOLE"), central);
    subtitle->setObjectName(QStringLiteral("subtitle"));
    brandLayout->addWidget(heading);
    brandLayout->addWidget(subtitle);
    stateBadge_ = new QLabel(QStringLiteral("●  STANDBY"), central);
    stateBadge_->setObjectName(QStringLiteral("stateBadge"));
    stateBadge_->setAlignment(Qt::AlignCenter);
    topBar->addWidget(logo);
    topBar->addLayout(brandLayout);
    topBar->addStretch();
    topBar->addWidget(stateBadge_);
    root->addLayout(topBar);

    auto* workspace = new QHBoxLayout;
    workspace->setSpacing(18);
    auto* controlColumn = new QVBoxLayout;
    controlColumn->setSpacing(14);

    auto* programGroup = new QGroupBox(QStringLiteral("01   /   LOAD PROGRAM"), central);
    programGroup->setObjectName(QStringLiteral("panel"));
    auto* programLayout = new QVBoxLayout(programGroup);
    programLayout->setSpacing(10);
    programPath_ = new QLineEdit(programGroup);
    programPath_->setPlaceholderText(QStringLiteral("Select a Vole program (.txt)"));
    auto* browseButton = new QPushButton(QStringLiteral("Browse files  ↗"), programGroup);
    auto* fileRow = new QHBoxLayout;
    fileRow->addWidget(programPath_, 1);
    fileRow->addWidget(browseButton);
    auto* addressLabel = new QLabel(QStringLiteral("START ADDRESS  /  HEX"), programGroup);
    addressLabel->setObjectName(QStringLiteral("fieldLabel"));
    startAddress_ = makeHexInput(QStringLiteral("00"), 2);
    startAddress_->setToolTip(QStringLiteral("Hexadecimal start address, 00–FE"));
    auto* loadButton = new QPushButton(QStringLiteral("↓   LOAD INTO MEMORY"), programGroup);
    loadButton->setObjectName(QStringLiteral("primaryButton"));
    loadButton->setMinimumHeight(42);
    programLayout->addLayout(fileRow);
    programLayout->addWidget(addressLabel);
    programLayout->addWidget(startAddress_, 0, Qt::AlignLeft);
    programLayout->addWidget(loadButton);
    controlColumn->addWidget(programGroup);

    auto* executionGroup = new QGroupBox(QStringLiteral("02   /   EXECUTION"), central);
    executionGroup->setObjectName(QStringLiteral("panel"));
    auto* executionLayout = new QVBoxLayout(executionGroup);
    executionLayout->setSpacing(9);
    runButton_ = new QPushButton(QStringLiteral("▶   RUN PROGRAM"), executionGroup);
    runButton_->setObjectName(QStringLiteral("runButton"));
    stepButton_ = new QPushButton(QStringLiteral("▸   STEP ONE INSTRUCTION"), executionGroup);
    resetButton_ = new QPushButton(QStringLiteral("↺   RESET MACHINE"), executionGroup);
    runButton_->setMinimumHeight(44);
    stepButton_->setMinimumHeight(40);
    resetButton_->setMinimumHeight(40);
    executionLayout->addWidget(runButton_);
    executionLayout->addWidget(stepButton_);
    executionLayout->addWidget(resetButton_);
    controlColumn->addWidget(executionGroup);

    auto* editGroup = new QGroupBox(QStringLiteral("03   /   EDIT REGISTER / MEMORY"), central);
    editGroup->setObjectName(QStringLiteral("panel"));
    auto* editLayout = new QVBoxLayout(editGroup);
    editLayout->setSpacing(11);
    auto* editHint = new QLabel(QStringLiteral("Enter hexadecimal values, then press Apply."), editGroup);
    editHint->setObjectName(QStringLiteral("editHint"));
    editLayout->addWidget(editHint);

    auto* registerForm = new QGridLayout;
    registerForm->setHorizontalSpacing(8);
    registerForm->setVerticalSpacing(5);
    registerIndex_ = makeHexInput(QString(), 1);
    registerIndex_->setPlaceholderText(QStringLiteral("0–F"));
    registerIndex_->setToolTip(QStringLiteral("Register number in hexadecimal, from 0 to F"));
    registerValue_ = makeHexInput(QString(), 2);
    registerValue_->setPlaceholderText(QStringLiteral("00–FF"));
    registerValue_->setToolTip(QStringLiteral("New register value in hexadecimal"));
    editRegisterButton_ = new QPushButton(QStringLiteral("APPLY"), editGroup);
    registerForm->addWidget(new QLabel(QStringLiteral("Register index (hex)"), editGroup), 0, 0);
    registerForm->addWidget(new QLabel(QStringLiteral("New value (hex)"), editGroup), 0, 1);
    registerForm->addWidget(registerIndex_, 1, 0);
    registerForm->addWidget(registerValue_, 1, 1);
    registerForm->addWidget(editRegisterButton_, 1, 2);

    auto* memoryForm = new QGridLayout;
    memoryForm->setHorizontalSpacing(8);
    memoryForm->setVerticalSpacing(5);
    memoryAddress_ = makeHexInput(QString(), 2);
    memoryAddress_->setPlaceholderText(QStringLiteral("00–FF"));
    memoryAddress_->setToolTip(QStringLiteral("Memory address in hexadecimal, from 00 to FF"));
    memoryValue_ = makeHexInput(QString(), 2);
    memoryValue_->setPlaceholderText(QStringLiteral("00–FF"));
    memoryValue_->setToolTip(QStringLiteral("Byte to write at that address, in hexadecimal"));
    editMemoryButton_ = new QPushButton(QStringLiteral("APPLY"), editGroup);
    memoryForm->addWidget(new QLabel(QStringLiteral("Memory address (hex)"), editGroup), 0, 0);
    memoryForm->addWidget(new QLabel(QStringLiteral("New value (hex)"), editGroup), 0, 1);
    memoryForm->addWidget(memoryAddress_, 1, 0);
    memoryForm->addWidget(memoryValue_, 1, 1);
    memoryForm->addWidget(editMemoryButton_, 1, 2);
    editLayout->addLayout(registerForm);
    editLayout->addLayout(memoryForm);
    controlColumn->addWidget(editGroup);
    controlColumn->addStretch();
    workspace->addLayout(controlColumn, 4);

    auto* monitorGroup = new QGroupBox(QStringLiteral("LIVE MACHINE MONITOR"), central);
    monitorGroup->setObjectName(QStringLiteral("monitorPanel"));
    auto* monitorLayout = new QVBoxLayout(monitorGroup);
    monitorLayout->setContentsMargins(14, 14, 14, 14);
    auto* monitorHint = new QLabel(
        QStringLiteral("CPU  /  REGISTERS  /  256-BYTE MEMORY  /  SCREEN"), monitorGroup);
    monitorHint->setObjectName(QStringLiteral("monitorHint"));
    statusView_ = new QTextEdit(monitorGroup);
    statusView_->setObjectName(QStringLiteral("terminal"));
    statusView_->setReadOnly(true);
    statusView_->setLineWrapMode(QTextEdit::NoWrap);
    statusView_->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    statusView_->setMinimumWidth(530);
    monitorLayout->addWidget(monitorHint);
    monitorLayout->addWidget(statusView_, 1);
    workspace->addWidget(monitorGroup, 7);
    root->addLayout(workspace, 1);

    setCentralWidget(central);
    setStyleSheet(QStringLiteral(
        "QMainWindow, QWidget { background: #090d18; color: #e6edf7; font-family: 'Segoe UI'; font-size: 10pt; }"
        "QLabel#logoMark { color: #07111b; font-size: 27px; font-weight: 900; border-radius: 14px;"
        " background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #5eead4, stop:1 #22d3ee); }"
        "QLabel#heading { color: #f2f7ff; font-size: 21px; font-weight: 800; letter-spacing: 3px; }"
        "QLabel#subtitle, QLabel#monitorHint { color: #76869f; font-size: 9px; letter-spacing: 1px; }"
        "QLabel#stateBadge { color: #67e8c1; background: #10251f; border: 1px solid #1e5143;"
        " border-radius: 12px; padding: 8px 14px; font-size: 9px; font-weight: 800; letter-spacing: 1px; }"
        "QGroupBox#panel, QGroupBox#monitorPanel { background: #0e1422; border: 1px solid #202b3e;"
        " border-radius: 12px; margin-top: 13px; padding: 14px; font-size: 9px;"
        " font-weight: 800; letter-spacing: 1px; color: #93a4bd; }"
        "QGroupBox#monitorPanel { background: #0b111d; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 14px; padding: 0 7px; }"
        "QLineEdit { background: #090f1b; border: 1px solid #27344a; border-radius: 7px;"
        " padding: 9px 10px; color: #d5e5f7; selection-background-color: #14b8a6; }"
        "QLineEdit:focus { border: 1px solid #2dd4bf; }"
        "QLabel#fieldLabel { color: #8292aa; font-size: 8px; font-weight: 800; letter-spacing: 1px; }"
        "QLabel#editHint { color: #8292aa; font-size: 9px; font-weight: 400; letter-spacing: 0px; }"
        "QTextEdit#terminal { background: #070b13; color: #9debd9; border: 1px solid #1d2b37;"
        " border-radius: 8px; padding: 14px; selection-background-color: #0e7490; font-size: 10pt; }"
        "QPushButton { background: #182235; border: 1px solid #2b3a51; border-radius: 7px;"
        " padding: 9px 12px; color: #c9d6e8; font-weight: 700; }"
        "QPushButton:hover { background: #223149; border-color: #3a526f; color: #ffffff; }"
        "QPushButton:pressed { background: #142033; }"
        "QPushButton:disabled { color: #536176; background: #111827; border-color: #202a39; }"
        "QPushButton#primaryButton { color: #041512; background: #5eead4; border: none; font-weight: 900; }"
        "QPushButton#primaryButton:hover { background: #8af3df; }"
        "QPushButton#runButton { color: #06120e; background: #34d399; border: none; font-weight: 900; }"
        "QPushButton#runButton:hover { background: #6ee7b7; }"));

    connect(browseButton, &QPushButton::clicked, this, [this] { chooseProgram(); });
    connect(loadButton, &QPushButton::clicked, this, [this] { loadProgram(); });
    connect(runButton_, &QPushButton::clicked, this, [this] { runProgram(); });
    connect(stepButton_, &QPushButton::clicked, this, [this] { stepProgram(); });
    connect(resetButton_, &QPushButton::clicked, this, [this] { resetMachine(); });
    connect(editRegisterButton_, &QPushButton::clicked, this, [this] { editRegister(); });
    connect(editMemoryButton_, &QPushButton::clicked, this, [this] { editMemory(); });

    setMachineLoaded(false);
    refreshStatus(QStringLiteral("Ready. Load a program to begin."));
}

void MainWindow::chooseProgram() {
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("Open Vole program"), QString(),
        QStringLiteral("Text files (*.txt);;All files (*)"));
    if (!path.isEmpty()) {
        programPath_->setText(path);
    }
}

void MainWindow::loadProgram() {
    unsigned address = 0;
    if (!parseHexValue(startAddress_, 0xFF, address)) {
        QMessageBox::warning(this, QStringLiteral("Invalid start address"),
                             QStringLiteral("Enter a hexadecimal address from 00 to FF."));
        return;
    }
    if (programPath_->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("No program selected"),
                             QStringLiteral("Choose a program file before loading."));
        return;
    }

    bool loaded = false;
    std::string diagnostics;
    {
        ConsoleCapture capture(false, true);
        const QByteArray localPath = programPath_->text().toLocal8Bit();
        loaded = machine_.loadProgram(localPath.toStdString(),
                                      static_cast<std::uint8_t>(address));
        diagnostics = capture.text();
    }

    setMachineLoaded(loaded);
    if (loaded) {
        refreshStatus(QStringLiteral("Loaded: %1 at 0x%2")
                          .arg(programPath_->text(),
                               QString::number(address, 16)
                                   .rightJustified(2, QLatin1Char('0')).toUpper()));
    } else {
        QString message = QStringLiteral("Program could not be loaded.");
        if (!diagnostics.empty()) {
            message += QLatin1Char('\n') + QString::fromLocal8Bit(diagnostics.data(),
                                                                  static_cast<int>(diagnostics.size()));
        }
        refreshStatus(message);
        QMessageBox::warning(this, QStringLiteral("Load failed"), message);
    }
}

void MainWindow::runProgram() {
    if (!programLoaded_) {
        return;
    }
    std::string output;
    {
        ConsoleCapture capture;
        machine_.run();
        output = capture.text();
    }
    QString message = QStringLiteral("Execution finished.");
    if (!output.empty()) {
        message += QLatin1Char('\n') + QString::fromLocal8Bit(output.data(),
                                                               static_cast<int>(output.size())).trimmed();
    }
    refreshStatus(message);
}

void MainWindow::stepProgram() {
    if (!programLoaded_) {
        return;
    }
    machine_.step();
    refreshStatus(QStringLiteral("Executed one instruction."));
}

void MainWindow::resetMachine() {
    machine_.reset();
    setMachineLoaded(false);
    refreshStatus(QStringLiteral("Machine reset. Load a program to execute again."));
}

void MainWindow::editRegister() {
    unsigned index = 0;
    unsigned value = 0;
    if (!parseHexValue(registerIndex_, 0x0F, index) ||
        !parseHexValue(registerValue_, 0xFF, value)) {
        QMessageBox::warning(this, QStringLiteral("Invalid register value"),
                             QStringLiteral("Register index must be 0–F and value must be 00–FF (hex)."));
        return;
    }
    machine_.editRegister(static_cast<std::uint8_t>(index),
                          static_cast<std::uint8_t>(value));
    refreshStatus(QStringLiteral("Set R%1 to 0x%2.")
                      .arg(QString::number(index, 16).toUpper(),
                           QString::number(value, 16)
                               .rightJustified(2, QLatin1Char('0')).toUpper()));
}

void MainWindow::editMemory() {
    unsigned address = 0;
    unsigned value = 0;
    if (!parseHexValue(memoryAddress_, 0xFF, address) ||
        !parseHexValue(memoryValue_, 0xFF, value)) {
        QMessageBox::warning(this, QStringLiteral("Invalid memory value"),
                             QStringLiteral("Memory address and value must be hexadecimal bytes (00–FF)."));
        return;
    }
    machine_.editMemory(static_cast<std::uint8_t>(address),
                        static_cast<std::uint8_t>(value));
    refreshStatus(QStringLiteral("Set memory 0x%1 to 0x%2.")
                      .arg(QString::number(address, 16)
                               .rightJustified(2, QLatin1Char('0')).toUpper(),
                           QString::number(value, 16)
                               .rightJustified(2, QLatin1Char('0')).toUpper()));
}

void MainWindow::refreshStatus(const QString& message) {
    std::string status;
    {
        ConsoleCapture capture;
        machine_.showStatus();
        status = capture.text();
    }

    QString viewText;
    if (!message.isEmpty()) {
        viewText = message + QStringLiteral("\n\n");
    }
    viewText += QString::fromLocal8Bit(status.data(), static_cast<int>(status.size()));
    statusView_->setPlainText(viewText);

    const bool halted = viewText.contains(QStringLiteral("Halted: true"), Qt::CaseInsensitive);
    if (halted) {
        stateBadge_->setText(QStringLiteral("■  HALTED"));
        stateBadge_->setStyleSheet(QStringLiteral(
            "color: #fca5a5; background: #2a171c; border: 1px solid #63313a;"
            " border-radius: 12px; padding: 8px 14px; font-size: 9px; font-weight: 800; letter-spacing: 1px;"));
    } else if (programLoaded_) {
        stateBadge_->setText(QStringLiteral("●  PROGRAM LOADED"));
        stateBadge_->setStyleSheet(QStringLiteral(
            "color: #67e8c1; background: #10251f; border: 1px solid #1e5143;"
            " border-radius: 12px; padding: 8px 14px; font-size: 9px; font-weight: 800; letter-spacing: 1px;"));
    } else {
        stateBadge_->setText(QStringLiteral("●  STANDBY"));
        stateBadge_->setStyleSheet(QStringLiteral(
            "color: #a5b4fc; background: #191a32; border: 1px solid #35365f;"
            " border-radius: 12px; padding: 8px 14px; font-size: 9px; font-weight: 800; letter-spacing: 1px;"));
    }
    runButton_->setEnabled(programLoaded_ && !halted);
    stepButton_->setEnabled(programLoaded_ && !halted);
}

void MainWindow::setMachineLoaded(bool loaded) {
    programLoaded_ = loaded;
    runButton_->setEnabled(loaded);
    stepButton_->setEnabled(loaded);
    resetButton_->setEnabled(loaded);
    editRegisterButton_->setEnabled(loaded);
    editMemoryButton_->setEnabled(loaded);
}
