#include "global.h"

// Kullanıcı ekle
int adduser(char *uname, struct user **head, struct user **tail);

// index değeri ile kullanıcıyı bul
int fnduser(char *uname, struct user *index, struct user **head, struct user **tail);

// Kullanıcıyı sil
int delusr(char *uname, struct user **head, struct user **tail);

// Veritabanını yoket
int deldb(struct user **head);

