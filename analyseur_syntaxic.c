//
// Created by Arsan Abdi  on 28/01/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define NUM_REGISTERS 32
uint16_t registers[NUM_REGISTERS] = {0}; // Initialisation de tous les registres à 0

typedef enum {
    ADD, SUB, MUL, DIV, AND, OR, XOR, SHL, MOV, UNKNOWN
} InstructionType;

typedef struct {
    InstructionType type;
    uint8_t rd;
    uint8_t rn;
    int16_t operand;
} Instruction;

int parse_register_number(const char* reg) {
    if (reg[0] == 'R' || reg[0] == 'r') {
        return atoi(reg + 1);
    }
    return -1; // Erreur de format
}

// Implémentation de la fonction parse_operand
int16_t parse_operand(const char* op) {
    if (op[0] == 'R' || op[0] == 'r') {
        int reg_num = parse_register_number(op);
        if (reg_num >= 0 && reg_num < NUM_REGISTERS) {
            // Si c'est un numéro de registre valide, renvoyez la valeur dans ce registre
            return registers[reg_num];
        } else {
            // Sinon, renvoyez une erreur
            return -1;
        }
    } else {
        // Si c'est une valeur immédiate, convertissez-la en nombre
        return (int16_t)atoi(op);
    }
}

Instruction* create_instruction(const char* type, const char* arg1, const char* arg2) {
    Instruction* instr = (Instruction*)malloc(sizeof(Instruction));

    if (strcmp(type, "ADD") == 0) {
        instr->type = ADD;
    } else if (strcmp(type, "SUB") == 0) {
        instr->type = SUB;
    } else if (strcmp(type, "MUL") == 0) {
        instr->type = MUL;
    } else if (strcmp(type, "DIV") == 0) {
        instr->type = DIV;
    } else if (strcmp(type, "AND") == 0) {
        instr->type = AND;
    } else if (strcmp(type, "OR") == 0) {
        instr->type = OR;
    } else if (strcmp(type, "XOR") == 0) {
        instr->type = XOR;
    } else if (strcmp(type, "SHL") == 0) {
        instr->type = SHL;
    } else if (strcmp(type, "MOV") == 0) {
        instr->type = MOV;
    } else {
        instr->type = UNKNOWN;
    }

    // Parse the arguments
    instr->rd = (uint8_t)parse_register_number(arg1);
    instr->rn = (uint8_t)parse_register_number(arg1); // Assuming arg1 is also a register for rn
    instr->operand = parse_operand(arg2);

    return instr;
}
// La fonction pour traiter l'instruction SUB
void process_ADD(Instruction* instr) {

    if (instr->rd == 0) {
        printf("R0 ne peut pas être modifié\n");
        return;
    }

    uint16_t value_rn = registers[instr->rn];
    uint16_t value_operand = (instr->operand < NUM_REGISTERS) ? registers[instr->operand] : instr->operand;

    uint16_t result = value_rn + value_operand;
    registers[instr->rd] = result;
    printf("ADD: R%d = R%d + %d -> Result: %d\n", instr->rd, instr->rn, value_operand, result);
}

// La fonction pour traiter l'instruction SUB
void process_SUB(Instruction* instr) {
    if (instr->rd == 0) {
        printf("R0 ne peut pas être modifié\n");
        return;
    }

    uint16_t value_rn = registers[instr->rn];
    uint16_t value_operand = (instr->operand < NUM_REGISTERS) ? registers[instr->operand] : instr->operand;

    uint16_t result = value_rn - value_operand;
    registers[instr->rd] = result;
    printf("SUB: R%d = R%d - %d -> Result: %d\n", instr->rd, instr->rn, value_operand, result);
}

// La fonction pour traiter l'instruction MUL
void process_MUL(Instruction* instr) {
    if (instr->rd == 0) {
        printf("R0 ne peut pas être modifié\n");
        return;
    }

    // Notez que pour MUL, seuls les 8 bits de poids faible de rn et S sont utilisés
    uint8_t value_rn = registers[instr->rn] & 0xFF; // 8 bits de poids faible de rn
    uint8_t value_operand = (instr->operand < NUM_REGISTERS) ? (registers[instr->operand] & 0xFF) : (instr->operand & 0xFF);

    uint16_t result = (uint16_t)value_rn * (uint16_t)value_operand;
    registers[instr->rd] = result;
    printf("MUL: R%d = R%d * %d -> Result: %d\n", instr->rd, instr->rn, value_operand, result);
}

// La fonction pour traiter l'instruction DIV
void process_DIV(Instruction* instr) {
    if (instr->rd == 0) {
        printf("R0 ne peut pas être modifié\n");
        return;
    }

    uint16_t value_rn = registers[instr->rn];
    uint16_t value_operand = (instr->operand < NUM_REGISTERS) ? registers[instr->operand] : instr->operand;

    if (value_operand == 0) {
        printf("Erreur de division par zéro\n");
        return;
    }

    uint16_t result = value_rn / value_operand;
    registers[instr->rd] = result;
    printf("DIV: R%d = R%d / %d -> Result: %d\n", instr->rd, instr->rn, value_operand, result);
}

