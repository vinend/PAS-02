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

int main() {
user **player;
    int numPlayer = 0;
    int maxPlayer = 100;  
    int pilihan, trigger = 0;

    player = (user **)malloc(maxPlayer * sizeof(user *));
    if (player == NULL) {
        fprintf(stderr, "Failed to allocate memory for players.\n");
        return EXIT_FAILURE;
    }

    readFromFile(player, &numPlayer);
    loginPageMenu(&player);
    
    return 0;

}