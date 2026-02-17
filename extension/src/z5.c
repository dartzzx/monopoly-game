#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "monopoly.h" // Nemenit !!!
// Nevkladat monopoly.c !!!

int main(int argc, char *argv[]) {
    int option, tflag = 0, bflag = 0, cislo = 0, cislo2 = 0, count=0, count2=0;
    const char *optstring = ":t:b:";
    char pole[101], color[101], pozemok[101], *pes, cena;
    FILE *properties_file = NULL;
    FILE *gameboard_file = NULL;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 't':
                properties_file = fopen(optarg, "r");
                if (properties_file == NULL) {
                    printf("E1");
                    return 0;
                }
                else {
                    tflag = 1;
                    break;
                }
            case 'b':
                gameboard_file = fopen(optarg, "r");
                if (gameboard_file == NULL) {
                    printf("E1");
                    return 0;
                }
                else {
                    bflag = 1;
                    break;
                }
        }
    }
    if (tflag && !bflag) {
        while (fgets(pole, sizeof(pole), properties_file) != NULL) {
            if (pole[0] == '[') {
                for (int i = 0; i < 101; i++) {
                    color[i] = 0;
                }
                color[0] = ' ';
                for (int i = 1; pole[i] != ']'; i++) {
                    color[i] = pole[i];
                }
                continue;
            }
            if (pole[0] =='"') {
                cena = pole[strlen(pole)-2];
                pes = strtok(pole, "\":123456789");
                for (int i = 0; i < 101; i++) {
                    pozemok[i] = pes[i];
                }
            }
            printf("%s %c %s\n", pozemok, cena, color);

            for (int i = 0; i < 101; i++) {
                pole[i] = 0;
            }
        }
    }
    else if (!tflag && bflag) {
        while (fgets(pole, sizeof(pole), gameboard_file) != NULL) {
            if (pole[0] == 'P') {
                count++;
            }
        }
        if (count > NUM_PROPERTIES) {
            printf("E2");
            return 0;
        }
        rewind(gameboard_file);
        while (fgets(pole, sizeof(pole), gameboard_file) != NULL) {
            if (pole[0] == 'P') {
                printf("%d. %s %d %s\n", cislo2, properties[cislo].name, properties[cislo].price, property_colors[properties[cislo].color]);
                cislo++;
            }
            else {
                if (cislo2 != 0) {
                    printf("%d. %s", cislo2, pole);
                }
            }
            cislo2++;
            for (int i = 0; i < 101; i++) {
                pole[i] = 0;
                color[i] = 0;
            }
        }
    }
    else if (tflag && bflag) {
        while (fgets(pole, sizeof(pole), gameboard_file) != NULL) {
            if (pole[0] == 'P') {
                count++;
            }
        }
        while (fgets(pole, sizeof(pole), properties_file) != NULL) {
            if (pole[0] == '"') {
                count2++;
            }
        }
        if (count > count2) {
            printf("E2");
            return 0;
        }
        rewind(gameboard_file);
        rewind(properties_file);
        while (fgets(pole, sizeof(pole), gameboard_file) != NULL) {
            if (pole[0] == 'P') {
                if (fgets(pole, sizeof(pole), properties_file) != NULL) {
                    if (pole[0] == '[') {
                        for (int i = 0; i < 101; i++) {
                            color[i] = 0;
                        }
                        color[0] = ' ';
                        for (int i = 1; pole[i] != ']'; i++) {
                            color[i] = pole[i];
                        }
                        fgets(pole, sizeof(pole), properties_file);
                    }
                    if (pole[0] =='"') {
                        cena = pole[strlen(pole)-2];
                        pes = strtok(pole, "\":123456789");
                        for (int i = 0; i < 101; i++) {
                            pozemok[i] = pes[i];
                        }
                    }
                    printf("%d. %s %c %s\n", cislo2, pozemok, cena, color);
                }
            }
            else {
                if (cislo2 != 0) {
                    printf("%d. %s", cislo2, pole);
                }
            }
            cislo2++;
            for (int i = 0; i < 101; i++) {
                pole[i] = 0;
            }
        }
    }
    return 0;
}
