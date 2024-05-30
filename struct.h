#ifndef STRUCTCUSTOM
#define STRUCTCUSTOM

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
    float Uang;
    GamesPtr Games; 
};
#endif