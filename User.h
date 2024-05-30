#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "Struct.h"
#include "MainMenu.h"

// Deklarasi fungsi-fungsi yang digunakan
void generateGameKeys(NodeGames *Shop);
void appendGameKeysToFile(NodeGames *Shop);
void generateGameKey(char *key, int length);
void appendUser(const char username[], const char password[], float Uang);
void flushInput();
void randomPassGen(char *pass, int length);
void createUser(user **playerLogin, int *numPlayerLogin);
void loginUser(user **player, int numPlayer, NodeGames *Shop);
void loginPageMenu(user **player, NodeGames *Shop, int numPlayers);

// Function untuk flush input
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Function untuk membuat password secara random
void randomPassGen(char *pass, int length) {
    // Membuat charset karakter yang dapat dirandomize
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));

    // Membuat password secara random dengan parallel programming
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        pass[i] = charset[key];
    }
    pass[length] = '\0';
}

// Function untuk membuat user baru
void createUser(user **playerLogin, int *numPlayersLogin) {
    // Pendeklarasian variabel
    char bufferUsername[100];
    char bufferPassword[100];
    float bufferUang;
    user *newUser = (user*)malloc(sizeof(user));

    // Error handling jika pengalokasian memori gagal
    if (newUser == NULL) {
        printf("Memory allocation failed.\n");
        getch();
        return;
    }

    system("cls");

    // Membuat gamertag baru
    printf("Create GamerTag: ");
    scanf("%99s", newUser->gamerTag);
    flushInput();

    system("cls");

    // Pendeklarasian variabel
    char buffer[10];
    int trigger = 0;

    // Looping untuk membuat password
    do {
        system("cls");

        // Memilih apakah password ingin dibuat secara otomatis
        printf("Suggest Password? (Yes/No): ");
        scanf("%9s", buffer);
        flushInput();

        // Jika ingin dibuat secara otomatis
        if (strcmp(buffer, "Yes") == 0) {
            #pragma omp parallel
            randomPassGen(newUser->password, 10); 
            printf("Your suggested password is: %s\n", newUser->password);
            getch(); system("cls");
            printf("Create Password: ");
            scanf("%99s", newUser->password);
            trigger = 1;

        // Jika tidak ingin dibuat secara otomatis
        } else if (strcmp(buffer, "No") == 0) {
            system("cls");
            printf("Create Password: ");
            scanf("%99s", newUser->password);
            trigger = 1;

        // Error handling jika input diluar yang disediakan
        } else {
            printf("Invalid input. Please type 'Yes' or 'No'.\n");
            getch();
        }
    } while (trigger == 0);
    printf("Masukkan uang yang dimiliki sekarang : ");
    scanf("%f", bufferUang);
    // Copy input ke buffer
    strcpy(bufferUsername, newUser->gamerTag);
    strcpy(bufferPassword, newUser->password);
    newUser->Uang = bufferUang;

    // Append user baru ke dalam file untuk file handling
    appendUser(bufferUsername, bufferPassword, bufferUang);
    
    system("cls");
    
    // Membuat game pada user baru menjadi NULL
    newUser->Games = NULL;
    playerLogin[*numPlayersLogin] = newUser;
    *numPlayersLogin = *numPlayersLogin + 1; 
}

// Function untuk mengappend user
void appendUser(const char username[], const char password[], float Uang) {
    // Membuka file players.txt
    FILE *file = fopen("players.txt", "a");

    // Jika gagal membuka file
    if (file == NULL) {
        perror("Failed to open file for appending");
        getch();
        return;
    }

    // Mengappend user ke dalam file
    fprintf(file, "\nGamerTag: %s\n", username);
    fprintf(file, "Password: %s\n", password);
    fprintf(file, "Uang: %.2f", Uang);

    // Menutup kembali file tersebut
    fclose(file);
}

