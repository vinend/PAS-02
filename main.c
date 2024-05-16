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

struct videoGames{
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    int gameKey;
    struct videoGames* next;

};

struct shopGames {
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    struct shopGames* next;
};

typedef struct User user;
typedef struct videoGames NodeGames;
typedef NodeGames *GamesPtr;

struct User{
    char gamerTag[100];
    char password[100];
    GamesPtr Games; 
};


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

void generateGameKey(char *key, int length) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        int index = rand() % (sizeof(charset) - 1);
        key[i] = charset[index];
    }
    key[length] = '\0';
}


void readPlayersFromFile(user **players, int *numPlayers) {
    FILE *file = fopen("players.txt", "r");
    if (!file) {
        printf("Failed to open the file.\n");
        return;
    }

    char buffer[1024];
    int index = 0;
    user *newUser;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        newUser = (user *)malloc(sizeof(user));
        if (sscanf(buffer, "GamerTag: %99[^\n]", newUser->gamerTag) == 1) {
            if (fgets(buffer, sizeof(buffer), file) && sscanf(buffer, "Password: %99[^\n]", newUser->password) == 1) {
                newUser->Games = NULL;
                players[index++] = newUser;
            } else {
                free(newUser);
            }
        }
    }

    fclose(file);
    *numPlayers = index;
}

void readGamesFromFileForUsers(user **players, int numPlayers) {
    FILE *file = fopen("games.txt", "r");
    if (!file) {
        perror("Failed to open the file");
        return;
    }

    char buffer[1024];
    char gamerTag[100];
    NodeGames *newGame;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        newGame = (NodeGames *)malloc(sizeof(NodeGames));
        if (!newGame) {
            fprintf(stderr, "Memory allocation failed for new game\n");
            continue;
        }

        if (sscanf(buffer, "GamerTag: %99[^;]; Game: %99[^,], %99[^,], %999[^,], %99[^,], %f, %f",
            gamerTag, newGame->title, newGame->genre, newGame->desc, newGame->publisher, &newGame->rating, &newGame->price) == 7) {
            for (int i = 0; i < numPlayers; i++) {
                if (strcmp(players[i]->gamerTag, gamerTag) == 0) {
                    newGame->next = players[i]->Games;
                    players[i]->Games = newGame;
                    break;
                }
            }
        } else {
            fprintf(stderr, "Failed to parse game data: %s\n", buffer);
            free(newGame);
        }
    }

    fclose(file);
}

NodeGames* createNode(char *Title, char *Genre, char *Desc, char *Publisher, int Rating, int Price) {
    NodeGames* newNode = (NodeGames*)malloc(sizeof(NodeGames));
    if (newNode == NULL) {
        fprintf(stderr, "Error allocating memory\\n");
        return NULL;
    }
    strcpy(newNode->title, Title);
    strcpy(newNode->genre, Genre);
    strcpy(newNode->desc, Desc);
    strcpy(newNode->publisher, Publisher);
    newNode->rating = Rating;
    newNode->price = Price;
    return newNode;
}

void readShopFromFileForUsers(NodeGames **Shop) {
    FILE *file = fopen("Shop.txt", "r");
    if (!file) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    char buffer[1024]; // Increased buffer size to handle larger lines
    NodeGames **nodePtr = Shop;

    while (fgets(buffer, sizeof(buffer), file)) {
        char Title[50], Genre[25], Desc[100], Publisher[50], PriceStr[10];
        float Rating, Price;
        if (sscanf(buffer, "Game: %49[^,], %24[^,], %99[^,], %49[^,], %f, %9s",
            Title, Genre, Desc, Publisher, &Rating, PriceStr) == 6) {
            if (strcmp(PriceStr, "Free") == 0) {
                Price = 0.0;
            } else {
                Price = atof(PriceStr); // Convert string to float
            }
            NodeGames *newNode = createNode(Title, Genre, Desc, Publisher, Rating, Price);
            if (newNode == NULL) continue; // Handle memory allocation failure
            *nodePtr = newNode;
            nodePtr = &newNode->next;
        } else {
            printf("Failed to parse line: %s\n", buffer);
        }
    }
    fclose(file);
}

void DataShop(NodeGames *Shop) {
    if (Shop == NULL || Shop == NULL) {
        printf("Error: Player data is not available.\n");
        return;
    }
    if (Shop == NULL) {
        printf("No games in your library");
        return;
    }

    int i = 0;
    NodeGames *current = Shop;

    printf("Games List : \n");
    printf("_________________________________________________________________________________________________________________________________\n");
    printf("|No   |Title Game                                    |Publisher                           |Genre          |Price     |Rating    |\n");
    printf("|_____|______________________________________________|____________________________________|_______________|__________|__________|\n");

    while (current != NULL) {
        printf("|%-4d |%-45s |%-30s |%-20s|%-10.2f|%-10.2f|\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
        i++;
        current = current->next;
    }
    printf("|_____|______________________________________________|____________________________________|_______________|__________|__________|\n");
    getch();
    printf("Press Enter to continue...\n");
    fflush(stdout); // Ensure all output is printed before blocking for input
    while(getchar() != '\n'); // Wait for Enter key to be pressed
}

void lihatData(user **player[], int loginKey) {
    if (player == NULL || *player == NULL) {
        printf("Error: Player data is not available.\n");
        return;
    }
    if ((*player)[loginKey]->Games == NULL) {
        printf("No games in your library for %s.\n", (*player)[loginKey]->gamerTag);
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
            menuShopGames(player, Shop, i);
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
        return;
    }

    fprintf(file, "GamerTag: %s\n", username);
    fprintf(file, "Password: %s\n", password);

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
            getch(); // Wait for user to press a key
            trigger++;
            if (trigger == 3) {
                printf("Maximum login attempts exceeded.\n");
                getch(); // Wait for user to press a key
            }
        }
    }

    // Return to the calling function (loginPageMenu) if login attempts are exhausted
    if (!found) {
        printf("Returning to the main menu...\n");
        getch(); // Wait for user input before clearing the screen
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
            offset = ftell(file);
            break;
        }
    }

    if (found) {
        // Move the file pointer to the position after the GamerTag line
        fseek(file, offset, SEEK_SET);
        // Skip the current password line
        fgets(buffer, sizeof(buffer), file);
        // Move the file pointer back to the beginning of the password line
        fseek(file, -strlen(buffer), SEEK_CUR);
        // Write the new password
        fprintf(file, "Password: %s\n", newPassword);
    } else {
        printf("GamerTag not found.\n");
    }

    fclose(file);
}



void menuShopGames(user **player[], NodeGames *Shop, int Data) {
    
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
    printf(" |  2  |                                           |\n");
    printf(" +-----+-- -----------------------------------------+\n");
    printf(" |  3  | Exit Program                              |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
        case 1 :
            DataShop(Shop);
            break;
        case 2 :
            break;
        case 3 :
            break;
        default :
            break;

    }
    } while(trigger == 0);
}

int main() {
    user **players;
    NodeGames *Shop;
    int numPlayers = 100;  
    int pilihan, trigger = 0;

    Shop = (NodeGames *)malloc(sizeof(NodeGames *));
    players = (user **)malloc(numPlayers * sizeof(user *));
    if (players == NULL) {
        fprintf(stderr, "Failed to allocate memory for players.\n");
        return EXIT_FAILURE;
    }

    readPlayersFromFile(players, &numPlayers);
    readGamesFromFileForUsers(players, numPlayers);
    readShopFromFileForUsers(&Shop);

    getch();

    loginPageMenu(players, Shop, numPlayers);
    return 0;
}
