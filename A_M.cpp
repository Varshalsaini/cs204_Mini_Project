#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>

using namespace std;

// Function to convert register name to its corresponding number
int getRegisterNumber(string reg) {
    unordered_map<string, int> regMap = {
        {"x0", 0}, {"x1", 1}, {"x2", 2}, {"x3", 3},
        {"x4", 4}, {"x5", 5}, {"x6", 6}, {"x7", 7},
        {"x8", 8}, {"x9", 9}, {"x10", 10}, {"x11", 11},
        {"x12", 12}, {"x13", 13}, {"x14", 14}, {"x15", 15},
        {"x16", 16}, {"x17", 17}, {"x18", 18}, {"x19", 19},
        {"x20", 20}, {"x21", 21}, {"x22", 22}, {"x23", 23},
        {"x24", 24}, {"x25", 25}, {"x26", 26}, {"x27", 27},
        {"x28", 28}, {"x29", 29}, {"x30", 30}, {"x31", 31}
    };
    return regMap[reg];
}

// Function to convert R-type instruction to machine code
string convertRType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, destReg, srcReg1, srcReg2;
    ss >> opcode >> destReg >> srcReg1 >> srcReg2;

    int opcodeValue;
    if (opcode == "add")      opcodeValue = 0b0110011;
    else if (opcode == "and") opcodeValue = 0b0110011;
    else if (opcode == "or")  opcodeValue = 0b0110011;
    else if (opcode == "sll") opcodeValue = 0b0110011;
    else if (opcode == "slt") opcodeValue = 0b0110011;
    else if (opcode == "sra") opcodeValue = 0b0110011;
    else if (opcode == "srl") opcodeValue = 0b0110011;
    else if (opcode == "sub") opcodeValue = 0b0110011;
    else if (opcode == "xor") opcodeValue = 0b0110011;
    else if (opcode == "mul") opcodeValue = 0b0110011;
    else if (opcode == "div") opcodeValue = 0b0110011;
    else if (opcode == "rem") opcodeValue = 0b0110011;
    else return ""; // Invalid opcode

    int rd = getRegisterNumber(destReg);
    int rs1 = getRegisterNumber(srcReg1);
    int rs2 = getRegisterNumber(srcReg2);

    int funct7 = 0; // Set to 0 for now, update according to the instruction
    int machineCode = opcodeValue | (rd << 7) | (funct7 << 25) | (rs2 << 20) | (rs1 << 15);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to convert I-type instruction to machine code
string convertIType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, destReg, srcReg;
    int immediate;
    ss >> opcode >> destReg >> srcReg >> immediate;

    int opcodeValue;
    if (opcode == "addi")      opcodeValue = 0b0010011;
    else if (opcode == "andi") opcodeValue = 0b0010011;
    else if (opcode == "ori")  opcodeValue = 0b0010011;
    else if (opcode == "lb")   opcodeValue = 0b0000011;
    else if (opcode == "ld")   opcodeValue = 0b0000011;
    else if (opcode == "lh")   opcodeValue = 0b0000011;
    else if (opcode == "lw")   opcodeValue = 0b0000011;
    else if (opcode == "jalr") opcodeValue = 0b1100111;
    else return ""; // Invalid opcode

    int rd = getRegisterNumber(destReg);
    int rs1 = getRegisterNumber(srcReg);

    int machineCode = opcodeValue | (rd << 7) | (immediate << 20) | (rs1 << 15);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to convert S-type instruction to machine code
string convertSType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, srcReg, destReg;
    int immediate;
    ss >> opcode >> srcReg >> immediate >> destReg;

    int opcodeValue;
    if (opcode == "sb") opcodeValue = 0b0100011;
    else if (opcode == "sw") opcodeValue = 0b0100011;
    else if (opcode == "sd") opcodeValue = 0b0100011;
    else if (opcode == "sh") opcodeValue = 0b0100011;
    else return ""; // Invalid opcode

    int rs1 = getRegisterNumber(srcReg);
    int rs2 = getRegisterNumber(destReg);

    int imm11_5 = immediate >> 5;
    int imm4_0 = immediate & 0b11111;

    int machineCode = opcodeValue | (rs2 << 20) | (rs1 << 15) | (imm11_5 << 25) | (imm4_0 << 7);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to convert SB-type instruction to machine code
string convertSBType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, srcReg1, srcReg2;
    int labelAddress;
    ss >> opcode >> srcReg1 >> srcReg2 >> labelAddress;

    int opcodeValue;
    if (opcode == "beq") opcodeValue = 0b1100011;
    else if (opcode == "bne") opcodeValue = 0b1100011;
    else if (opcode == "bge") opcodeValue = 0b1100011;
    else if (opcode == "blt") opcodeValue = 0b1100011;
    else return ""; // Invalid opcode

    int rs1 = getRegisterNumber(srcReg1);
    int rs2 = getRegisterNumber(srcReg2);

    int offset = labelAddress - address;
    int imm12 = (offset >> 11) & 0b1;
    int imm10_5 = (offset >> 5) & 0b111111;
    int imm4_1 = (offset >> 1) & 0b1111;

    int machineCode = opcodeValue | (imm12 << 31) | (rs2 << 20) | (rs1 << 15) | (imm10_5 << 25) | (imm4_1 << 8);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to convert U-type instruction to machine code
