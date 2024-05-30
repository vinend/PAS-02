#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "Struct.h"

// Deklarasi fungsi-fungsi yang digunakan
void readPlayersFromFile(user **players, int *numPlayers);
void readGamesFromFileForUsers(user **players, int numPlayers);
void readShopFromFileForUsers(NodeGames **Shop);
NodeGames* createNode(char *Title, char *Genre, char *Desc, char *Publisher, int Rating, int Price);

void readPlayersFromFile(user **players, int *numPlayers) {
    // Buka file players.txt untuk membaca data pengguna
    FILE *file = fopen("players.txt", "r");
    if (!file) {
        printf("Failed to open the file.\n");
        getch();
        return;
    }

    char buffer[1024]; // Buffer untuk membaca baris dari file
    int index = 0; // Indeks untuk array pemain
    user *newUser; // Pointer untuk menyimpan data pengguna baru

    // Membaca setiap baris dari file hingga akhir file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        newUser = (user *)malloc(sizeof(user));
        if (newUser == NULL) {
            printf("Memory allocation failed.\n");
            break;
        }
        
        // Baca gamerTag dari buffer
        if (sscanf(buffer, "GamerTag: %99[^\n]", newUser->gamerTag) == 1) {
            // Baca password dari baris berikutnya dalam file
            if (fgets(buffer, sizeof(buffer), file) && sscanf(buffer, "Password: %99[^\n]", newUser->password) == 1) {
                // Baca uang dari baris berikutnya dalam file
                if (fgets(buffer, sizeof(buffer), file) && sscanf(buffer, "Uang : %f", &newUser->Uang) == 1) {
                    newUser->Games = NULL; // Inisialisasi pointer ke game sebagai NULL
                    players[index++] = newUser; // Tambahkan pengguna baru ke array pemain
                } else {
                    free(newUser); // Bebaskan memori jika membaca uang gagal
                }
            } else {
                free(newUser); // Bebaskan memori jika membaca password gagal
            }
        } else {
            free(newUser); // Bebaskan memori jika membaca gamerTag gagal
        }
    }

    fclose(file); // Tutup file setelah selesai membaca
    *numPlayers = index; // Set jumlah pemain yang berhasil dibaca
}

void readGamesFromFileForUsers(user **players, int numPlayers) {
     // Buka file games.txt untuk membaca data game
    FILE *file = fopen("games.txt", "r");
    if (!file) {
        perror("Failed to open the file");
        getch();
        return;
    }

    char buffer[1024]; // Buffer untuk membaca baris dari file
    char gamerTag[100];  // Buffer untuk menyimpan gamerTag
    NodeGames *newGame; // Pointer untuk menyimpan data game baru

    // Membaca setiap baris dari file hingga akhir file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        newGame = (NodeGames *)malloc(sizeof(NodeGames)); // Alokasikan memori untuk game baru
        if (!newGame) {
            fprintf(stderr, "Memory allocation failed for new game\n");
            continue;
        }

        // Baca gamerTag dan data game dari buffer
        if (sscanf(buffer, "GamerTag: %99[^;]; Game: %99[^,], %99[^,], %999[^,], %99[^,], %f, %f",
            gamerTag, newGame->title, newGame->genre, newGame->desc, newGame->publisher, &newGame->rating, &newGame->price) == 7) {
            // Cari pengguna yang sesuai dengan gamerTag
            for (int i = 0; i < numPlayers; i++) {
                if (strcmp(players[i]->gamerTag, gamerTag) == 0) {
                     // Tambahkan game baru ke daftar game pengguna
                    newGame->next = players[i]->Games;
                    players[i]->Games = newGame;
                    break;
                }
            }
        } else {
            // Jika gagal mem-parsing data game, tampilkan pesan kesalahan
            fprintf(stderr, "Failed to parse game data: %s\n", buffer);
            getch();
            free(newGame); // Bebaskan memori jika parsing gagal
        }
    }

    fclose(file); // Tutup file setelah selesai membaca
}

void readShopFromFileForUsers(NodeGames **Shop) {
    // Buka file Shop.txt untuk membaca data game toko
    FILE *file = fopen("Shop.txt", "r");
    if (!file) {
        fprintf(stderr, "Failed to open file\n");
        getch();
        return;
    }

    char buffer[1024]; // Buffer untuk membaca baris dari file
    NodeGames **nodePtr = Shop; // Pointer untuk menambahkan node ke daftar

    // Membaca setiap baris dari file hingga akhir file
    while (fgets(buffer, sizeof(buffer), file)) {
        char Title[50], Genre[25], Desc[100], Publisher[50], PriceStr[10];
        float Rating, Price;
        // Baca data game dari buffer
        if (sscanf(buffer, "Game: %49[^,], %24[^,], %99[^,], %49[^,], %f, %9s",
            Title, Genre, Desc, Publisher, &Rating, PriceStr) == 6) {
            // Periksa apakah harga adalah "Free"
            if (strcmp(PriceStr, "Free") == 0) {
                Price = 0.0;
            } else {
                Price = atof(PriceStr); // Konversi string harga ke float
            }
             // Buat node baru untuk game
            NodeGames *newNode = createNode(Title, Genre, Desc, Publisher, Rating, Price);
            if (newNode == NULL) continue; // Jika alokasi memori gagal, lanjutkan ke iterasi berikutnya
            *nodePtr = newNode; // Tambahkan node baru ke daftar
            nodePtr = &newNode->next; // Pindahkan pointer ke node berikutnya
        } else {
            // Jika gagal mem-parsing data game, tampilkan pesan kesalahan
            printf("Failed to parse line: %s\n", buffer);
            getch();
        }
    }
    fclose(file); // Tutup file setelah selesai membaca
}

NodeGames* createNode(char *Title, char *Genre, char *Desc, char *Publisher, int Rating, int Price) {
     // Alokasikan memori untuk node baru
    NodeGames* newNode = (NodeGames*)malloc(sizeof(NodeGames));
    if (newNode == NULL) {
        fprintf(stderr, "Error allocating memory\\n");
        getch();
        return NULL; // Kembalikan NULL jika alokasi memori gagal
    }
    // Salin informasi game ke node baru
    strcpy(newNode->title, Title);
    strcpy(newNode->genre, Genre);
    strcpy(newNode->desc, Desc);
    strcpy(newNode->publisher, Publisher);
    newNode->rating = Rating;
    newNode->price = Price;
    return newNode; // Kembalikan pointer ke node baru
}