// La fonction pour traiter l'instruction AND
void process_AND(Instruction* instr) {
    if (instr->rd == 0) {
        printf("R0 ne peut pas être modifié\n");
        return;
    }

    uint16_t value_rn = registers[instr->rn];
    uint16_t value_operand = (instr->operand < NUM_REGISTERS) ? registers[instr->operand] : instr->operand;

    uint16_t result = value_rn & value_operand;
    registers[instr->rd] = result;
    printf("AND: R%d = R%d & %d -> Result: %d\n", instr->rd, instr->rn, value_operand, result);
}

void process_instruction(Instruction* instr) {
    switch (instr->type) {
        case ADD:
            process_ADD(instr);
            break;
        case SUB:
            process_SUB(instr);
            break;
        case MUL:
            process_MUL(instr);
            break;
        case DIV:
            process_DIV(instr);
            break;
        case AND:
            process_AND(instr);
            break;
        default:
            printf("Unknown instruction type\n");
            break;
    }
}
void number_to_binary(uint16_t number, char* binary_str, int length) {
    binary_str[length] = '\0'; // Null-terminate the string
    for (int i = length - 1; i >= 0; --i) {
        binary_str[i] = (number & 1) + '0';
        number >>= 1;
    }
}

// Function to encode an Instruction into a 32-bit binary string
void encode_instruction_to_binary(Instruction* instr, char* binary_instruction) {
    // Define opcodes for each instruction type in binary form
    const char* opcodes[] = {
            "00001", // ADD
            "00010", // SUB
            "00011", // MUL
            "00100", // DIV
            // ... (other opcodes for each instruction type)
    };

    char binary_opcode[6], binary_rd[6], binary_rn[6], binary_operand[17] = {0};
    // Get the binary opcode for the instruction type
    strcpy(binary_opcode, opcodes[instr->type]);

    // Convert each part of the instruction to binary
    number_to_binary(instr->rd, binary_rd, 5);
    number_to_binary(instr->rn, binary_rn, 5);

    // Handle the operand part of the instruction
    if (instr->operand < NUM_REGISTERS) { // Check if the operand is a register
        number_to_binary(instr->operand, binary_operand, 5);
        strcat(binary_operand, "0000000000000000"); // Pad the rest with zeros for register
    } else { // Operand is an immediate value
        number_to_binary(instr->operand, binary_operand, 16); // Convert the immediate value to 16-bit binary
    }

    // Construct the full 32-bit binary instruction as a string
    strcpy(binary_instruction, binary_opcode); // Start with the opcode
    strcat(binary_instruction, binary_rd);     // Add the destination register
    strcat(binary_instruction, binary_rn);     // Add the first source register
    strcat(binary_instruction, instr->operand < NUM_REGISTERS ? "0" : "1"); // Immediate flag
    strcat(binary_instruction, binary_operand); // Add the operand (either register or immediate)
}
/*int main() {
    // Initialization of registers for testing
    // Assuming R0 should always be 0, start from 1
    for (int i = 1; i < NUM_REGISTERS; i++) {
        registers[i] = i * 10;  // Arbitrary values for registers
    }

    // Create and process an ADD instruction
    Instruction* add_instr = create_instruction("ADD", "R1", "45"); // ADD R1, 45
    process_instruction(add_instr);

    // Print the state of the registers after ADD
    printf("After ADD: R1 = %d\n", registers[1]);

    // Assuming you have implemented MOV similarly
    // Instruction* mov_instr = create_instruction("MOV", "R2", "R1"); // MOV R2, R1
    // process_instruction(mov_instr);

    // Print the state of the registers after MOV
    // printf("After MOV: R2 = %d\n", registers[2]);

    // Free the instruction
    free(add_instr);

    return 0;
}*/

int main() {
    // Initialization of registers for testing
    // Assuming R0 should always be 0, start from 1
    for (int i = 1; i < NUM_REGISTERS; i++) {
        registers[i] = i * 10;  // Arbitrary values for registers
    }

    // Create an ADD instruction
    Instruction* add_instr = create_instruction("ADD", "R1", "R2"); // ADD R1, R2

    // Ensure that the instruction was created successfully
    if (add_instr != NULL) {
        // Process the instruction (assuming you have a function to do so)
        process_instruction(add_instr);

        // Print the state of the registers after ADD
        printf("After ADD: R1 = %d\n", registers[1]);

        // Encode the instruction into binary
        char binary_instruction[33]; // 32 bits for the instruction and 1 for the null terminator
        encode_instruction_to_binary(add_instr, binary_instruction);
        printf("Binary Instruction: %s\n", binary_instruction);

        // Free the instruction
        free(add_instr);
    } else {
        // Handle the error if the instruction wasn't created
        fprintf(stderr, "Error creating ADD instruction.\n");
    }

    return 0;
}