string convertUType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, destReg;
    int immediate;
    ss >> opcode >> destReg >> immediate;

    int opcodeValue;
    if (opcode == "auipc") opcodeValue = 0b0010111;
    else if (opcode == "lui") opcodeValue = 0b0110111;
    else return ""; // Invalid opcode

    int rd = getRegisterNumber(destReg);

    int machineCode = opcodeValue | (rd << 7) | (immediate << 12);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to convert UJ-type instruction to machine code
string convertUJType(string instruction, int address) {
    stringstream ss(instruction);
    string opcode, destReg;
    int labelAddress;
    ss >> opcode >> destReg >> labelAddress;

    int opcodeValue;
    if (opcode == "jal") opcodeValue = 0b1101111;
    else return ""; // Invalid opcode

    int rd = getRegisterNumber(destReg);

    int offset = labelAddress - address;
    int imm20 = (offset >> 19) & 0b1;
    int imm10_1 = (offset >> 9) & 0b1111111111;
    int imm11 = (offset >> 10) & 0b1;
    int imm19_12 = (offset >> 11) & 0b11111111;

    int machineCode = opcodeValue | (rd << 7) | (imm20 << 31) | (imm10_1 << 21) | (imm11 << 20) | (imm19_12 << 12);

    // Convert machine code to hexadecimal string
    stringstream hexStream;
    hexStream << "0x" << hex << machineCode;
    return hexStream.str() + " " + to_string(address);
}

// Function to handle assembler directives
vector<string> handleDirectives(string directive, string operands, int &dataAddress) {
    vector<string> machineCode;
    if (directive == ".text") {
        // .text directive, do nothing
    } else if (directive == ".data") {
        // .data directive, update dataAddress
        dataAddress = stoi(operands, nullptr, 0);
    } else if (directive == ".byte" || directive == ".half" || directive == ".word" || directive == ".dword") {
        // Data directives, convert operands to machine code and update dataAddress
        stringstream ss(operands);
        string operand;
        while (getline(ss, operand, ',')) {
            int value = stoi(operand, nullptr, 0);
            if (directive == ".byte") {
                machineCode.push_back(to_string(value & 0xFF));
            } else if (directive == ".half") {
                machineCode.push_back(to_string(value & 0xFFFF));
            } else if (directive == ".word") {
                machineCode.push_back(to_string(value & 0xFFFFFFFF));
            } else if (directive == ".dword") {
                machineCode.push_back(to_string(value));
            }
            dataAddress += (directive == ".byte") ? 1 : (directive == ".half") ? 2 : (directive == ".word") ? 4 : 8;
        }
    } else if (directive == ".asciz") {
        // .asciz directive, convert string to ASCII and update dataAddress
        for (char c : operands) {
            machineCode.push_back(to_string(int(c)));
            dataAddress++;
        }
        // Add null terminator
        machineCode.push_back(to_string(0));
        dataAddress++;
    }
    return machineCode;
}

int main() {
    ifstream asmFile("input.asm");
    ofstream mcFile("output.mc");

    if (!asmFile.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    if (!mcFile.is_open()) {
        cout << "Error: Unable to open output file." << endl;
        return 1;
    }

    string line;
    int codeAddress = 0;
    int dataAddress = 0;
    while (getline(asmFile, line)) {
        if (!line.empty()) {
            // Check if it's a directive
            if (line[0] == '.') {
                stringstream ss(line);
                string directive, operands;
                ss >> directive >> operands;
                vector<string> machineCode = handleDirectives(directive, operands, dataAddress);
                for (string code : machineCode) {
                    mcFile << code << " " << to_string(dataAddress) << endl;
                }
            } else {
                // Otherwise, it's an instruction
                stringstream ss(line);
                string opcode;
                ss >> opcode;

                string machineCode;
                if (opcode == "add" || opcode == "and" || opcode == "or" || opcode == "sll" ||
                    opcode == "slt" || opcode == "sra" || opcode == "srl" || opcode == "sub" ||
                    opcode == "xor" || opcode == "mul" || opcode == "div" || opcode == "rem") {
                    machineCode = convertRType(line, codeAddress);
                } else if (opcode == "addi" || opcode == "andi" || opcode == "ori" ||
                           opcode == "lb" || opcode == "ld" || opcode == "lh" || opcode == "lw" || opcode == "jalr") {
                    machineCode = convertIType(line, codeAddress);
                } else if (opcode == "sb" || opcode == "sw" || opcode == "sd" || opcode == "sh") {
                    machineCode = convertSType(line, codeAddress);
                } else if (opcode == "beq" || opcode == "bne" || opcode == "bge" || opcode == "blt") {
                    machineCode = convertSBType(line, codeAddress);
                } else if (opcode == "auipc" || opcode == "lui") {
                    machineCode = convertUType(line, codeAddress);
                } else if (opcode == "jal") {
                    machineCode = convertUJType(line, codeAddress);
                } else {
                    cout << "Error: Invalid opcode - " << opcode << endl;
                }

                if (!machineCode.empty()) {
                    mcFile << machineCode << endl;
                    codeAddress += 4; // Assuming each instruction occupies 4 bytes
                }
            }
        }
    }

    // Write termination code to signify end of text segment and assembly program
    mcFile << "0x00000000 0x00000000" << endl;

    asmFile.close();
    mcFile.close();

    return 0;
}