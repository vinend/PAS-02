#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "struct.h"

void TampilkanData(user **player, int loginKey);
GamesPtr swap( GamesPtr ptr1, GamesPtr ptr2);
void string_to_lower(char *str);
void pilihSort(user **player, int jumlahData);
void pilihSearch(user **player, int jumlahData);
void sortingRating(user **player, int jumlahData);
void sortingHarga(user **player, int jumlahData);
void searchingString(user **player, char* namaDicari, int i);
void MergeSort(user **player, int pilihan);
GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan);
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef);

void readFromFile(user **players, int *numPlayers) {
    FILE *file = fopen("players.txt", "r");
    if (!file) {
        printf("Failed to open the file.\n");
        return;
    }

    char buffer[1024];
    int index = 0;
    user *newUser = NULL;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp("GamerTag:", buffer, 9) == 0) {
            newUser = (user *)malloc(sizeof(user));
            sscanf(buffer, "GamerTag: %99[^\n]", newUser->gamerTag);

            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "Password: %99[^\n]", newUser->password);

            newUser->Games = NULL;
            players[index++] = newUser;
        } else if (strncmp("Game:", buffer, 5) == 0) {
            NodeGames *newGame = (NodeGames *)malloc(sizeof(NodeGames));
            sscanf(buffer, "Game: %99[^,], %99[^,], %999[^,], %99[^,], %f, %f", 
                newGame->title, newGame->genre, newGame->desc, newGame->publisher, &newGame->rating, &newGame->price);
            
            newGame->next = newUser->Games;
            newUser->Games = newGame;
        }
    }

    fclose(file);
    *numPlayers = index;
}

void TampilkanData(user **player, int loginKey){
    int i = 0, pilihan, trigger;

    printf("Data:\n");
    printf("_________________________________________________________________________________________________\n");
    printf("|No\tTitle Game\t\tPublisher\tGenre\tHarga\tRating                               \n");
    printf("|_______________________________________________________________________________________________|\n");

    while ((*player)->Games->next != NULL)
    {
        printf("|%-2d\t%-20s\t%11s\t%9s\t%9d\t%11d\n", i+1, (*player)->Games->title, (*player)->Games->publisher, (*player)->Games->genre, (*player)->Games->price, (*player)->Games->rating);
    }
    printf("|_______________________________________________________________________________________________|\n");

    do {
        system("cls");
        printf(" +-------------------------------------------------+\n");
        printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
        printf(" +-------------------------------------------------+\n");
        printf(" | No. |              OPSI                         |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  1  | Sorting Data                              |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf(" |  2  | Searching Data                            |\n");
        printf(" +-----+-------------------------------------------+\n");
        printf("Pilih Opsi: "); scanf("%d", &pilihan);

        switch(pilihan) {
            case 1 : 
            pilihSort(player, i);
            break;

            case 2 : 
            pilihSearch(player, i);
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
    } while(trigger == 0);
}

GamesPtr swap( GamesPtr ptr1, GamesPtr ptr2) {

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
    int pilihan;
    printf(" +-------------------------------------------------+\n");
    printf(" |          SELAMAT DATANG DI GAME LIBRARY         |\n");
    printf(" +-------------------------------------------------+\n");
    printf(" | No. |              OPSI                         |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  1  | Sorting Rating                            |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf(" |  2  | Sorting Harga                             |\n");
    printf(" +-----+-------------------------------------------+\n");
    printf("Pilih Opsi: "); scanf("%d", &pilihan);

    switch(pilihan) {
            case 1 : 
            MergeSort(player, pilihan);
            break;

            case 2 : 
            MergeSort(player, pilihan);
            break;

            default :
            system("cls"); 
            printf("Input Tidak Benar"); 
            getch();
            break;
        }
}

void pilihSearch(user **player, int jumlahData){
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

void MergeSort(user **player, int pilihan)
{
    GamesPtr head = player;
    GamesPtr a;
    GamesPtr b;

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a, pilihan);
    MergeSort(&b, pilihan);

    /* answer = merge the two sorted lists together */
    player = SortedMerge(a, b, pilihan);
}

GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan)
{
    GamesPtr result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);
    if (pilihan == 2){
        /* Pick either a or b, and recur */
        if (a->price <= b->price) {
            result = a;
            result->next = SortedMerge(a->next, b, pilihan);
        }
        else {
            result = b;
            result->next = SortedMerge(a, b->next, pilihan);
        }
        return (result);
    }
    else if (pilihan == 1){
        /* Pick either a or b, and recur */
        if (a->rating <= b->rating) {
            result = a;
            result->next = SortedMerge(a->next, b, pilihan);
        }
        else {
            result = b;
            result->next = SortedMerge(a, b->next, pilihan);
        }
        return (result);
    }
    else{

    }
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    Uses the fast/slow pointer strategy. */
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef)
{
    GamesPtr fast;
    GamesPtr slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void searchingString(user **player, char* namaDicari, int i){
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
            printf("|%-2d\t%-20s\t%11s\t%9s\t%9d\t%11d\n", i+1, (*player)->Games->title, (*player)->Games->publisher, (*player)->Games->genre, (*player)->Games->price, (*player)->Games->rating);
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