// Function untuk login user
void loginUser(user **player, int numPlayer, NodeGames *Shop) {
    // Pendeklarasian variabel
    int trigger = 0, found = 0, loginKey = -1;

    system("cls");
    printf("||         USER LOGIN PAGE         ||\n");

    // Jika trigger dibawah 3 akan lanjut looping
    while (trigger < 3) {
        system("cls");

        // Pendeklarasian buffer
        char bufferUsername[100], bufferPassword[100];

        // Memasukkan username dan password untuk login
        printf("Enter Username: ");
        scanf(" %99s", bufferUsername);
        printf("Enter Password: ");
        scanf(" %99s", bufferPassword);

        // Memeriksa input login
        for (int i = 0; i < numPlayer; i++) {
            if (strcmp(bufferUsername, player[i]->gamerTag) == 0 && strcmp(bufferPassword, player[i]->password) == 0) {
                printf("Login Successful!\n");
                found = 1;
                loginKey = i;
                break;
            }
        }

        // Jika ditemukan
        if (found) {
            // Melanjutkan ke library menu
            libraryMenu(&player, loginKey, Shop);
            return; 
        } 
        // Jika tidak ditemukan user tersebut
        else {
            // Akan diberikan kesempatan sebanyak 3 kali percobaan sebelum keluar kembali dari menu
            printf("We couldn't find your username / The password is wrong. You have %d attempts left.\n", 2 - trigger);
            getch(); 

            // Increment trigger
            trigger++;

            // Jika trigger sudah 3 kali (sudah 3x percobaan)
            if (trigger == 3) {
                printf("Maximum login attempts exceeded.\n");
                getch(); 
            }
        }
    }

    // Jika user tidak ditemukan
    if (!found) {
        printf("Returning to the main menu...\n");
        getch(); 
        system("cls");
    }
}

// Function untuk menu login page
void loginPageMenu(user **player, NodeGames *Shop, int numPlayers) {
    // Pendeklarasian variabel
    int pilihan, trigger = 0;
    int numPlayersLogin;

    numPlayersLogin = numPlayers;

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

    // Looping sampai opsi keluar
    do {
        printf("Jumlah Player: %d", numPlayersLogin);
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

        // Memasukkan pilihan menu login
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch(pilihan) {
            // Pilihan login user
            case 1: 
                loginUser(player, numPlayersLogin, Shop);
                break;

            // Pilihan menambahkan user
            case 2: 
                createUser(player, &numPlayersLogin);
                break;

            // Pilihan exit program
            case 3: 
                trigger = 1;
                break;
            
            // Jika pilihan diluar yang disediakan
            default: 
                printf("Please input the correct value!");
                getch();
                break;
        }
    } while (trigger == 0);
}

void generateGameKeys(NodeGames *Shop) {
    NodeGames *current = Shop;
    int count = 0;

    // Pertama, hitung jumlah node dalam linked list
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Buat array pointer ke NodeGames
    NodeGames **gameArray = (NodeGames **)malloc(count * sizeof(NodeGames *));
    if (!gameArray) {
        fprintf(stderr, "Memory allocation failed for gameArray\n");
        return;
    }

    // Isi array dengan pointer ke node
    current = Shop;
    for (int i = 0; i < count; i++) {
        gameArray[i] = current;
        current = current->next;
    }

     // Generate kunci secara paralel
    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        char key[21];
        generateGameKey(key, 20); // Fungsi untuk menghasilkan kunci game
        strcpy(gameArray[i]->gameKey, key); // Salin kunci ke dalam node
    }

    // Bebaskan memori yang dialokasikan untuk array
    free(gameArray);
}

void generateGameKey(char *key, int length) {
    // Karakter yang akan digunakan untuk membuat kunci
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    // Generate kunci secara paralel
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        int index = rand() % (sizeof(charset) - 1);
        key[i] = charset[index];
    }
    key[length] = '\0'; // Akhiri string dengan null terminator
}


void appendGameKeysToFile(NodeGames *Shop) {
    // Buka file untuk menulis kunci game
    FILE *file = fopen("gameKeys.txt", "w");
    if (!file) {
        perror("Failed to open gameKeys.txt");
        return;
    }

    // Iterasi melalui linked list dan tulis kunci game ke file
    NodeGames *current = Shop;
    while (current != NULL) {
        // Tulis judul game dan kunci ke file
        fprintf(file, "Game: %s, Key: %s\n", current->title, current->gameKey);
        current = current->next; // Pindah ke node berikutnya
    }

    // Tutup file setelah selesai
    fclose(file);
}
