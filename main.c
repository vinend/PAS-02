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
#include <ctype.h>
#include "Struct.h"
#include "ReadFile.h"
#include "User.h"

// Function main
int main() {
    // Pendeklarasian variabel
    user **players;
    NodeGames *Shop;
    int numPlayers = 100;  
    int pilihan, trigger = 0;

    srand(time(NULL));

    // Pengalokasian memori dinamis untuk shop dan players
    Shop = (NodeGames *)malloc(sizeof(NodeGames *));
    players = (user **)malloc(numPlayers * sizeof(user *));

    // Jika gagal mengalokasikan memori untuk players
    if (players == NULL) {
        fprintf(stderr, "Failed to allocate memory for players.\n");
        getch();
        return EXIT_FAILURE;
    }

    // Jika gagal mengalokasikan memori untuk shop
    if (Shop == NULL) {
        fprintf(stderr, "Failed to allocate memory for shop.\n");
        getch();
        return EXIT_FAILURE;
    }

    // Membaca data untuk players, games, dan shop dari file
    readPlayersFromFile(players, &numPlayers);
    readGamesFromFileForUsers(players, numPlayers);
    readShopFromFileForUsers(&Shop);

    // Mengenerate gamekeys dan mengappend ke dalam file
    generateGameKeys(Shop);
    appendGameKeysToFile(Shop);
    
    getch();

    // Pergi ke login page menu
    loginPageMenu(players, Shop, numPlayers);
    return 0;
}
