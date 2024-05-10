#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "struct.h"


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
    
    newUser->Games = NULL;
    *playerLogin = newUser; 
}

void loginUser(user **player, int numPlayer) {
    int trigger = 0, found = 0, loginKey = -1;
    char bufferUsername[100], bufferPassword[100];

    do {
        printf("||         USER LOGIN PAGE         ||\n");
        printf("Enter Username  : ");
        scanf("%99s", &bufferUsername);
        printf("Enter Password  : ");
        scanf("%99s",&bufferPassword);

        for (int i = 0; i < numPlayer; i++) {
            if (strcmp(bufferUsername, player[i]->gamerTag) == 0 && strcmp(bufferPassword, player[i]->password) == 0) {
                printf("Login Successful!\n");
                found = 1;
                loginKey = i;
                break;
            }
        }

        if (found == 1) {
            TampilkanData(&player[loginKey], loginKey);
            break; 
        } else {
            printf("We couldn't find your username / The password is wrong\n You have %d attempts left\n", 2 - trigger);
            trigger++;
            if (trigger == 3) {
                printf("Maximum login attempts exceeded.\n");
                break;
            }
        }
        fflush(stdin); 
    } while (trigger == 0);
}

void loginPageMenu(user **player) {

    int pilihan, trigger = 0;
    system("cls");
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
        case 1 : loginUser(&player, 100);
        break;

        case 2 : createUser(&player);
        break;

        case 3 : trigger = 1;
        break;

        default : printf("Please input the correct value!");
        break;
    }
    } while(trigger == 0);

}