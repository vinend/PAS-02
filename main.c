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

// Struct untuk menyimpan informasi tentang video game
struct videoGames{
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    char gameKey[21];
    struct videoGames* next; // Pointer ke game berikutnya dalam daftar

};

// Struct untuk menyimpan informasi tentang game toko (mirip dengan videoGames)
struct shopGames {
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    char gameKey[21];
    struct shopGames* next; // Pointer ke game berikutnya dalam daftar toko
};


typedef struct User user; // Alias untuk struct User
typedef struct videoGames NodeGames; // Alias untuk struct videoGames
typedef NodeGames *GamesPtr; // Alias untuk pointer ke struct videoGames

// Struct untuk menyimpan informasi tentang pengguna
struct User{
    char gamerTag[100];
    char password[100];
    GamesPtr Games; 
};

// Deklarasi fungsi-fungsi yang digunakan
void appendUser(const char username[], const char password[]);
void libraryMenu(user **player[], int loginKey, NodeGames *Shop);
GamesPtr swap(GamesPtr ptr1, GamesPtr ptr2);
void string_to_lower(char *str);
void pilihSort(user **player[], int jumlahData, int loginKey);
void pilihSearch(user **player[], int jumlahData, int loginKey);
void sortingRating(user **player, int jumlahData);
void sortingHarga(user **player, int jumlahData);
void searchingString(user **player[], char* namaDicari, int i, int loginKey);
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef);
GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan, int PilihanSort);
void MergeSort(GamesPtr* headRef, int pilihan, int PilihanSort);
void readPlayersFromFile(user **players, int *numPlayers);
void readGamesFromFileForUsers(user **players, int numPlayers);
void readShopFromFileForUsers(NodeGames **Shop);
void lihatData(user **player[], int loginKey);
void userSettings(user **player[], int loginKey);
void flushInput();
void randomPassGen(char *pass, int length);
void createUser(user **playerLogin, int *numPlayerLogin);
void loginUser(user **player, int numPlayer, NodeGames *Shop);
void loginPageMenu(user **player, NodeGames *Shop, int numPlayers);
void menuShopGames(user **player[], NodeGames *Shop, int Data);
void updatePasswordInFile(const char *gamerTag, const char *newPassword);
void generateGameKey(char *key, int length);
void addGameToLibrary(user **player[], NodeGames *selectedGame, int loginKey);
void appendGameToFile(user *player, NodeGames *selectedGame);
void sortShop(NodeGames **Shop);\
void searchingGame(NodeGames *shop);
void searchingStringShop(NodeGames *Shop, char namaDicari[], int i);


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
        // Baca gamerTag dari buffer
        if (sscanf(buffer, "GamerTag: %99[^\n]", newUser->gamerTag) == 1) {
            // Baca password dari baris berikutnya dalam file
            if (fgets(buffer, sizeof(buffer), file) && sscanf(buffer, "Password: %99[^\n]", newUser->password) == 1) {
                newUser->Games = NULL; // Inisialisasi pointer ke game sebagai NULL
                players[index++] = newUser;  // Tambahkan pengguna baru ke array pemain
            } else {
                free(newUser); // Bebaskan memori jika membaca password gagal
            }
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
    printf("Masukkan Genre dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->genre);
    printf("Masukkan Deskripsi dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->desc);
    printf("Masukkan Publisher dari Game yang ingin dimasukkan : ");
    scanf(" %[^\n]", ptr->publisher);
    printf("Masukkan Rating dari Game yang ingin dimasukkan : ");
    scanf("%d", ptr->rating);

    

}

void searchingGame(NodeGames *shop) {
    system("cls");
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
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
        case 1: 
            system("cls");
            printf("Masukkan Title yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;
        case 2: 
            system("cls");
            printf("Masukkan Genre yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;
        case 3: 
            system("cls");
            printf("Masukkan Publisher yang ingin di search: ");
            scanf(" %[^\n]", Searching);
            searchingStringShop(shop, Searching, pilihan);
            break;
        default:
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
    }
}


void searchingStringShop(NodeGames *Shop, char namaDicari[], int i) {
    int j = 0;
    int found = 0;
    string_to_lower(namaDicari);

    NodeGames *current = Shop;

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    while (current != NULL) {
        char currentNameLower[100];
        if (i == 1) {
            strcpy(currentNameLower, current->title);
        } else if (i == 2) {
            strcpy(currentNameLower, current->genre);
        } else if (i == 3) {
            strcpy(currentNameLower, current->publisher);
        }
        string_to_lower(currentNameLower);

        if (strstr(currentNameLower, namaDicari) != NULL) {
            printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", j+1, current->title, current->publisher, current->genre, current->price, current->rating);
            found = 1;
        }
        current = current->next;
        j++;
    }

    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    if (!found) {
        system("cls");
        printf("Tidak ada data yang sesuai.\n");
        getch();
    }

    getch();
    system("cls");
}

void redeemGame(user **player[], NodeGames *Shop, int loginKey) {
    int j = 0;
    int found = 0;
    char gameKey[50];

    NodeGames *current = Shop;

    printf("Enter the Game Key to Redeem: "); scanf("%s", &gameKey);
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

    if (!found) {
        system("cls");
        printf("Redeem Key Not Valid!\n");
        getch();
    }

    getch();
    system("cls");
}



void lihatData(user **player[], int loginKey) {
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

    printf("Data for %s:\n", (*player)[loginKey]->gamerTag);
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    while (current != NULL) {
        printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
        i++;
        current = current->next;
    }
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");
    getch();
    printf("Press Enter to continue...\n");
    fflush(stdout); // Ensure all output is printed before blocking for input
    while(getchar() != '\n'); // Wait for Enter key to be pressed
}

void sortShop(NodeGames **Shop) {
    system("cls");
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
    printf("Pilih Opsi: "); scanf("%d", &pilihan);
    printf("Mau Naik Atau Turun (1 = Naik, 2 = Turun) : "); scanf("%d", &pilihanSort);

    switch(pilihan) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            MergeSort(Shop, pilihan, pilihanSort);
            break;
        default:
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
    }
}


void libraryMenu(user **player[], int loginKey, NodeGames *Shop){
    int trigger = 0;
    int i = 0, pilihan, O = 0;

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    NodeGames *currentGame = (*player)[loginKey]->Games;
    while (currentGame != NULL) {
        printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n",
            i+1, currentGame->title, currentGame->publisher, currentGame->genre, currentGame->price, currentGame->rating);
        currentGame = currentGame->next;
        i++;
    }
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");
    getch();
    
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

        switch(pilihan) {
            
            case 1 :
            lihatData(player, loginKey);
            break;
            
            case 2 : 
            pilihSort(player, i, loginKey);
            break;

            case 3 : 
            pilihSearch(player, i, loginKey);
            break;

            case 4 : 
            menuShopGames(player, Shop, loginKey);
            break;

            case 5 :
            AddShop(Shop);
            break;

            case 6 : 
            userSettings(player, loginKey);
            break;

            case 7 :
            trigger = 1;
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
    } while(trigger == 0);
}

GamesPtr swap(GamesPtr ptr1, GamesPtr ptr2) {

    GamesPtr tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;

    return ptr2;
}

void string_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void pilihSort(user **player[], int jumlahData, int loginKey){
    system("cls");
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
    printf("Pilih Opsi: "); scanf("%d", &pilihan);
    printf("Mau Naik Atau Turun (1 = Naik, 2 = Turun) : "); scanf("%d", &pilihanSort);

    switch(pilihan) {
            case 1 : 

            case 2 : 

            case 3 : 

            case 4 : 

            case 5 : 
            MergeSort(&(*player)[loginKey]->Games, pilihan, pilihanSort);
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}

void pilihSearch(user **player[], int jumlahData, int loginKey){
    system("cls");
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
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
            case 1 : 
            system("cls");
            printf("Masukkan Title yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            case 2 : 
            system("cls");
            printf("Masukkan Genre yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            case 3 : 
            system("cls");
            printf("Masukkan Publisher yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan, loginKey);
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}

void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef) {
    GamesPtr fast;
    GamesPtr slow;
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    slow = source;
    fast = source->next;

    #pragma omp parallel shared(fast)
    {
        while (fast != NULL) {
            #pragma omp single nowait
            {
                fast = fast->next;
                if (fast != NULL) {
                    slow = slow->next;
                    fast = fast->next;
                }
            }
        }
    }
    
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}


GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan, int PilihanSort) {
    GamesPtr result = NULL;
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (pilihan == 1) {
        if (PilihanSort == 1){
            if (a->rating <= b->rating) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        else if (PilihanSort == 2){
            if (a->rating >= b->rating) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    } 
    else if (pilihan == 2) {
        if (PilihanSort == 1){
            if (a->price <= b->price) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        else if (PilihanSort == 2){
            if (a->price >= b->price) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    else if (pilihan == 3){
        if (PilihanSort == 1){
            if (strcmp(a->title, b->title) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        else if (PilihanSort == 2){
            if (strcmp(a->title, b->title) > 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    else if (pilihan == 4){
        if (PilihanSort == 1){
            if (strcmp(a->title, b->title) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        else if (PilihanSort == 2){
            if (strcmp(a->title, b->title) > 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    else if (pilihan == 5){
        if (PilihanSort == 1){
            if (strcmp(a->title, b->title) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        else if (PilihanSort == 2){
            if (strcmp(a->title, b->title) > 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    else {
        printf("Pilihan diluar yang disediakan, silahkan masukkan ulang !");
        getch();
    }
    return result;
}

void MergeSort(GamesPtr* headRef, int pilihan, int PilihanSort) {
    GamesPtr head = *headRef;
    GamesPtr a;
    GamesPtr b;
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    FrontBackSplit(head, &a, &b);  

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            MergeSort(&a, pilihan, PilihanSort);  
        }
        #pragma omp section
        {
            MergeSort(&b, pilihan, PilihanSort);
        }
    }

    *headRef = SortedMerge(a, b, pilihan, PilihanSort); 
}

void searchingString(user **player[], char* namaDicari, int i, int loginKey){
    int j = 0;
    GamesPtr current = (*player)[loginKey]->Games;
    int found = 0;
    string_to_lower(namaDicari);

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Searching data yang sesuai
    while (current != NULL) {
        char currentNameLower[100];
        if (i == 1) {
            strcpy(currentNameLower, current->title);
        }
        else if (i == 2){
            strcpy(currentNameLower, current->genre);
        }
        else if (i == 3){
            strcpy(currentNameLower, current->publisher);
        }
        string_to_lower(currentNameLower);
        if (strstr(currentNameLower, namaDicari) != NULL) {
            printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", j+1, current->title, current->publisher, current->genre, current->price, current->rating);
            found = 1;
        }
        current = current->next;
    }

    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Error handling jika tidak ada data yang sesuai
    if (!found) {
        system("cls");
        printf("Tidak ada data yang sesuai.\n");
        getch();
    }

    getch(); system("cls");
}


void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void randomPassGen(char *pass, int length) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));
    #pragma omp parallel for
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        pass[i] = charset[key];
    }
    pass[length] = '\0';
}

void createUser(user **playerLogin, int *numPlayersLogin) {
    char bufferUsername[100];
    char bufferPassword[100];
    user *newUser = (user*)malloc(sizeof(user));
    if (newUser == NULL) {
        printf("Memory allocation failed.\n");
        getch();
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
        system("cls");
        printf("Suggest Password? (Yes/No): ");
        scanf("%9s", buffer);
        flushInput();

        if (strcmp(buffer, "Yes") == 0) {
            #pragma omp parallel
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
            getch();
        }
    } while (trigger == 0);

    strcpy(bufferUsername, newUser->gamerTag);
    strcpy(bufferPassword, newUser->password);

    appendUser(bufferUsername, bufferPassword);
    
    system("cls");
    
    newUser->Games = NULL;
    *playerLogin = newUser;
    *numPlayersLogin = *numPlayersLogin + 1; 
}

void appendUser(const char username[], const char password[]) {
    FILE *file = fopen("players.txt", "a");
    if (file == NULL) {
        perror("Failed to open file for appending");
        getch();
        return;
    }

    fprintf(file, "\nGamerTag: %s\n", username);
    fprintf(file, "Password: %s", password);

    fclose(file);
}

void loginUser(user **player, int numPlayer, NodeGames *Shop) {
    int trigger = 0, found = 0, loginKey = -1;

    system("cls");
    printf("||         USER LOGIN PAGE         ||\n");

    while (trigger < 3) {
        system("cls");
        char bufferUsername[100], bufferPassword[100];
        printf("Enter Username: ");
        scanf(" %99s", bufferUsername);
        printf("Enter Password: ");
        scanf(" %99s", bufferPassword);

        for (int i = 0; i < numPlayer; i++) {
            if (strcmp(bufferUsername, player[i]->gamerTag) == 0 && strcmp(bufferPassword, player[i]->password) == 0) {
                printf("Login Successful!\n");
                found = 1;
                loginKey = i;
                break;
            }
        }

        if (found) {
            libraryMenu(&player, loginKey, Shop);
            return; 
        } else {
            printf("We couldn't find your username / The password is wrong. You have %d attempts left.\n", 2 - trigger);
            getch(); 
            trigger++;
            if (trigger == 3) {
                printf("Maximum login attempts exceeded.\n");
                getch(); 
            }
        }
    }

    if (!found) {
        printf("Returning to the main menu...\n");
        getch(); 
        system("cls");
    }
}


void loginPageMenu(user **player, NodeGames *Shop, int numPlayers) {
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
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch(pilihan) {
            case 1: 
                loginUser(player, numPlayersLogin, Shop);
                break;

            case 2: 
                createUser(player, &numPlayersLogin);
                break;

            case 3: 
                trigger = 1;
                break;

            default: 
                printf("Please input the correct value!");
                getch();
                break;
        }
    } while (trigger == 0);
}

void userSettings(user **player[], int loginKey) {
    int trigger = 0, found = 0;
    char bufferPassword[100], newPassword[100];

    system("cls");
    printf("||         Please Enter Your Password Again         ||\n");

    while (trigger < 3 && !found) {
        system("cls");
        printf("Enter Password: ");
        scanf(" %99s", bufferPassword);

        if (strcmp(bufferPassword, (*player)[loginKey]->password) == 0) {
            printf("Login Successful!\n");
            found = 1;
            break;
        } else {
            printf("The password is wrong. You have %d attempts left.\n", 2 - trigger);
            getch();
            trigger++;
        }

        if (trigger == 3) {
            printf("Maximum login attempts exceeded.\n");
            getch();
            return;
        }
    }

    if (found) {
        printf("Enter new password: ");
        scanf(" %99s", newPassword);
        strcpy((*player)[loginKey]->password, newPassword);
        printf("Password updated successfully!\n");

        updatePasswordInFile((*player)[loginKey]->gamerTag, newPassword);
    }
}

void updatePasswordInFile(const char *gamerTag, const char *newPassword) {
    FILE *file = fopen("players.txt", "r+");
    if (!file) {
        perror("Failed to open the file");
        return;
    }

    char buffer[256];
    long offset = 0;
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "GamerTag:") != NULL && strstr(buffer, gamerTag) != NULL) {
            found = 1;
            fgets(buffer, sizeof(buffer), file);  // Read the current password line
            offset = ftell(file) - strlen(buffer);  // Calculate the start position of the current password line
            break;
        }
    }

    if (found) {
        fseek(file, offset, SEEK_SET);  // Set file pointer to the beginning of the password line

        int currentPassLength = strlen(strchr(buffer, ':') + 2) - 1;  // Calculate the length of the existing password (+2 for skipping ": ")
        int newPassLength = strlen(newPassword);

        // Construct the new password line with the correct format
        char newPassLine[256];
        int written = snprintf(newPassLine, sizeof(newPassLine), "Password: %s\n", newPassword);

        if (written > currentPassLength) {  // If the new password is longer, truncate it
            newPassLine[currentPassLength + 10] = '\n';  // +10 to account for "Password: "
            newPassLine[currentPassLength + 11] = '\0';
        } else if (written < currentPassLength) {  // If the new password is shorter, pad with spaces
            memset(newPassLine + written - 1, ' ', currentPassLength - written + 1);  // Fill space with spaces
            newPassLine[currentPassLength + 10] = '\n';
            newPassLine[currentPassLength + 11] = '\0';
        }

        // Write the new password line, padded correctly to fit the original space
        fputs(newPassLine, file);
    } else {
        printf("GamerTag not found.\n");
        getch();
    }

    fclose(file);
}



void menuShopGames(user **player[], NodeGames *Shop, int loginkey) {
    int pilihan, trigger = 0;

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
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                DataShop(player, Shop, loginkey);
                break;
            case 2:
                sortShop(&Shop);
                break;
            case 3:
                searchingGame(Shop);
                break;

            case 4 :
                redeemGame(player, Shop, loginkey);

            case 5:
                trigger = 1;
                break;
            default:
                printf("Invalid option. Please try again.\n");
                getch();
                break;
        }
    } while (trigger == 0);
}

int main() {
    user **players;
    NodeGames *Shop;
    int numPlayers = 100;  
    int pilihan, trigger = 0;

    srand(time(NULL));

    Shop = (NodeGames *)malloc(sizeof(NodeGames *));
    players = (user **)malloc(numPlayers * sizeof(user *));
    if (players == NULL) {
        fprintf(stderr, "Failed to allocate memory for players.\n");
        getch();
        return EXIT_FAILURE;
    }

    readPlayersFromFile(players, &numPlayers);
    readGamesFromFileForUsers(players, numPlayers);
    readShopFromFileForUsers(&Shop);
    generateGameKeys(Shop);
    appendGameKeysToFile(Shop);
    
    getch();

    loginPageMenu(players, Shop, numPlayers);
    return 0;
}
