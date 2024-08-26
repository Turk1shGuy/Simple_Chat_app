#include "../include/global.h"

// Yeni Kullanıcı ekle
int addusr(char *uname, char *passh, struct user **head, struct user **tail);


// Kullanıcı sil
int delusr(char *uname, struct user **head);


// Ne kadar kullanıcı var?
int num_users(struct user *head);


// Kullanıcı Soketlerini yapıya kullanıcı adı aracılığı ile ekle
int addvals(char *uname, char *sessionid, int *getfd, int *sndfd, struct user **head, struct user **tail);


// Herhangi bir Kullanıcı Soketinden değerleri sıfırla
int reset_val2soket(int *clsoket, struct user **head, struct user **tail);


// Kullanıcı adından değerleri sıfırla
int reset_val2uname(char *uname, struct user **head, struct user **tail);


// ID 'yi Listeden kontrolet
int fnd_id2list(char *id, struct user *head, struct user *tail);


// Kullanıcı adından kullanıcıyı bul
int auth_name(char *uname, struct user *head, struct user *tail);


// Kullanıcı ID den kullanıcıyı bul
int auth_id(char *id, struct user *head, struct user *tail);

