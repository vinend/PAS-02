#ifndef STRUCTCUSTOM
#define STRUCTCUSTOM

typedef struct{
    char title[100];
    char genre[100];
    char desc[1000];
    char publisher[100];
    float rating;
    float price;
    int gameKey;
    struct videoGames* next;

} videoGames;

typedef struct{
    char gamerTag[100];
    char password[100];
    struct videoGames; 
} user;

#endif