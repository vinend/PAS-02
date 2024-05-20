#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "struct.h"
#include "Shop.h"

// Deklarasi fungsi-fungsi yang digunakan
void lihatData(user **player[], int loginKey);
void libraryMenu(user **player[], int loginKey, NodeGames *Shop);
void pilihSort(user **player[], int jumlahData, int loginKey);
void pilihSearch(user **player[], int jumlahData, int loginKey);
void userSettings(user **player[], int loginKey);
void updatePasswordInFile(const char *gamerTag, const char *newPassword);

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
    getch();
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
        printf(" |  7. | Logging Out                               |\n");
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
            AddShop(Shop);
            break;

            // Pilihan menu untuk meyetting user
            case 6 : 
            userSettings(player, loginKey);
            break;

            // Pilihan keluar dari menu ini
            case 7 :
            trigger = 1;
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
            MergeSort(&(*player)[loginKey]->Games, pilihan, pilihanSort);
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
void updatePasswordInFile(const char *gamerTag, const char *newPassword) {
    // Membuka file players.txt
    FILE *file = fopen("players.txt", "r+");

    // Jika gagal membuka file
    if (!file) {
        perror("Failed to open the file");
        return;
    }

    // Pendeklarasian variabel
    char buffer[256];
    long offset = 0;
    int found = 0;

    // Looping untuk mencari line yang sesuai dengan user yang diganti passwordnya
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "GamerTag:") != NULL && strstr(buffer, gamerTag) != NULL) {
            found = 1;

            // Membaca line sekarang
            fgets(buffer, sizeof(buffer), file);

            // Menghitung start position line tersebut
            offset = ftell(file) - strlen(buffer);  
            break;
        }
    }

    // Jika ditemukan
    if (found) {
        // Set pointer ke awal dari start position tersebut
        fseek(file, offset, SEEK_SET);  

        // Menghitung panjang password sebelumnya
        int currentPassLength = strlen(strchr(buffer, ':') + 2) - 1;  
        int newPassLength = strlen(newPassword);

        // Memasukkan password baru dengan format yang benar
        char newPassLine[256];
        int written = snprintf(newPassLine, sizeof(newPassLine), "Password: %s\n", newPassword);

        // Jika password lebih panjang akan di trunct agar muat
        if (written > currentPassLength) {  
            newPassLine[currentPassLength + 10] = '\n'; 
            newPassLine[currentPassLength + 11] = '\0';
        } 

        // Jika password lebih pendek akan diberi space agar sesuai
        else if (written < currentPassLength) {  
            memset(newPassLine + written - 1, ' ', currentPassLength - written + 1);  
            newPassLine[currentPassLength + 10] = '\n';
            newPassLine[currentPassLength + 11] = '\0';
        }

        // Menuliskan password baru
        fputs(newPassLine, file);
    } 

    // Jika tidak ditemukan gamertag yang sesuai
    else {
        printf("GamerTag not found.\n");
        getch();
    }

    // Menutup file
    fclose(file);
}