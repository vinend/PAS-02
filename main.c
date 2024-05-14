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

typedef struct User user;
typedef struct videoGames NodeGames;
typedef NodeGames *GamesPtr;

struct User{
    char gamerTag[100];
    char password[100];
    GamesPtr Games; 
};



void libraryMenu(user **player[], int loginKey);
GamesPtr swap( GamesPtr ptr1, GamesPtr ptr2);
void string_to_lower(char *str);
void pilihSort(user **player, int jumlahData);
void pilihSearch(user **player, int jumlahData);
void sortingRating(user **player, int jumlahData);
void sortingHarga(user **player, int jumlahData);
void searchingString(user **player, char* namaDicari, int i);
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef);
GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan, int PilihanSort);
void MergeSort(GamesPtr* headRef, int pilihan, int PilihanSort) ;

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

void readGamesFromFileForUser(user *player) {
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
            if (strcmp(player->gamerTag, gamerTag) == 0) {
                newGame->next = player->Games;
                player->Games = newGame;
            } else {
                free(newGame);
            }
        } else {
            fprintf(stderr, "Failed to parse game data: %s\n", buffer);
            free(newGame);
        }
    }

    fclose(file);
}

void lihatData(user **player, int loginKey) {
    int i = 0;

    NodeGames *current = (*player)->Games;

    printf("Data:\n");
    printf("_________________________________________________________________________________________________\n");
    printf("|No\tTitle Game\t\tPublisher\tGenre\tHarga\tRating                               \n");
    printf("|_______________________________________________________________________________________________|\n");

    while (current != NULL) {
        printf("|%-2d\t%-20s\t%11s\t%9s\t%9.2f\t%11.2f\n", i+1, current->title, current->publisher, current->genre, current->price, current->rating);
        i++;
        current = current->next;
        }
    printf("|_______________________________________________________________________________________________|\n");
    getch();
}

void libraryMenu(user **player[], int loginKey){
    int trigger = 0;
    int i = 0, pilihan, O = 0;

    readGamesFromFileForUser((*player)[loginKey]);

    printf("Data:\n");
    printf("_________________________________________________________________________________________________\n");
    printf("|No\tTitle Game\t\tPublisher\tGenre\tHarga\tRating                               \n");
    printf("|_______________________________________________________________________________________________|\n");

    NodeGames *currentGame = (*player)[loginKey]->Games;
    while (currentGame != NULL) {
        printf("|%-2d\t%-20s\t%11s\t%9s\t%9.2f\t%9.2f\n", i+1, currentGame->title, currentGame->publisher, currentGame->genre, currentGame->price, currentGame->rating);
        currentGame = currentGame->next;
        i++;
    }
    printf("|_______________________________________________________________________________________________|\n");
    getch();
    
    do {
        int i = 0, pilihan, O = 0;
        NodeGames *currentGame = (*player)[loginKey]->Games;
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
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch(pilihan) {
            
            case 1 :
            lihatData(player[loginKey], i);
            break;
            
            case 2 : 
            pilihSort(player[loginKey], i);
            break;

            case 3 : 
            pilihSearch(player[loginKey], i);
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

void pilihSort(user **player, int jumlahData){
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
            MergeSort(&(*player)->Games, pilihan, pilihanSort);
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}

void pilihSearch(user **player, int jumlahData){
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
            searchingString(player, Searching, pilihan);
            break;

            case 2 : 
            system("cls");
            printf("Masukkan Genre yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan);
            break;

            case 3 : 
            system("cls");
            printf("Masukkan Publisher yang ingin di search : ");
            scanf(" %[^\n]", Searching);
            searchingString(player, Searching, pilihan);
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

void searchingString(user **player, char* namaDicari, int i){
    int j = 0;
    GamesPtr current = (*player)->Games;
    int found = 0;
    string_to_lower(namaDicari);

    printf("Data:\n");
    printf("_________________________________________________________________________________________________\n");
    printf("|No\tTitle Game\t\tPublisher\tGenre\tHarga\tRating                               \n");
    printf("|_______________________________________________________________________________________________|\n");

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
            printf("|%-2d\t%-20s\t%11s\t%9s\t%9.2f\t%11.2f\n", j+1, current->title, current->publisher, current->genre, current->price, current->rating);
            found = 1;
        }
        current = current->next;
    }

    printf("|_______________________________________________________________________________________________|\n");

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

void createUser(user **playerLogin) {
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

    system("cls");
    
    newUser->Games = NULL;
    *playerLogin = newUser; 
}

void loginUser(user **player, int numPlayer) {
    int trigger = 0, found = 0, loginKey = -1;
    char bufferUsername[100], bufferPassword[100];

    system("cls");
    printf("||         USER LOGIN PAGE         ||\n");

    while (trigger < 3 && !found) {
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
            libraryMenu(&player, loginKey);
            break; 
        } else {
            printf("We couldn't find your username / The password is wrong. You have %d attempts left.\n", 2 - trigger);
            trigger++;
            if (trigger == 3) {
                printf("Maximum login attempts exceeded.\n");
                break;
            }
        }
    }
}
void loginPageMenu(user **player) {
    getch();
    int pilihan, trigger = 0;
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
        case 1 : loginUser(player, 100);
        break;

        case 2 : createUser(player);
        break;

        case 3 : trigger = 1;
        break;

        default : printf("Please input the correct value!");
        break;
    }
    } while(trigger == 0);

}

int main() {

    user **player;
    int maxPlayer = 100;  
    int pilihan, trigger = 0;

    player = (user **)malloc(maxPlayer * sizeof(user *));
    if (player == NULL) {
        fprintf(stderr, "Failed to allocate memory for players.\n");
        return EXIT_FAILURE;
    }

    readPlayersFromFile(player, &maxPlayer);
    loginPageMenu(player);
    return 0;

}