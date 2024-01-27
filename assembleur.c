//
// Created by Arsan Abdi  on 27/01/2024.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t traduireInstructionEnCodeMachine(const char *instruction) { // fonction -> instruction d'assemblage en code machine

    uint32_t codeMachine = 0;
    /*instructions arithmétiques et logiques*/
    if (strncmp(instruction, "add ", 4) == 0) {

        codeMachine = 0x00000000; // Supposons que NOP soit 0x00000000 en langage machine
    }
    else if (strncmp(instruction, "sub ", 4) == 0) {


    }
    else if (strncmp(instruction, "mul ", 4) == 0) {


    }
    else if (strncmp(instruction, "div ", 4) == 0) {


    }
    else if (strncmp(instruction, "and ", 4) == 0) {


    }
    else if (strncmp(instruction, "or ", 3) == 0) {


    }
    else if (strncmp(instruction, "xor ", 4) == 0) {


    }
    else if (strncmp(instruction, "shl ", 4) == 0) {


    }
        /*instructions de transfert*/
    else if (strncmp(instruction, "ldb ", 4) == 0) {


    }
    else if (strncmp(instruction, "ldw ", 4) == 0) {


    }
    else if (strncmp(instruction, "stb ", 4) == 0) {


    }
    else if (strncmp(instruction, "stw ", 4) == 0) {


    }
        /*instructions de sauts*/
    else if (strncmp(instruction, "jmp ", 4) == 0) {


    }
    else if (strncmp(instruction, "jzs ", 4) == 0) {


    }
    else if (strncmp(instruction, "jzc ", 4) == 0) {


    }
    else if (strncmp(instruction, "jcs ", 4) == 0) {


    }
    else if (strncmp(instruction, "jcc ", 4) == 0) {


    }
    else if (strncmp(instruction, "jns ", 4) == 0) {


    }
    else if (strncmp(instruction, "jnc ", 4) == 0) {


    }
        /*instructions d’entrée-sortie*/
    else if (strncmp(instruction, "in ", 3) == 0) {


    }
    else if (strncmp(instruction, "out ", 4) == 0) {


    }
        /*instructions diverses*/
    else if (strncmp(instruction, "rnd ", 4) == 0) {


    }
    else if (strncmp(instruction, "hlt ", 4) == 0) {


    }

    return codeMachine;
}

void assembler(const char* fichierSource, const char* fichierDestination) {
    FILE *src = fopen(fichierSource, "r");
    FILE *dest = fopen(fichierDestination, "w");

    if (src == NULL || dest == NULL) {
        perror("Erreur lors de l'ouverture des fichiers");
        exit(1);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), src)) {
        // Supprimer le saut de ligne à la fin
        ligne[strcspn(ligne, "\n")] = 0;

        // Traduire l'instruction
        uint32_t codeMachine = traduireInstructionEnCodeMachine(ligne);

        // Écrire le code machine dans le fichier destination
        fprintf(dest, "%08X\n", codeMachine);
    }

    fclose(src);
    fclose(dest);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <fichier source> <fichier destination>\n", argv[0]);
        return 1;
    }

    assembler(argv[1], argv[2]);
    return 0;
}

