#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "Struct.h"
#include "Shop.h"
#define MAX_CODE_LENGTH 20

// Deklarasi fungsi-fungsi yang digunakan
void lihatData(user **player[], int loginKey);
void libraryMenu(user **player[], int loginKey, NodeGames *Shop);
void pilihSort(user **player[], int jumlahData, int loginKey);
void pilihSearch(user **player[], int jumlahData, int loginKey);
void userSettings(user **player[], int loginKey);
void updatePasswordInFile(const char *gamerTag, const char *newPassword);
void generateTopUpCode(char *code, int length);
void topUpUang(const char *filename, float amount);
float redeemUang(const char *filename, const char *code);
void updateUangInFile(const char* gamerTag, double newUang);
void refundGame(user **player[], int loginKey);
void deleteGameFromFile(const char *gamerTag, const char *gameTitle);


// Function untuk melihat data dari player
void lihatData(user **player[], int loginKey) {
    // Error handling jika tidak ada data pada player
    if (player == NULL || *player == NULL) {
        printf("Error: Player data is not available.\n");
        getch();
        return;
    }

    // Error handling jika tidak ada game di library
    if ((*player)[loginKey]->Games == NULL) {
        printf("No games in your library for %s.\n", (*player)[loginKey]->gamerTag);
        getch();
        return;
    }

    // Deklarasi variabel
    int i = 0;
    NodeGames *current = (*player)[loginKey]->Games;

    printf("Data for %s:\n", (*player)[loginKey]->gamerTag);
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Print seluruh game yang dimiliki player tersebut
    while (current != NULL) {
        printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
        i++;
        current = current->next;
    }
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    printf("Jumlah uang sekarang : %.2f", (*player)[loginKey]->Uang);
    getch();
}

void refundGame(user **player[], int loginKey) {
    if (player == NULL || *player == NULL) {
        printf("Error: Player data is not available.\n");
        getch();
        return;
    }

    if ((*player)[loginKey]->Games == NULL) {
        printf("No games in your library for %s.\n", (*player)[loginKey]->gamerTag);
        getch();
        return;
    }

    int i = 0;
    NodeGames *current = (*player)[loginKey]->Games;
    NodeGames *prev = NULL;

    printf("Games in your library:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    while (current != NULL) {
        printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
        current = current->next;
        i++;
    }
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    int choice;
    printf("Select the game number you want to refund: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > i) {
        printf("Invalid selection. Please try again.\n");
        getch();
        return;
    }

    current = (*player)[loginKey]->Games;
    for (int j = 1; j < choice; j++) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        (*player)[loginKey]->Uang += current->price;
        if (prev == NULL) {
            (*player)[loginKey]->Games = current->next;
        } else {
            prev->next = current->next;
        }
        updateUangInFile((*player)[loginKey]->gamerTag, (*player)[loginKey]->Uang);
        deleteGameFromFile((*player)[loginKey]->gamerTag, current->title);
        free(current);

        printf("Game refunded successfully! New balance: %.2f\n", (*player)[loginKey]->Uang);
    } else {
        printf("Error processing the refund.\n");
    }

    getch();
}

void deleteGameFromFile(const char *gamerTag, const char *gameTitle) {
    FILE *file = fopen("games.txt", "r");
    if (file == NULL) {
        perror("Failed to open games.txt");
        return;
    }

    char tempFilename[] = "temp_games.txt";
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        perror("Failed to open temporary file");
        fclose(file);
        return;
    }

    char buffer[1024];
    char currentGamerTag[100];
    char currentTitle[100];
    int keepLine = 1;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "GamerTag: %99[^;]; Game: %99[^,],", currentGamerTag, currentTitle) == 2) {
            if (strcmp(currentGamerTag, gamerTag) == 0 && strcmp(currentTitle, gameTitle) == 0) {
                keepLine = 0;
            } else {
                keepLine = 1;
            }
        }

        if (keepLine) {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("games.txt");
    rename(tempFilename, "games.txt");
}

