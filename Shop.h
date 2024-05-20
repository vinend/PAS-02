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
void AddShop(NodeGames *Shop);
void menuShopGames(user **player[], NodeGames *Shop, int Data);
void addGameToLibrary(user **player[], NodeGames *selectedGame, int loginKey);
void appendGameToFile(user *player, NodeGames *selectedGame);
void sortShop(NodeGames **Shop);
void searchingGame(NodeGames *shop);

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

void AddShop(NodeGames *Shop){
    //Membuka file games.txt untuk dibaca
    FILE *file = fopen("games.txt", "r");
    if (!file) {
        perror("Failed to open the file");
        getch();
        return; // Return jika gagal membuka file
    }

    // Temukan node terakhir dalam daftar game toko
    NodeGames* ptr = Shop;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr = ptr->next;

    // Memasuki deskripsi game kita
    printf("Masukkan Judul dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->title);
    printf("%s", ptr->title);
    printf("Masukkan Genre dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->genre);
    printf("Masukkan Deskripsi dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->desc);
    printf("Masukkan Publisher dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->publisher);
    printf("Masukkan Rating dari Game yang ingin dimasukkan : ");
    scanf("%d", ptr->rating);
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
        printf(" |  2  | Sort Games                                |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  3  | Search Games                              |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  4  | Redeem Game                               |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  5  | Return to Menu                            |\n");
        printf(" +-----+-------------------------------------------+\n");

        // Pilih opsi
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch (pilihan) {
            // Pilihan shop games
            case 1:
                DataShop(player, Shop, loginkey);
                break;
            
            // Pilihan untuk sorting games
            case 2:
                sortShop(&Shop);
                break;
            
            // Pilihan untuk searching game
            case 3:
                searchingGame(Shop);
                break;

            // Pilihan untuk meredeem game
            case 4 :
                redeemGame(player, Shop, loginkey);

            // Pilihan untuk keluar dari menu
            case 5:
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

// Function untuk sorting pada shop
void sortShop(NodeGames **Shop) {
    system("cls");
    
    // Pendeklarasian variabel
    int pilihan, pilihanSort;

    // Tampilan pada menu sorting game shop
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

    // Falling down pada switch case karena function yang digunakan sama
    switch(pilihan) {
        // Pilihan sorting rating
        case 1:

        // Pilihan sorting harga
        case 2:
        
        // Pilihan sorting nama
        case 3:

        // Pilihan sorting genre
        case 4:

        // Pilihan sorting publisher
        case 5:
            MergeSort(Shop, pilihan, pilihanSort);
            break;

        // Error handling jika pilihan tidak sesuai
        default:
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
    }
}

// Function untuk searching Game pada shop
void searchingGame(NodeGames *shop) {
    // Deklarasi variabel
    system("cls");
    int pilihan;
    char Searching[100];

    // Tampilan menu Searching shop
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
    printf("Pilih Opsi: "); scanf("%d", &pilihan); 

    // Pilihan opsi
    switch(pilihan) {
        // Pilihan searching title
        case 1: 
            system("cls");
            printf("Masukkan Title yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;

        // Pilihan searching genre
        case 2: 
            system("cls");
            printf("Masukkan Genre yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;

        // Pilihan searching publisher
        case 3: 
            system("cls");
            printf("Masukkan Publisher yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;

        // Error handling jika input diluar pilihan
        default:
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
    }
}