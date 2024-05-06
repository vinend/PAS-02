#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "struct.h"


void createUser() {
    int trigger = 0;
    char buffer[100];
    user playerLogin;

    user *playerLogin = (user*)malloc(sizeof(user));

    printf("Create GamerTag: "); scanf("%s", &playerLogin.gamerTag);
    flushInput();

    do {
        printf("Suggest Password? Yes / No"); scanf("%s", &buffer); 
    } while(trigger == 0); 

    printf("Create Password: "); scanf("%s", &playerLogin.password);

}

void loginUser() {

}

void loginPageMenu() {

    int pilihan, trigger = 0;

    do {
    printf(" __      _____________.____   _________  ________      _____  ___________            __________________     \n");
    printf("/  \    /  \_   _____/|    |  \_   ___ \ \_____  \    /     \ \_   _____/           \__    ___/\_____ \     \n");
    printf("\   \/\/   /|    __)_ |    |  /    \  \/  /   |   \  /  \ /  \ |    __)_              |    |    /   |  \    \n");
    printf(" \        / |        \|    |__\     \____/    |    \/    Y    \|        \             |    |   /    |   \  \ n");
    printf("  \__/\  / /_______  /|_______ \______  /\_______  /\____|__  /_______  /             |____|   \_______  /  \n");
    printf("       \/          \/         \/      \/         \/         \/        \/                               \/   \n");
    printf("\n\n");
    printf("  ________    _____      _____  ___________            .____    ._______________________    _____ _______________.___. \n");
    printf(" /  _____/   /  _  \    /     \ \_   _____/            |    |   |   \______   \______   \  /  _  \\______   \__  |   | \n");
    printf("/   \  ___  /  /_\  \  /  \ /  \ |    __)_             |    |   |   ||    |  _/|       _/ /  /_\  \|       _//   |   | \n");
    printf("\    \_\  \/    |    \/    Y    \|        \            |    |___|   ||    |   \|    |   \/    |    \    |   \\____   | \n");
    printf(" \______  /\____|__  /\____|__  /_______  /            |_______ \___||______  /|____|_  /\____|__  /____|_  // ______| \n");
    printf("        \/         \/         \/        \/                     \/           \/        \/         \/       \/ \/        \n");

    printf("\n\nPress Any Button to Continue\n");
    getch();
    system("cls");

    printf(" +-------------------------------------------------+\n");
    printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
    printf(" +-------------------------------------------------+\n");
    printf(" | No. |              OPSI                         |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  1  | Login User                                |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  2  | Tambahkan User                            |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  3  | Exit Program                              |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
        case 1 :

        case 2 :

        case 3 :

        default :
    }
    } while(trigger == 0);

}