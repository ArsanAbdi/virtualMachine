//
// Created by Arsan Abdi  on 26/01/2024.
//
#include <stdint.h>
#include <string.h>

#define MEMOIRE_TAILLE 65535 // correspond à la taille de la mémoire (64 kilo-octets)
#define REGISTRES_GENERAUX 32 // pour les 32 registres généraux de 16 bits

typedef struct registresEtats {

    uint8_t Z: 1; // uint8_t -> codé sur 8 bits // 1 si nul
    uint8_t C: 1; // :1 -> réduire utilisation de la mémoire // 1 si retenue
    uint8_t N: 1; // car Z, C, N sont tous codé sur 1 bit // 1 si négatif
} RegistresEtats;


typedef struct machineVirtuelle {

    uint8_t memoire[MEMOIRE_TAILLE]; // memoire de 64KO
    uint32_t registres[REGISTRES_GENERAUX]; // 32 registres généraux
    uint32_t PC; // l’adresse de la prochaine instruction à exécuter // compteur de programme
    RegistresEtats etatsDesRegistres; // structure des registres d'état
} Machine;

void miseEnPlaceMachineVirtuelle(Machine *machine) { // prend une machine et la configure

    memset(machine -> memoire, 0, MEMOIRE_TAILLE); // initialise mémoire à 0

    for (int i = 0; i < REGISTRES_GENERAUX; i++) { // pareil pour les registres

        machine -> registres[i] = 0;
    }

    machine -> PC = 0; // initialise PC à 0
    machine -> etatsDesRegistres.C = 0;
    machine -> etatsDesRegistres.N = 0;
    machine -> etatsDesRegistres.Z = 0;
}