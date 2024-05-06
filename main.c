// Program Platform Library Aplikasi dan Game
// Versi 1.0

// Anggota Kelompok 2 :
// 1. Andi Muhammad Alvin Farhansyah
// 2. Alexander Chirsthian


// Pendeklarasian Library yang Dipakai di dalam Program Ini
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "struct.h"
#include "Data.h"

typedef struct videoGames {
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    int gameKey;
    struct videoGames* next;
} videoGames;

typedef struct {
    char gamerTag[100];
    char password[100];
    videoGames *games;
} user;

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void randomPassGen(char *pass, int length) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        pass[i] = charset[key];
    }
    pass[length] = '\0';
}

void createUser(user **playerLogin) {
    user *newUser = (user*)malloc(sizeof(user));
    if (newUser == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    system("cls");

    printf("Create GamerTag: ");
    scanf("%99s", newUser->gamerTag);
    flushInput();

    system("cls");

    char buffer[10];
    int trigger = 0;
    do {
        printf("Suggest Password? (Yes/No): ");
        scanf("%9s", buffer);
        flushInput();

        if (strcmp(buffer, "Yes") == 0) {
            randomPassGen(newUser->password, 10); 
            printf("Your suggested password is: %s\n", newUser->password);
            getch(); system("cls");
            printf("Create Password: ");
            scanf("%99s", newUser->password);
            trigger = 1;
        } else if (strcmp(buffer, "No") == 0) {
            system("cls");
            printf("Create Password: ");
            scanf("%99s", newUser->password);
            trigger = 1;
        } else {
            printf("Invalid input. Please type 'Yes' or 'No'.\n");
        }
    } while (trigger == 0);

    system("cls");
    
    newUser->games = NULL;
    *playerLogin = newUser; 
}

loginUser(user **player) {

    char bufferUsername[100];
    char bufferPassword[100];

    printf("||         USER LOGIN PAGE         ||\n");
    printf("Enter Username  : "); scanf("%s", &bufferUsername);
    printf("Enter Password  : "); scanf("%s", &bufferPassword); 

}

void loginPageMenu(user **player) {

    int pilihan, trigger = 0;

    printf(" _    _      _                            _____     \n");
    printf("| |  | |    | |                          |_   _|    \n");
    printf("| |  | | ___| | ___ ___  _ __ ___   ___    | |      \n");
    printf("| |/| |/ _ / |/ __/ _ /| '_ ` _ / / _ /    | |/ _   \n");
    printf("/  //  /  __/ | (_| (_) | | | | | |  __/   | | (_) |\n");
    printf(" //  // /___|_|/___/___/|_| |_| |_|/___|   /_//___/ \n");
    printf("                The Game Library                    \n");
    printf("         Press Any Button to Continue               \n");
    getch();
    system("cls");


    do {
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
        case 1 : loginUser(player);
        break;

        case 2 : createUser(player);
        break;

        case 3 : 
        break;

        default :
        break;
    }
    } while(trigger == 0);

}

int main() {
    int pilihan, trigger;

    user *player = NULL;

    loginPageMenu(&player);

    do {
        printf(" +-------------------------------------------------+\n");
        printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
        printf(" +-------------------------------------------------+\n");
        printf(" | No. |              OPSI                         |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  1  | Tambahkan Data                            |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  2  | Lihat Data                                |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  3  | Exit Program                              |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch(pilihan) {
            case 1 : TambahData(&player);
            break;

            case 2 : TampilkanData(player);
            break;

            case 3 : 
            break;

            default :
            break;
        }
    } while(trigger == 0);

    return 0;

}