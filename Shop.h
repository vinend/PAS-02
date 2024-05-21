#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "Struct.h"
#include "SearchingSorting.h"

// Deklarasi fungsi-fungsi yang digunakan
void redeemGame(user **player[], NodeGames *Shop, int loginKey);
void DataShop(user **player[], NodeGames *Shop, int loginKey);
void AddShop(NodeGames **Shop);
void menuShopGames(user **player[], NodeGames *Shop, int Data);
void addGameToLibrary(user **player[], NodeGames *selectedGame, int loginKey);
void appendGameToFile(user *player, NodeGames *selectedGame);

void DataShop(user **player[], NodeGames *Shop, int loginKey) {
    int pilihan, trigger = 0;
    char status[40];

    // Periksa apakah data toko tersedia
    if (Shop == NULL) {
        printf("Error: Shop data is not available.\n");
        getch();
        return;
    }

    int i = 0; // Inisialisasi variabel untuk nomor game
    NodeGames *current = Shop; // Pointer ke game saat ini dalam daftar

    do {
        system("cls");  // Bersihkan layar
        printf("Games List:\n");
        printf("____________________________________________________________________________________\n");
        printf("|No   |Title                                     |Publisher           |Genre      |Price   |Rating |\n");
        printf("|_____|_________________________________________|____________________|___________|________|_______|\n");

         // Iterasi melalui daftar game dan tampilkan
        while (current != NULL) {
            printf("|%-4d |%-40s |%-20s |%-10s |%-7.2f |%-6.2f |\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
            i++;
            current = current->next;
        }
        printf("|_____|_________________________________________|____________________|___________|________|_______|\n");

        // Tanyakan kepada pengguna apakah mereka ingin membeli game
        printf("Buy a game? Yes / No: "); scanf("%s", &status);

        if(strcmp(status, "Yes") == 0) {
            // Minta pengguna memilih game untuk ditambahkan ke perpustakaan
            printf("Select a game to add to your library (enter the number): ");
            scanf("%d", &pilihan);

            if (pilihan < 1 || pilihan > i) {
            printf("Invalid selection. Please try again.\n");
            getch();
            return;
            }

            current = Shop;
            for (int j = 1; j < pilihan; j++) {
                current = current->next;
            }

            // Tambahkan game ke perpustakaan pengguna
            addGameToLibrary(player, current, loginKey);
            appendGameToFile((*player)[loginKey], current);

            printf("Game added to your library!\n");
            getch();
        }

        else if(strcmp(status, "No") == 0) {
            system("cls");
            printf("Thank you for browsing!");
            getch();
            trigger == 1; // Setel trigger untuk keluar dari loop
        }

        else {
            printf("Please input a valid answer!");
        }
    } while(trigger == 1);
}

void AddShop(NodeGames** Shop) {
    // Membuka file
    FILE *file = fopen("Shop.txt", "a");

    // Error handling jika file tidak dapat dibuka
    if (file == NULL) {
        perror("Failed to open Shop.txt");
        return;
    }

    // Pendeklarasian variabel
    char title[50], publisher[50], genre[50], description[200];
    float price, rating;

    // Memasukkan game yang ingin dimasukkan
    printf("Enter game title: ");
    scanf(" %[^\n]%*c", title);
    printf("Enter game genre: ");
    scanf(" %[^\n]%*c", genre);
    printf("Enter game description: ");
    scanf(" %[^\n]%*c", description);
    printf("Enter game publisher: ");
    scanf(" %[^\n]%*c", publisher);
    printf("Enter game rating: ");
    scanf("%f", &rating);
    printf("Enter game price: ");
    scanf("%f", &price);

    // Append data ke dalam file
    fprintf(file, "Game: %s, %s, %s, %s, %.1f, %.2f\n", title, genre, description, publisher, rating, price);
    fclose(file);

    // Mengalokasikan memori untuk node baru
    NodeGames* newNode = (NodeGames*)malloc(sizeof(NodeGames));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new game node");
        return;
    }

    // Memasukkan data ke dalam node
    strcpy(newNode->title, title);
    strcpy(newNode->publisher, publisher);
    strcpy(newNode->genre, genre);
    newNode->price = price;
    newNode->rating = rating;
    newNode->next = NULL;

    // Memasukkan node baru ke akhir linked list
    if (*Shop == NULL) {
        *Shop = newNode;
    } else {
        NodeGames* current = *Shop;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    printf("Game telah ditambahkan.\n");
    getch();
}


void addGameToLibrary(user **player[], NodeGames *selectedGame, int loginKey) {
    // Alokasikan memori untuk game baru
    NodeGames *newGame = (NodeGames *)malloc(sizeof(NodeGames));
    if (!newGame) {
        fprintf(stderr, "Memory allocation failed for new game\n");
        return; // Kembalikan jika alokasi memori gagal
    }       

     // Salin informasi game dari game yang dipilih ke game baru
    strcpy(newGame->title, selectedGame->title);
    strcpy(newGame->genre, selectedGame->genre);
    strcpy(newGame->desc, selectedGame->desc);
    strcpy(newGame->publisher, selectedGame->publisher);
    newGame->rating = selectedGame->rating;
    newGame->price = selectedGame->price;
    // Tambahkan game baru ke awal daftar game pengguna
    newGame->next = (*player)[loginKey]->Games;
    (*player)[loginKey]->Games = newGame; // Setel game baru sebagai head dari daftar game pengguna
}

void appendGameToFile(user *player, NodeGames *selectedGame) {
    // Buka file games.txt untuk menambahkan data game baru
    FILE *file = fopen("games.txt", "a");
    if (!file) {
        perror("Failed to open games.txt");
        return; // Kembalikan jika gagal membuka file
    }

    // Tulis data game baru ke file
    fprintf(file, "GamerTag: %s; Game: %s, %s, %s, %s, %.1f, %.2f\n",
            player->gamerTag, selectedGame->title, selectedGame->genre, selectedGame->desc, selectedGame->publisher, selectedGame->rating, selectedGame->price);
    
    // Tutup file setelah selesai menulis
    fclose(file);
}

// Function untuk meredeem code untuk game
void redeemGame(user **player[], NodeGames *Shop, int loginKey) {
    // Pendeklarasian variabel
    int j = 0;
    int found = 0;
    char gameKey[50];

    NodeGames *current = Shop;

    // Memasukkan key untuk di redeem
    printf("Enter the Game Key to Redeem: "); scanf("%s", &gameKey);
        // Mencari key yang sesuai
        while (current != NULL) {
            if (strcmp(gameKey, current->gameKey) == 0) {
                    found = 1;
                    addGameToLibrary(player, current, loginKey);
                    appendGameToFile((*player)[loginKey], current);
                    printf("%s added to your library!\n", current->title);
                    break;
            }
            current = current->next;
        }

    // Jika tidak ditemukan key yang sesuai
    if (!found) {
        system("cls");
        printf("Redeem Key Not Valid!\n");
        getch();
    }

    getch();
    system("cls");
}

// Function untuk menu pada shop
void menuShopGames(user **player[], NodeGames *Shop, int loginkey) {
    // Pendeklarasian variabel
    int pilihan, trigger = 0;
    
    // Looping jika pilihan belum keluar menu
    do {
        system("cls");
        printf(" +-------------------------------------------------+\n");
        printf(" |          SELAMAT DATANG DI GAME SHOP            |\n");
        printf(" +-------------------------------------------------+\n");
        printf(" | No. |              OPSI                         |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  1  | Shop For Games                            |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  2  | Search Games                              |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  3  | Redeem Game                               |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  4  | Return to Menu                            |\n");
        printf(" +-----+-------------------------------------------+\n");

        // Pilih opsi
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch (pilihan) {
            // Pilihan shop games
            case 1:
                DataShop(player, Shop, loginkey);
                break;
            
            // Pilihan untuk searching game
            case 2:
                searchingGame(Shop);
                break;

            // Pilihan untuk meredeem game
            case 3 :
                redeemGame(player, Shop, loginkey);

            // Pilihan untuk keluar dari menu
            case 4:
                trigger = 1;
                break;

            // Jika pilihan diluar dari yang disediakan
            default:
                printf("Invalid option. Please try again.\n");
                getch();
                break;
        }
    } while (trigger == 0);
}