#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include "Struct.h"

// Deklarasi fungsi-fungsi yang digunakan
GamesPtr swap(GamesPtr ptr1, GamesPtr ptr2);
void string_to_lower(char *str);
void sortingRating(user **player, int jumlahData);
void sortingHarga(user **player, int jumlahData);
void searchingString(user **player[], char* namaDicari, int i, int loginKey);
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef);
GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan, int PilihanSort);
void MergeSort(GamesPtr* headRef, int pilihan, int PilihanSort);
void sortShop(NodeGames **Shop);\
void searchingGame(NodeGames *shop);
void searchingStringShop(NodeGames *Shop, char namaDicari[], int i);

// Function untuk swap 2 pointer
GamesPtr swap(GamesPtr ptr1, GamesPtr ptr2) {

    GamesPtr tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;

    return ptr2;
}

// Function untuk tolower suatu string
void string_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function untuk front back split pada merge sort
void FrontBackSplit(GamesPtr source, GamesPtr* frontRef, GamesPtr* backRef) {
    // Pendeklarasian variabel
    GamesPtr fast;
    GamesPtr slow;

    // Jika data masih NULL
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    slow = source;
    fast = source->next;

    // Melakukan front back split dengan parallel programming
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

// Function untuk merge data yang telah di sort
GamesPtr SortedMerge(GamesPtr a, GamesPtr b, int pilihan, int PilihanSort) {
    // Pendeklarasian pointer
    GamesPtr result = NULL;

    // Jika data masih NULL
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // Jika pilihan adalah sorting bedasarkan rating
    if (pilihan == 1) {
        // Jika pilihan sorting adalah naik
        if (PilihanSort == 1){
            if (a->rating <= b->rating) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        // Jika pilihan sorting adalah turun
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
    // Jika pilihan adalah sorting bedasarkan harga
    else if (pilihan == 2) {
        // Jika pilihan sorting adalah naik
        if (PilihanSort == 1){
            if (a->price <= b->price) {
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        // Jika pilihan sorting adalah turun
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
    // Jika pilihan adalah sorting bedasarkan nama
    else if (pilihan == 3){
        // Jika pilihan sorting adalah naik
        if (PilihanSort == 1){
            if (strcmp(a->title, b->title) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        // Jika pilihan sorting adalah turun
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
    // Jika pilihan adalah sorting bedasarkan genre
    else if (pilihan == 4){
        // Jika pilihan sorting adalah naik
        if (PilihanSort == 1){
            if (strcmp(a->genre, b->genre) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        // Jika pilihan sorting adalah turun
        else if (PilihanSort == 2){
            if (strcmp(a->genre, b->genre) > 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    // Jika pilihan adalah sorting bedasarkan publisher
    else if (pilihan == 5){
        // Jika pilihan sorting adalah naik
        if (PilihanSort == 1){
            if (strcmp(a->publisher, b->publisher) < 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
        // Jika pilihan sorting adalah turun
        else if (PilihanSort == 2){
            if (strcmp(a->publisher, b->publisher) > 0){
                result = a;
                result->next = SortedMerge(a->next, b, pilihan, PilihanSort);
            } else {
                result = b;
                result->next = SortedMerge(a, b->next, pilihan, PilihanSort);
            }
        }
    }
    // Error handling jika pilihan diluar yang disediakan
    else {
        printf("Pilihan diluar yang disediakan, silahkan masukkan ulang !");
        getch();
    }
    // Mereturn hasil sorting
    return result;
}

void MergeSort(GamesPtr* headRef, int pilihan, int PilihanSort) {
    // Pendeklarasian pointer
    GamesPtr head = *headRef;
    GamesPtr a;
    GamesPtr b;

    // Jika data masih NULL
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    // Melakukan front back split
    FrontBackSplit(head, &a, &b);  

    // Merge sort dengan menggunakan parallel programming
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

    // Menjadikan data menjadi hasil sorting
    *headRef = SortedMerge(a, b, pilihan, PilihanSort); 
}

// Function untuk searching string
void searchingString(user **player[], char* namaDicari, int i, int loginKey){
    // Pendeklarasian variabel
    int j = 0;
    GamesPtr current = (*player)[loginKey]->Games;
    int found = 0;
    
    // String to lower nama yang ingin dicari
    string_to_lower(namaDicari);

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Searching data yang sesuai
    while (current != NULL) {
        char currentNameLower[100];

        // Jika pilihan adalah searching title
        if (i == 1) {
            strcpy(currentNameLower, current->title);
        }

        // Jika pilihan adalah searching genre
        else if (i == 2){
            strcpy(currentNameLower, current->genre);
        }

        // Jika pilihan adalah searching publisher
        else if (i == 3){
            strcpy(currentNameLower, current->publisher);
        }

        // String to lower data sekarang
        string_to_lower(currentNameLower);

        // Memeriksa apabila data sesuai dan print jika data sesuai
        if (strstr(currentNameLower, namaDicari) != NULL) {
            printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", j+1, current->title, current->publisher, current->genre, current->price, current->rating);
            found = 1;
        }

        // Pindah ke node selanjutnya
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

// Function untuk searching string pada shop
void searchingStringShop(NodeGames *Shop, char namaDicari[], int i) {
    // Deklarasi variabel
    int j = 0;
    int found = 0;

    // String to lower nama yang ingin dicari
    string_to_lower(namaDicari);

    NodeGames *current = Shop;

    printf("Data:\n");
    printf("_______________________________________________________________________________________________\n");
    printf("|No   |Title Game                 |Publisher            |Genre          |Price     |Rating    |\n");
    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Looping jika current masih NULL
    while (current != NULL) {
        char currentNameLower[100];
        
        // Jika pilihan searching title
        if (i == 1) {
            strcpy(currentNameLower, current->title);
        } 

        // Jika pilihan searching genre
        else if (i == 2) {
            strcpy(currentNameLower, current->genre);
        } 

        // Jika pilihan searching publisher
        else if (i == 3) {
            strcpy(currentNameLower, current->publisher);
        }

        // String to lower nama pada current
        string_to_lower(currentNameLower);

        // Compare hasil string
        if (strstr(currentNameLower, namaDicari) != NULL) {
            printf("|%-4d |%-26s |%-20s |%-15s|%-10.2f|%-10.2f|\n", j+1, current->title, current->publisher, current->genre, current->price, current->rating);
            found = 1;
        }

        // Pindah ke node next
        current = current->next;
        j++;
    }

    printf("|_____|___________________________|_____________________|_______________|__________|__________|\n");

    // Error handling jika data tidak ada yang sesuai
    if (!found) {
        system("cls");
        printf("Tidak ada data yang sesuai.\n");
        getch();
    }

    getch();
    system("cls");
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


