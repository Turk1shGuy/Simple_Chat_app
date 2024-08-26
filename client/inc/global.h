// Başlık Koruması
#ifndef GLOBAL_H
 #define GLOBAL_H

/* Gerekli Kütüphaneler */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <json-c/json.h>

// Gereli dönüşümler
typedef struct sockaddr_in soi;
typedef struct sockaddr so;


// Kullanıcı listesi
struct user
{
    // Kullanıcı adı
    char uname[15 + 1];


    // Sıradaki liste
    struct user *next;
};


// Thread oluşturmak için 
struct all
{
    int *getfd;
    int *sndfd;

    struct sockaddr_in *getst;
    struct sockaddr_in *sndst;

    char *id;
};

#endif