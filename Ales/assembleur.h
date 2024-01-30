#ifndef VIRTUALMACHINE2_ASSEMBLEUR_H
#define VIRTUALMACHINE2_ASSEMBLEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Structures pour stocker les étiquettes et les adresses
typedef struct {
    char label[50];
    unsigned int address;
} Label;

unsigned int get_opcode(const char* instruction);
unsigned int get_register(const char* reg);
unsigned int get_immediate(const char* imm, int* error);
int is_immediate_operand(const char* operand);
unsigned int assemble_line(const char *instruction, const char *operands);
void first_pass(FILE* input_file);
void second_pass(FILE* input_file, FILE* output_file);
unsigned int resolve_label(const char* label, const char* instruction);

#endif //VIRTUALMACHINE2_ASSEMBLEUR_H
