#include "../include/global.h"


// Veritabanını oku
int readdb(struct user *head, struct user *tail);

// Veritabanını yok et
int freedb(struct user *head, struct user *tail);

// Konfigürasyonu oku
int readconf(struct conf *sconf, struct user *head, struct user *tail);


// Hatayı dosyaya kayıt et
void logError(char *text);

// Giriş bilgilerini dosyaya kayıt et
void logLogin(char *text, char *uname);


// Gelen Parolayı hash 'le
void hashpasswd(char *pass);
