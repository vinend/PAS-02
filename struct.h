#ifndef STRUCTCUSTOM
#define STRUCTCUSTOM

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

typedef struct videoGames NodeGames;

typedef NodeGames *GamesPtr;

struct User{
    char gamerTag[100];
    char password[100];
    GamesPtr Games; 
};

typedef struct User user;

#endif