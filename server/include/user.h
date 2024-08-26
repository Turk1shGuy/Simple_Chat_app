// struct user için
#include "global.h"

// ID 'yi üret
void genid(char *id);


// ID 'yi alıcısına gönder
int sndid(char *id, int *recvierfd);


// Kullanıcı giriş bilgilerini al 
int getlogincr(int *clgetfd, int *clsndfd, int *srvsndfd, struct sockaddr_in *clsndst, char *uname, char *pass, char *id, struct user *head);


// Kullanıcı adı ve Şifre doğrula
int ulogin(char *uname, char *passh, struct user *head);


// Mesaj al
int getMsg(char *msg, int *clgetfd, struct user *head, struct user *tail);


// Mesajı çevir
int convert(char **msg, char *recvierName, struct user *head, struct user *tail);


// Mesajı gönder
int sndMsg(char *msg, char *recvierName, struct user *head, struct user*tail);