// Function untuk menu utama library game
void libraryMenu(user **player[], int loginKey, NodeGames *Shop){
    // Pendeklarasian variabel
    int trigger = 0;
    int i = 0, pilihan, O = 0;

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Membuat variabel untuk node game
    NodeGames *currentGame = (*player)[loginKey]->Games;

    // Print seluruh data player
    while (currentGame != NULL) {
        printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n",
            i+1, currentGame->title, currentGame->publisher, currentGame->genre, currentGame->price, currentGame->rating);
        currentGame = currentGame->next;
        i++;
    }
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    printf("Jumlah uang sekarang : %.2f", (*player)[loginKey]->Uang);
    getch();
    
    // Looping jika belum pilihan keluar menu
    do {
        int pilihan;
        system("cls");
        printf(" +-------------------------------------------------+\n");
        printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
        printf(" +-------------------------------------------------+\n");
        printf(" | No. |              OPSI                         |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  1  | Lihat Game                                |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  2  | Sorting Game                              |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  3  | Search Game                               |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  4  | Shop for Games                            |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  5. | Publish a Game                            |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  6. | User Settings                             |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  7. | Top Up Uang                               |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  8. | Redeem Wallet                             |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  9. | Refund Game                               |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  10.| Logout dari Akun                          |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        // Memilih pilihan menu utama
        switch(pilihan) {
            
            // Pilihan menu lihat game
            case 1 :
            lihatData(player, loginKey);
            break;
            
            // Pilihan menu sorting game
            case 2 : 
            pilihSort(player, i, loginKey);
            break;

            // Pilihan menu search game 
            case 3 : 
            pilihSearch(player, i, loginKey);
            break;

            // Pilihan menu untuk shop game
            case 4 : 
            menuShopGames(player, Shop, loginKey);
            break;

            // Pilihan menu untuk mempublish sebuah game
            case 5 :
            AddShop(&Shop);
            break;

            // Pilihan menu untuk meyetting user
            case 6 : 
            userSettings(player, loginKey);
            break;

            // Pilihan keluar dari menu ini
            case 10 :
            trigger = 1;
            break;

            case 9 : 
            refundGame(player, loginKey);
            break;

            case 7 : 
            float amount;
            printf("Masukkan jumlah uang yang ingin di topup : ");
            scanf("%f", &amount);
            topUpUang("code.txt", amount);
            getch();
            break;

            case 8 : 
            char code[MAX_CODE_LENGTH + 1];
            printf("Enter the code to redeem: ");
            scanf("%s", code);
            float redeemedAmount = redeemUang("code.txt", code);
            printf("Redeemed Amount : %.2f\n", redeemedAmount);
            if (redeemedAmount > 0) {
                (*player)[loginKey]->Uang += redeemedAmount;
                printf("Uang sekarang menjadi : %.2f", (*player)[loginKey]->Uang);
                updateUangInFile((*player)[loginKey]->gamerTag, (*player)[loginKey]->Uang);
                getch();
            } 
            else {
                printf("Invalid code or already redeemed.\n");
                getch();
            }
            break;

            // Jika pilihan diluar yang disediakan
            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
    } while(trigger == 0); // Loop berlanjut jika trigger masih nol
}

// Function untuk memilih jenis sort pada data player
void pilihSort(user **player[], int jumlahData, int loginKey){
    system("cls");
    
    // Pendeklarasian variabel
    int pilihan, pilihanSort;
    printf(" +-------------------------------------------------+\n");
    printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
    printf(" +-------------------------------------------------+\n");
    printf(" | No. |              OPSI                         |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  1  | Sorting Rating                            |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  2  | Sorting Harga                             |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  3  | Sorting Nama                              |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  4  | Sorting Genre                             |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  5  | Sorting Publisher                         |\n");
    printf(" +-----+-------------------------------------------+\n");

    // Memasukkan pilihan sorting
    printf("Pilih Opsi: "); scanf("%d", &pilihan);
    printf("Mau Naik Atau Turun (1 = Naik, 2 = Turun) : "); scanf("%d", &pilihanSort);

    // Switch case dengan falling down
    switch(pilihan) {
            // Pilihan sorting rating
            case 1 : 

            // Pilihan sorting harga
            case 2 : 

            // Pilihan sorting nama
            case 3 : 

            // Pilihan sorting genre
            case 4 : 

            // Pilihan sorting publisher
            case 5 : 
            if(pilihanSort == 2 || pilihanSort == 1){
                MergeSort(&(*player)[loginKey]->Games, pilihan, pilihanSort);
                break;
            }
            else{
                printf("Input tidak benar !");
                getch();
            }
            
            break;

            // Jika pilihan diluar yang disediakan
            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}

// Function untuk memilih jenis searching
void pilihSearch(user **player[], int jumlahData, int loginKey){
    system("cls");
    
    // Pendeklarasian variabel
    int pilihan;
    char Searching[100];

    printf(" +-------------------------------------------------+\n");
    printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
    printf(" +-------------------------------------------------+\n");
    printf(" | No. |              OPSI                         |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  1  | Searching Title                           |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  2  | Searching Genre                           |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  3  | Searching Publisher                       |\n");
    printf(" +-----+-------------------------------------------+\n");

    // Memilih pilihan searchinh
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
            // Pilihan searching title
            case 1 : 
            system("cls");
            printf("Masukkan Title yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            // Pilihan searching genre
            case 2 : 
            system("cls");
            printf("Masukkan Genre yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            // Pilihan searching publisher
            case 3 : 
            system("cls");
            printf("Masukkan Publisher yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            // Jika pilihan diluar yang disediakan
            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}
// Function untuk menu user setting
void userSettings(user **player[], int loginKey) {
    // Pendeklarasian variabel
    int trigger = 0, found = 0;
    char bufferPassword[100], newPassword[100];

    system("cls");
    printf("||         Please Enter Your Password Again         ||\n");

    // Looping untuk login
    while (trigger < 3 && !found) {
        // Memasukkan password
        system("cls");
        printf("Enter Password: ");
        scanf(" %99s", bufferPassword);

        // Jika password sesuai
        if (strcmp(bufferPassword, (*player)[loginKey]->password) == 0) {
            printf("Login Successful!\n");
            found = 1;
            break;
        } 
        // Jika password tidak sesuai akan diberi 3 attempt
        else {
            printf("The password is wrong. You have %d attempts left.\n", 2 - trigger);
            getch();
            trigger++;
        }

        // Jika attempt sudah 3x
        if (trigger == 3) {
            printf("Maximum login attempts exceeded.\n");
            getch();
            return;
        }
    }

    // Jika password benar
    if (found) {
        // Memasukkan password baru
        printf("Enter new password: ");
        scanf(" %99s", newPassword);
        strcpy((*player)[loginKey]->password, newPassword);
        printf("Password updated successfully!\n");

        // Mengupdate password menjadi yang baru
        updatePasswordInFile((*player)[loginKey]->gamerTag, newPassword);
    }
}

// Function untuk mengupdate password
void updatePasswordInFile(const char* gamerTag, const char* newPassword) {
    FILE *file = fopen("players.txt", "r");
    if (file == NULL) {
        perror("Gagal membuka players.txt");
        return;
    }

    char tempFilename[] = "temp.txt";
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        perror("Gagal membuka temp.txt");
        fclose(file);
        return;
    }

    char line[256];
    char currentGamerTag[256];
    int isGamerTagLine = 0;
    int passwordUpdated = 0;

    // Membaca setiap baris dari file asli
    while (fgets(line, sizeof(line), file)) {
        // Memeriksa apakah baris adalah GamerTag
        if (strncmp(line, "GamerTag: ", 10) == 0) {
            isGamerTagLine = 1;
            strcpy(currentGamerTag, line + 10);
            currentGamerTag[strcspn(currentGamerTag, "\n")] = '\0'; // Menghapus karakter newline
        }

        // Memeriksa apakah baris adalah Password untuk GamerTag yang relevan
        if (isGamerTagLine && strcmp(currentGamerTag, gamerTag) == 0 && strncmp(line, "Password: ", 10) == 0) {
            fprintf(tempFile, "Password: %s\n", newPassword); // Menulis password baru ke file sementara
            passwordUpdated = 1;
            isGamerTagLine = 0; // Reset flag
        } else {
            fprintf(tempFile, "%s", line); // Menulis baris asli ke file sementara
        }

        if (strncmp(line, "Password: ", 10) == 0) {
            isGamerTagLine = 0; // Reset flag setelah baris password
        }
    }

    fclose(file);
    fclose(tempFile);

    // Mengganti file lama dengan file baru
    remove("players.txt");
    rename(tempFilename, "players.txt");

    // Menambahkan password baru jika belum ada
    if (!passwordUpdated) {
        file = fopen("players.txt", "a");
        if (file == NULL) {
            perror("Gagal membuka players.txt untuk menambahkan data");
            return;
        }
        fprintf(file, "GamerTag: %s\nPassword: %s\n", gamerTag, newPassword);
        fclose(file);
    }

    printf("Password berhasil diperbarui.\n");
}

// Function to generate a random top-up code
void generateTopUpCode(char *code, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (int i = 0; i < length; i++) {
        int index = rand() % (int)(sizeof(charset) - 1);
        code[i] = charset[index];
    }

    code[length] = '\0';
}

// Function to top up primogems with a new code
void topUpUang(const char *filename, float amount) {
    char code[MAX_CODE_LENGTH + 1];
    generateTopUpCode(code, MAX_CODE_LENGTH);
    FILE *file = fopen(filename, "w+");
    printf("Code untuk top up adalah : %s", code);
    fprintf(file, "%s %f", code, amount);
    fclose(file);
}

// Function to redeem primogems with a code
float redeemUang(const char *filename, const char *code) {
    char Kode[100];
    float Uang;
    FILE *file = fopen(filename, "r+");
    fscanf(file,"%20s %f", Kode, &Uang);
    printf("Uang %.2f\n", Uang);
    if(strcmp(Kode,code) == 1){
        fclose(file);
        return 0;
    }
    else{
        fclose(file);
        return Uang;
    }
}

void updateUangInFile(const char* gamerTag, double newUang) {
    FILE *file = fopen("players.txt", "r");
    if (file == NULL) {
        perror("Gagal membuka players.txt");
        return;
    }

    char tempFilename[] = "temp.txt";
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        perror("Gagal membuka temp.txt");
        fclose(file);
        return;
    }

    char line[256];
    char currentGamerTag[256];
    int isGamerTagLine = 0;
    int uangUpdated = 0;

    // Membaca setiap baris dari file asli
    while (fgets(line, sizeof(line), file)) {
        // Memeriksa apakah baris adalah GamerTag
        if (strncmp(line, "GamerTag: ", 10) == 0) {
            isGamerTagLine = 1;
            strcpy(currentGamerTag, line + 10);
            currentGamerTag[strcspn(currentGamerTag, "\n")] = '\0'; // Menghapus karakter newline
        }

        // Memeriksa apakah baris adalah Uang untuk GamerTag yang relevan
        if (isGamerTagLine && strcmp(currentGamerTag, gamerTag) == 0 && strncmp(line, "Uang : ", 7) == 0) {
            fprintf(tempFile, "Uang : %.2f\n", newUang); // Menulis uang baru ke file sementara
            uangUpdated = 1;
            isGamerTagLine = 0; // Reset flag setelah baris uang
        } else {
            fprintf(tempFile, "%s", line); // Menulis baris asli ke file sementara
        }

        if (strncmp(line, "Uang : ", 7) == 0) {
            isGamerTagLine = 0; // Reset flag setelah baris uang
        }
    }

    fclose(file);
    fclose(tempFile);

    // Mengganti file lama dengan file baru
    remove("players.txt");
    rename(tempFilename, "players.txt");

    // Menambahkan uang baru jika belum ada
    if (!uangUpdated) {
        file = fopen("players.txt", "a");
        if (file == NULL) {
            perror("Gagal membuka players.txt untuk menambahkan data");
            return;
        }
        fprintf(file, "GamerTag: %s\nUang : %.2f\n", gamerTag, newUang);
        fclose(file);
    }

    printf("Uang berhasil diperbarui.\n");
}
