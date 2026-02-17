#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "monopoly.h" // NEMENIT !!!
#include <string.h>

int calculate_property_value(PLAYER *player) {
    int total_value = 0;
    int i = 0;
    while(i < player->num_properties) {
        total_value += player->owned_properties[i]->price;
        i++;
    }
    return total_value;
}
void zaciatok(int i, int S, int C, int JP, char *IJ) {
    printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n", i, S, C, JP, IJ);
}
int has_monopoly(PLAYER *player, COLOR color) {
    int count = 0, i = 0;
    while(i < player->num_properties) {
        if (player->owned_properties[i]->color == color) {
            count++;
        }
        i++;
    }
    return count > 1;
}


int main(int argc, char *argv[]) {
    char *players_temp = "2";
    int option, sflag = 0, pflag = 0, gflag = 0;
    const char *optstring = ":n:spg";

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'n':
                if (strcmp(optarg, "3") == 0) {
                    players_temp = "3";
                }
                else if (strcmp(optarg, "4") == 0) {
                    players_temp = "4";
                }
                break;
            case 's':
                sflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
            case 'g':
                gflag = 1;
                break;
            default:
                break;
        }
    }

    int num_players = atoi(players_temp);

    PLAYER players[num_players];
    for (int i = 0; i < num_players; i++) {
        players[i].number = i + 1;
        players[i].space_number = 1;
        players[i].num_jail_pass = 0;
        players[i].is_in_jail = 0;
        players[i].num_properties = 0;
        for (int j = 0; j < NUM_PROPERTIES; j++) {
            players[i].owned_properties[j] = NULL;
        }
        if (num_players == 2) {
            players[i].cash = 20;
        }
        else if (num_players == 3) {
            players[i].cash = 18;
        }
        else if (num_players == 4) {
            players[i].cash = 16;
        }
    }
    printf("Players:\n");
    for (int i = 0; i<num_players; i++) {
        if (!(players[i].is_in_jail)) {
            char *IJ = "no";
            zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
        }
        else {
            char *IJ = "yes";
            zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
        }
    }
    printf("Game board:\n");
    for (int l = 0; l < NUM_SPACES; l++) {
        if (game_board[l].type == Property){
            printf("%d. %-10s %d  %s\n", l+1, game_board[l].property->name,game_board[l].property->price,property_colors[game_board[l].property->color]);
        }
        else
        {
            printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
        }
    }
    printf("WINNER: -\n");

    int turn = 1;

    while(1) {
        for (int k = 0; k < num_players; k++) {
            int pohyb;
            scanf("%d", &pohyb);
            printf("R: %d\n", pohyb);
            printf("Turn: %d\n", turn);
            printf("Player on turn: P%d\n", k+1);
            printf("\n");
            turn++;
            players[k].space_number += pohyb;
            switch (game_board[players[k].space_number - 1].type) {
                case Jail_pass:
                    players[k].num_jail_pass++;
                    break;
                case Go_to_jail:
                    if (players[k].num_jail_pass > 0) {
                        players[k].num_jail_pass--;
                    }
                    else {
                        players[k].is_in_jail = 1;
                        for (int i = 0; i < NUM_SPACES; i++) {
                            if (game_board[i].type == In_jail) {
                                players[k].space_number = i + 1;
                                break;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            if (players[k].is_in_jail) {
                if (players[k].cash > 0) {
                    players[k].is_in_jail = 0;
                    players[k].cash -= 1;
                }
                else {
                    PLAYER *winner = &players[0];
                    int max_cash = players[0].cash;
                    int max_property_value = calculate_property_value(&players[0]);
                    for (int i = 1; i < num_players; i++) {
                        int property_value = calculate_property_value(&players[i]);
                        if (players[i].cash > max_cash ||
                            (players[i].cash == max_cash && property_value > max_property_value)) {
                            winner = &players[i];
                            max_cash = players[i].cash;
                            max_property_value = property_value;
                        }
                    }
                    int draw = 0;
                    for (int i = 0; i < num_players; i++) {
                        if (i != winner->number - 1) {
                            int property_value = calculate_property_value(&players[i]);
                            if (players[i].cash == max_cash && property_value == max_property_value) {
                                draw = 1;
                                break;
                            }
                        }
                    }
                    printf("Players:\n");
                    for (int i = 0; i < num_players; i++) {
                        if (!(players[i].is_in_jail)) {
                            char *IJ = "no";
                            zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                            for (int j = 0; j < players[i].num_properties; j++) {
                                printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                for (int m = 0; m < NUM_SPACES; m++) {
                                    if (game_board[m].property == players[i].owned_properties[j]) {
                                        printf("S%d\n", m + 1);
                                    }
                                }
                            }
                        }
                        else {
                            char *IJ = "yes";
                            zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                            for (int j = 0; j < players[i].num_properties; j++) {
                                printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                for (int m = 0; m < NUM_SPACES; m++) {
                                    if (game_board[m].property == players[i].owned_properties[j]) {
                                        printf("S%d\n", m + 1);
                                    }
                                }
                            }
                        }
                    }
                    printf("Game board:\n");
                    for (int l = 0; l < NUM_SPACES; l++) {
                        if (game_board[l].type == Property){
                            int owner_number = 0;
                            PLAYER *owner = NULL;
                            for (int i = 0; i < num_players; i++) {
                                for (int j = 0; j < players[i].num_properties; j++) {
                                    if (players[i].owned_properties[j] == game_board[l].property) {
                                        owner_number = players[i].number;
                                        owner = &players[i];
                                        break;
                                    }
                                }
                                if (owner_number != 0) {
                                    break;
                                }
                            }
                            printf("%d. %-10s %d  %s", l+1, game_board[l].property->name, game_board[l].property->price, property_colors[game_board[l].property->color]);
                            if (owner_number != 0) {
                                char *monopoly_status = "no";
                                if (owner != NULL && has_monopoly(owner, game_board[l].property->color)) {
                                    monopoly_status = "yes";
                                }
                                printf(" P%d %s", owner_number, monopoly_status);
                            }
                            printf("\n");
                        }
                        else
                        {
                            printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
                        }
                    }
                    if (draw == 1) {
                        printf("WINNER: ?\n");
                    }
                    else {
                        printf("WINNER: P%d\n", winner->number);
                    }
                    return 0;
                }
            }
            if (players[k].space_number > 24) {
                players[k].cash += 2;
                players[k].space_number -= 24;
            }
            if (game_board[players[k].space_number - 1].property != NULL) {
                PROPERTY *landed_property = game_board[players[k].space_number - 1].property;
                int owned = 0;
                for (int j = 0; j < players[k].num_properties; j++) {
                    if (players[k].owned_properties[j] == landed_property) {
                        owned = 1;
                        break;
                    }
                }
                if (!owned) {
                    PLAYER *owner = NULL;
                    for (int i = 0; i < num_players; i++) {
                        if (i != k) {
                            for (int j = 0; j < players[i].num_properties; j++) {
                                if (players[i].owned_properties[j] == landed_property) {
                                    owner = &players[i];
                                    break;
                                }
                            }
                        }
                        if (owner != NULL) {
                            break;
                        }
                    }
                    if (owner != NULL) {
                        int rent = landed_property->price;
                        if (has_monopoly(owner, landed_property->color)) {
                            rent *= 2;
                        }
                        if (players[k].cash >= rent) {
                            players[k].cash -= rent;
                            owner->cash += rent;
                        }
                        else {
                            PLAYER *winner = &players[0];
                            int max_cash = players[0].cash;
                            int max_property_value = calculate_property_value(&players[0]);
                            for (int i = 1; i < num_players; i++) {
                                int property_value = calculate_property_value(&players[i]);
                                if (players[i].cash > max_cash ||
                                    (players[i].cash == max_cash && property_value > max_property_value)) {
                                    winner = &players[i];
                                    max_cash = players[i].cash;
                                    max_property_value = property_value;
                                }
                            }
                            int draw = 0;
                            for (int i = 0; i < num_players; i++) {
                                if (i != winner->number - 1) {
                                    int property_value = calculate_property_value(&players[i]);
                                    if (players[i].cash == max_cash && property_value == max_property_value) {
                                        draw = 1;
                                        break;
                                    }
                                }
                            }
                            printf("Players:\n");
                            for (int i = 0; i < num_players; i++) {
                                if (!(players[i].is_in_jail)) {
                                    char *IJ = "no";
                                    zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                                    for (int j = 0; j < players[i].num_properties; j++) {
                                        printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                        for (int m = 0; m < NUM_SPACES; m++) {
                                            if (game_board[m].property == players[i].owned_properties[j]) {
                                                printf("S%d\n", m + 1);
                                            }
                                        }
                                    }
                                }
                                else {
                                    char *IJ = "yes";
                                    zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                                    for (int j = 0; j < players[i].num_properties; j++) {
                                        printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                        for (int m = 0; m < NUM_SPACES; m++) {
                                            if (game_board[m].property == players[i].owned_properties[j]) {
                                                printf("S%d\n", m + 1);
                                            }
                                        }
                                    }
                                }
                            }
                            printf("Game board:\n");
                            for (int l = 0; l < NUM_SPACES; l++) {
                                if (game_board[l].type == Property){
                                    int owner_number = 0;
                                    PLAYER *owner = NULL;
                                    for (int i = 0; i < num_players; i++) {
                                        for (int j = 0; j < players[i].num_properties; j++) {
                                            if (players[i].owned_properties[j] == game_board[l].property) {
                                                owner_number = players[i].number;
                                                owner = &players[i];
                                                break;
                                            }
                                        }
                                        if (owner_number != 0) {
                                            break;
                                        }
                                    }
                                    printf("%d. %-10s %d  %s", l+1, game_board[l].property->name, game_board[l].property->price, property_colors[game_board[l].property->color]);
                                    if (owner_number != 0) {
                                        char *monopoly_status = "no";
                                        if (owner != NULL && has_monopoly(owner, game_board[l].property->color)) {
                                            monopoly_status = "yes";
                                        }
                                        printf(" P%d %s", owner_number, monopoly_status);
                                    }
                                    printf("\n");
                                }
                                else
                                {
                                    printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
                                }
                            }
                            if (draw == 1) {
                                printf("WINNER: ?\n");
                            }
                            else {
                                printf("WINNER: P%d\n", winner->number);
                            }
                            return 0;
                        }
                    }
                    else {
                        if (players[k].cash >= landed_property->price) {
                            players[k].cash -= landed_property->price;
                            players[k].owned_properties[players[k].num_properties] = landed_property;
                            players[k].num_properties++;
                        } else {
                            PLAYER *winner = &players[0];
                            int max_cash = players[0].cash;
                            int max_property_value = calculate_property_value(&players[0]);
                            for (int i = 1; i < num_players; i++) {
                                int property_value = calculate_property_value(&players[i]);
                                if (players[i].cash > max_cash ||
                                    (players[i].cash == max_cash && property_value > max_property_value)) {
                                    winner = &players[i];
                                    max_cash = players[i].cash;
                                    max_property_value = property_value;
                                }
                            }
                            int draw = 0;
                            for (int i = 0; i < num_players; i++) {
                                if (i != winner->number - 1) {
                                    int property_value = calculate_property_value(&players[i]);
                                    if (players[i].cash == max_cash && property_value == max_property_value) {
                                        draw = 1;
                                        break;
                                    }
                                }
                            }
                            printf("Players:\n");
                            for (int i = 0; i < num_players; i++) {
                                if (!(players[i].is_in_jail)) {
                                    char *IJ = "no";
                                    zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                                    for (int j = 0; j < players[i].num_properties; j++) {
                                        printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                        for (int m = 0; m < NUM_SPACES; m++) {
                                            if (game_board[m].property == players[i].owned_properties[j]) {
                                                printf("S%d\n", m + 1);
                                            }
                                        }
                                    }
                                }
                                else {
                                    char *IJ = "yes";
                                    zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                                    for (int j = 0; j < players[i].num_properties; j++) {
                                        printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                                        for (int m = 0; m < NUM_SPACES; m++) {
                                            if (game_board[m].property == players[i].owned_properties[j]) {
                                                printf("S%d\n", m + 1);
                                            }
                                        }
                                    }
                                }
                            }
                            printf("Game board:\n");
                            for (int l = 0; l < NUM_SPACES; l++) {
                                if (game_board[l].type == Property){
                                    int owner_number = 0;
                                    PLAYER *owner = NULL;
                                    for (int i = 0; i < num_players; i++) {
                                        for (int j = 0; j < players[i].num_properties; j++) {
                                            if (players[i].owned_properties[j] == game_board[l].property) {
                                                owner_number = players[i].number;
                                                owner = &players[i];
                                                break;
                                            }
                                        }
                                        if (owner_number != 0) {
                                            break;
                                        }
                                    }
                                    printf("%d. %-10s %d  %s", l+1, game_board[l].property->name, game_board[l].property->price, property_colors[game_board[l].property->color]);
                                    if (owner_number != 0) {
                                        char *monopoly_status = "no";
                                        if (owner != NULL && has_monopoly(owner, game_board[l].property->color)) {
                                            monopoly_status = "yes";
                                        }
                                        printf(" P%d %s", owner_number, monopoly_status);
                                    }
                                    printf("\n");
                                }
                                else
                                {
                                    printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
                                }
                            }
                            if (draw == 1) {
                                printf("WINNER: ?\n");
                            }
                            else {
                                printf("WINNER: P%d\n", winner->number);
                            }
                            return 0;
                        }
                    }
                }
            }
            if (sflag) {
                printf("Game board:\n");
                for (int l = 0; l < NUM_SPACES; l++) {
                    if (game_board[l].type == Property){
                        int owner_number = 0;
                        PLAYER *owner = NULL;
                        for (int i = 0; i < num_players; i++) {
                            for (int j = 0; j < players[i].num_properties; j++) {
                                if (players[i].owned_properties[j] == game_board[l].property) {
                                    owner_number = players[i].number;
                                    owner = &players[i];
                                    break;
                                }
                            }
                            if (owner_number != 0) {
                                break;
                            }
                        }
                        printf("%d. %-10s %d  %s", l+1, game_board[l].property->name, game_board[l].property->price, property_colors[game_board[l].property->color]);
                        if (owner_number != 0) {
                            char *monopoly_status = "no";
                            if (owner != NULL && has_monopoly(owner, game_board[l].property->color)) {
                                monopoly_status = "yes";
                            }
                            printf(" P%d %s", owner_number, monopoly_status);
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
                    }
                }
            }
            else if(pflag){
                printf("Players:\n");
                for (int i = 0; i < num_players; i++) {
                    if (!(players[i].is_in_jail)) {
                        char *IJ = "no";
                        zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                        for (int j = 0; j < players[i].num_properties; j++) {
                            printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                            for (int m = 0; m < NUM_SPACES; m++) {
                                if (game_board[m].property == players[i].owned_properties[j]) {
                                    printf("S%d\n", m + 1);
                                }
                            }
                        }
                    }
                    else {
                        char *IJ = "yes";
                        zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                        for (int j = 0; j < players[i].num_properties; j++) {
                            printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                            for (int m = 0; m < NUM_SPACES; m++) {
                                if (game_board[m].property == players[i].owned_properties[j]) {
                                    printf("S%d\n", m + 1);
                                }
                            }
                        }
                    }
                }
            }
            else if(gflag) {
                printf("Players:\n");
                for (int i = 0; i < num_players; i++) {
                    if (!(players[i].is_in_jail)) {
                        char *IJ = "no";
                        zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                        for (int j = 0; j < players[i].num_properties; j++) {
                            printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                            for (int m = 0; m < NUM_SPACES; m++) {
                                if (game_board[m].property == players[i].owned_properties[j]) {
                                    printf("S%d\n", m + 1);
                                }
                            }
                        }
                    }
                    else {
                        char *IJ = "yes";
                        zaciatok(i + 1, players[i].space_number, players[i].cash, players[i].num_jail_pass, IJ);
                        for (int j = 0; j < players[i].num_properties; j++) {
                            printf("      %s %d %s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price, property_colors[players[i].owned_properties[j]->color]);
                            for (int m = 0; m < NUM_SPACES; m++) {
                                if (game_board[m].property == players[i].owned_properties[j]) {
                                    printf("S%d\n", m + 1);
                                }
                            }
                        }
                    }
                }
                printf("Game board:\n");
                for (int l = 0; l < NUM_SPACES; l++) {
                    if (game_board[l].type == Property){
                        int owner_number = 0;
                        PLAYER *owner = NULL;
                        for (int i = 0; i < num_players; i++) {
                            for (int j = 0; j < players[i].num_properties; j++) {
                                if (players[i].owned_properties[j] == game_board[l].property) {
                                    owner_number = players[i].number;
                                    owner = &players[i];
                                    break;
                                }
                            }
                            if (owner_number != 0) {
                                break;
                            }
                        }
                        printf("%d. %-10s %d  %s", l+1, game_board[l].property->name, game_board[l].property->price, property_colors[game_board[l].property->color]);
                        if (owner_number != 0) {
                            char *is_monopoly = "no";
                            if (owner != NULL && has_monopoly(owner, game_board[l].property->color)) {
                                is_monopoly = "yes";
                            }
                            printf(" P%d %s", owner_number, is_monopoly);
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("%d. %-10s\n", l + 1, space_types[game_board[l].type]);
                    }
                }
                printf("WINNER: -\n");
            }
        }
    }
}