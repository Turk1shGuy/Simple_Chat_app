// Head Gurad
#ifndef GLOBAL_H
 #define GLOBAL_H

// Gerekli Kütüphaneler
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
#include <openssl/sha.h>


// Kullanıcı yapısı
struct user
{
    int // Kullanıcı Soketleri
        sndfd, // Mesaj gönderme Soketi
        getfd; // Mesaj alma Soketi

    // Kullanıcı adı, Şifre hash 'i ve SessionID
    char name[15 + 1];
    char passHash[256 + 1];
    char sessionid[128 + 1];

    // Kullanıcı aktif mi?
    bool active;

    // Sıradaki user
    struct user *next;
}; // Çevir


// Sunucu Konfigürasyonu
struct conf
{
    // Sunucu adı
    char srvName[15 + 1];

/*     // Hata ayıklama modu
    bool debugMode;

    // Komutlar
    bool commands;
     */

    // Oda lar geçerli mi?
    bool rooms;
    
/*     // Admin 'in adı
    char admin[15 + 1]; */

    // Veritabanındaki kullanıcı yapısı
    int numUsrs;
};


// all yapısı
struct all
{
    // Head & Tail
    struct user *head;
    struct user *tail;

    // Sunucu Soketleri
    int *getfd;
    int *sndfd;

    // Client giriş ve Mesaj alma Soketi ve Yapısı
    int *clgetfd;
    struct sockaddr_in *clgetst;

    // Kullanıcı adı
    char *uname;
};


// Header Guard end
#endif