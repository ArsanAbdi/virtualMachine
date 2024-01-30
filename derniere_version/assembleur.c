#include "assembleur.h"
#include <ctype.h>

#define MAX_LABELS 256
#define MAX_LINE_LENGTH 256

Label labels[MAX_LABELS];
unsigned int label_count = 0;

void effacerTableau(char *tableau, size_t taille) {
    for (size_t i = 0; i < taille; i++) {
        tableau[i] = '\0';
    }
}

void copierTableau(char *destination, const char *source, size_t tailleDest) {
    size_t i;
    for (i = 0; i < tailleDest - 1 && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    destination[i] = '\0'; // Assure que le tableau de destination se termine par '\0'
}

// Cette fonction convertit une instruction assembleur en son opcode numérique.
unsigned int get_opcode(const char* instruction) {
    // Mapping des instructions aux opcodes
    if (strcmp(instruction, "add") == 0) return 0;
    if (strcmp(instruction, "sub") == 0) return 1;
    if (strcmp(instruction, "mul") == 0) return 2;
    if (strcmp(instruction, "div") == 0) return 3;
    if (strcmp(instruction, "and") == 0) return 4;
    if (strcmp(instruction, "or") == 0) return 5;
    if (strcmp(instruction, "xor") == 0) return 6;
    if (strcmp(instruction, "shl") == 0) return 7;
    if (strcmp(instruction, "ldb") == 0) return 10;
    if (strcmp(instruction, "ldw") == 0) return 11;
    if (strcmp(instruction, "stb") == 0) return 12;
    if (strcmp(instruction, "stw") == 0) return 13;
    if (strcmp(instruction, "jmp") == 0) return 20;
    if (strcmp(instruction, "jzs") == 0) return 21;
    if (strcmp(instruction, "jzc") == 0) return 22;
    if (strcmp(instruction, "jcs") == 0) return 23;
    if (strcmp(instruction, "jcc") == 0) return 24;
    if (strcmp(instruction, "jns") == 0) return 25;
    if (strcmp(instruction, "jnc") == 0) return 26;
    if (strcmp(instruction, "in") == 0) return 27;
    if (strcmp(instruction, "out") == 0) return 28;
    if (strcmp(instruction, "rnd") == 0) return 29;
    if (strcmp(instruction, "hlt") == 0) return 31;

    // Si l'instruction n'est pas reconnue, renvoyer un opcode spécial ou signaler une erreur
    return 0xFFFFFFFF; // Valeur spéciale indiquant une instruction inconnue ou invalide
}

unsigned int get_register(const char* reg) {
    // Cette fonction renvoie le numéro du registre à partir de son nom.
    // Exemple : "r0" -> 0, "r1" -> 1, ...
    if (reg[0] == 'r') {
        return atoi(&reg[1]);
    }
    return 0; // Si le registre n'est pas reconnu
}

unsigned int get_immediate(const char* imm, int* error) {
    char* end;
    long int value = strtol(imm, &end, 0); // 0 permet de déduire la base à partir du préfixe
    if (*end != '\0') { // Si end ne pointe pas vers le caractère null, la conversion a échoué.
        *error = 1;
        return 0;
    }
    *error = 0;
    return (unsigned int)value;
}

int is_immediate_operand(const char* operand) {
    return operand[0] == '#';
}

// Convertit une ligne d'assembleur en code machine.
unsigned int assemble_line(const char* instruction, const char* operands) {
    unsigned int opcode = get_opcode(instruction);
    if (opcode == 0xFFFFFFFF) {
        fprintf(stderr, "Instruction inconnue: %s\n", instruction);
        exit(EXIT_FAILURE);
    }
    unsigned int rd = 0, rn = 0, imm = 0, src2 = 0;
    int imm_flag = 0;
    int error_flag = 0;

    // Gérer les instructions sans opérandes.
    if (operands == NULL || operands[0] == '\0') {
        if (strcmp(instruction, "hlt") == 0) {
            return opcode << 27; // hlt a un opcode mais pas d'opérandes.
        }
    }

    char* tokens[3]; // Pour contenir les opérandes
    int token_count = 0;

    // Copie les opérandes pour ne pas modifier la chaîne originale
    char operands_copy[256];
    strncpy(operands_copy, operands, 255);
    operands_copy[255] = '\0';

    char* token = strtok(operands_copy, ", ");
    while(token != NULL && token_count < 3) {
        tokens[token_count++] = token;
        token = strtok(NULL, ", ");
    }

    opcode = get_opcode(instruction);
    rd = get_register(tokens[0]);
    rn = get_register(tokens[1]);

    if (is_immediate_operand(tokens[2])) {
        imm_flag = 1;
        src2 = get_immediate(tokens[2] + 1, &error_flag); // Skip the '#' character
    } else {
        src2 = get_register(tokens[2]);
    }

    if (error_flag) {
        fprintf(stderr, "Erreur de syntaxe dans les opérandes\n");
        return 0;
    }

    // Encodage de l'instruction
    unsigned int instruction_code = (opcode << 27) | (rd << 22) | (rn << 17) | (imm_flag << 16) | src2;

    return instruction_code;
}

void first_pass(FILE* input_file) {
    char line[MAX_LINE_LENGTH];
    unsigned int address_counter = 0;

    printf("First pass:\n");
    while (fgets(line, sizeof(line), input_file)) {
        // Remove newline character
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        printf("Line read: '%s'\n", line);

        // Skip empty lines and comments
        if (strlen(line) == 0 || line[0] == ';') continue;

        // Check for label
        char* label_end = strchr(line, ':');
        if (label_end) {
            *label_end = '\0'; // Terminate the label string
            strncpy(labels[label_count].label, line, sizeof(labels[label_count].label) - 1);
            labels[label_count].address = address_counter;
            printf("Label found: '%s' at address %u\n", labels[label_count].label, address_counter);
            label_count++;
        }

        // Increment address for each instruction line
        address_counter++;
    }

    // Reset the file position indicator to the beginning of the file
    rewind(input_file);
}

void second_pass(FILE* input_file, FILE* output_file) {
    char line[MAX_LINE_LENGTH];
    unsigned int line_number = 0;
    unsigned int machine_code;

    printf("Second pass:\n");
    while (fgets(line, sizeof(line), input_file)) {
        line_number++;
        // Remove newline character
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        // Ignore empty lines and comments
        if (strlen(line) == 0 || line[0] == ';') {
            continue;
        }

        char* instruction = NULL;
        char* operands = NULL;
        char* next_instr = line;

        // Check for label-only lines and lines with label followed by an instruction
        char* label_end = strchr(line, ':');
        if (label_end) {
            // Check if there's more in the line after the label
            next_instr = label_end + 1;
            while (*next_instr != '\0' && isspace((unsigned char)*next_instr)) {
                next_instr++; // Skip white spaces
            }
            if (*next_instr == '\0') {
                continue; // Skip label-only lines
            }
        }

        // Tokenize the line into instruction and operands
        instruction = strtok(next_instr, " ");
        operands = strtok(NULL, "\n");

        // If we have an instruction to process, let's do it
        if (instruction) {
            unsigned int opcode = get_opcode(instruction);

            if (opcode == 0xFFFFFFFF) {
                fprintf(stderr, "Assembly error on line %u: unknown instruction '%s'\n", line_number, instruction);
                fclose(input_file);
                fclose(output_file);
                exit(EXIT_FAILURE);
            }

            // Handle instructions without operands like 'hlt'
            if (operands == NULL) {
                machine_code = opcode << 27;
            } else {
                // Handle jump instructions with label operands
                if (opcode == 20) { // 'jmp' instruction
                    unsigned int label_address = resolve_label(operands, "jmp");
                    machine_code = (opcode << 27) | (label_address & 0x07FFFFFF);
                } else if (opcode >= 21 && opcode <= 26) { // Other jump instructions
                    unsigned int label_address = resolve_label(operands, "jzs"); // Changez "instruction" par "jzs" si c'est spécifique à 'jzs'
                    machine_code = (opcode << 27) | (label_address & 0x07FFFFFF);
                } else {
                    // Regular instruction, assemble it
                    machine_code = assemble_line(instruction, operands);
                    if (machine_code == 0xFFFFFFFF) {
                        fprintf(stderr, "Assembly error on line %u: cannot assemble line '%s'\n", line_number, line);
                        fclose(input_file);
                        fclose(output_file);
                        exit(EXIT_FAILURE);
                    }
                }
            }

            printf("Machine code for line %u: %08X\n", line_number, machine_code);
            fprintf(output_file, "%08X\n", machine_code);
        } else {
            fprintf(stderr, "Syntax error on line %u: no instruction found\n", line_number);
            fclose(input_file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }
    }
}

unsigned int resolve_label(const char* label, const char* instruction) {
    for (unsigned int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            unsigned int address = labels[i].address * 4;
            // Ajouter un décalage supplémentaire uniquement pour l'instruction 'jmp'.
            if (strcmp(instruction, "jzs") == 0 || strcmp(instruction, "jmp") == 0) {
                address += 0x10000;
            }
            return address;
        }
    }
    fprintf(stderr, "Label '%s' not found.\n", label);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <assembly_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* input_filename = argv[1];
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Effectue la première passe pour collecter les étiquettes et leurs adresses.
    first_pass(input_file);

    FILE* output_file = fopen("hexa.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    // Effectue la deuxième passe pour assembler les instructions et écrire le code machine.
    second_pass(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    printf("Assembly completed successfully.\n");

    return EXIT_SUCCESS;
}