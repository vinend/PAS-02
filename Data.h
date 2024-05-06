#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "struct.h"

void TampilkanData(user **player){
    int i = 0, pilihan, trigger;

    printf("Data:\n");
    printf("_________________________________________________________________________________________________\n");
    printf("|No\tTitle Game\t\tPublisher\tGenre\tHarga\tRating                               \n");
    printf("|_______________________________________________________________________________________________|\n");

    while ((*player)->Games->next != NULL)
    {
        printf("|%-2d\t%-20s\t%11s\t%9s\t%9d\t%11d\n", i+1, (*player)->Games->title, (*player)->Games->publisher, (*player)->Games->genre, (*player)->Games->price, (*player)->Games->rating);
    }

    do {
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
            sortingRating(player, jumlahData);
            break;

            case 2 : 
            sortingHarga(player, jumlahData);
            break;

            default :
            
            break;
        }
}

void pilihSearch(user **player, int jumlahData){
    int pilihan;
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
            searchingTitle(player, jumlahData);
            break;

            case 2 : 
            searchingGenre(player, jumlahData);
            break;

            case 3 : 
            searchingPublisher(player, jumlahData);
            break;

            default :
            
            break;
        }
}

void sortingRating(user **player, int jumlahData){
    GamesPtr h; 
    int swapped;

    for(int i = 0; i < jumlahData; i++) {
        h = (*player)->games;
        swapped = 0;
        
        for(int j = 0; j < jumlahData - i - 1; j++) {
            GamesPtr p1 = h;
            GamesPtr p2 = p1->next;

            if(p1->rating > p2->rating) {
                h = swap(p1, p2);
                swapped = 1;
            }

            h = h->next;
        }

        if(swapped == 0) {
            break;
        }
    }
}

void sortingHarga(user **player, int jumlahData){
    GamesPtr h; 
    int swapped;

    for(int i = 0; i < jumlahData; i++) {
        h = (*player)->games;
        swapped = 0;
        
        for(int j = 0; j < jumlahData - i - 1; j++) {
            GamesPtr p1 = h;
            GamesPtr p2 = p1->next;

            if(p1->price > p2->price) {
                h = swap(p1, p2);
                swapped = 1;
            }

            h = h->next;
        }

        if(swapped == 0) {
            break;
        }
    }
}

void searchingTitle(user **player, int jumlahData){
    struct Node* current = *head;
    int found = 0;

    printf("%-20s %-10s %-10s %-10s %-10s\n", "Name", "Kelas", "Gerbong", "Kursi", "Harga");
    printf("---------------------------------------------------------------\n");

    while (current != NULL) {
        if (current->harga >= BatasBawah && current->harga <= BatasAtas) {
            printf("%-20s %-10s %-10d %-10d %-10.2f\n", current->namaPenumpang, current->kelas, current->noGerbong, current->noKursi, current->harga);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("Tidak ada data yang sesuai.\n");
    }
}

void searchingGenre(user **player, int jumlahData){
    
}

void searchingPublisher(user **player, int jumlahData){

}