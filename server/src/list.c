/* Gerekli Kütüphane ve Başlık dosyalarını Ekle */
#include "../include/list.h"
#include "../include/global.h"
#include "../lib/print.h"


// Yeni Kullanıcı ekle
int addusr(char *uname, char *passh, struct user **head, struct user **tail)
{
    // Yeni Kullanıcı ıluştur
    struct user *newuser = (struct user*)malloc(sizeof(struct user));
    if (newuser == NULL)
    {
        // Bellekten yer açılamadıysa:

        return 1; // Bellek hatası
    }
    

    /* Değerleri ekle */
    newuser->active = false;
    
    strncpy(newuser->name, uname, sizeof(newuser->name) - 1);
    newuser->name[sizeof(newuser->name) - 1] = '\0'; // Null terminator
    
    newuser->next = NULL;

    // Eğer liste boşsa head ve tail'ı yeni kullanıcıya ayarla
    if (*head == NULL)
    {
        *head = newuser;
        *tail = newuser;
    }
    
    // Liste boş ise
    else
    {
        // Mevcut tail'in next'ine yeni kullanıcıyı ekle
        (*tail)->next = newuser;
    
        // Tail'i güncelle
        *tail = newuser;
    }

    return 0; // Başarılı
}


// Listeden Kullanıcı silme fonksiyonu
int delusr(char *uname, struct user **head)
{
    // Hata Kontrol: Liste boş mu?
    if (*head == NULL)
    {
        return 1; // Liste boş, hata
    }

    struct user *current = *head; // Şu anki kullanıcı
    struct user *previous = NULL; // Önceki Kullanıcı

    // Listeyi tara
    while (current != NULL)
    {
        // Kullanıcı adları uyuşuyor ise
        if (strcmp(uname, current->name) == 0)
        {
            // Kullanıcıyı sil
            if (previous == NULL)
            {
                // Silinecek kullanıcı başta ise
                *head = current->next; // Başka bir kullanıcı varsa başı güncelle
            }
            
            else
            {
                // Silinecek kullanıcı ortada veya sonda ise
                previous->next = current->next; // Önceki kullanıcının next'ini güncelle
            }


            free(current); // Bellekten sil
            return 0; // Başarıyla silindi
        }


        previous = current; // Önceki kullanıcıyı güncelle
        current = current->next; // Bir sonraki kullanıcıya geç
    }

    return 1; // Kullanıcı bulunamadı
}


// Ne kadar kullanıcı olduğunu say
int num_users(struct user *head)
{
    // Tarama için index değeri
    struct user *index = head;

    // Kullanıcı Sayısını tutacak değişken
    int k_sayi = 0;

    // Listedeki kullanıcı sayısını say
    while (index != NULL)
    {
        k_sayi++; // Kullanıcı sayısını arttır

        // Sıradaki kullanıcıya geç
        index = index->next;
    }

    // Kullanıcı sayısını döndür
    return k_sayi;
}


// Kullanıcı Soketlerini yapıya kullanıcı adı aracılığı ile ekle
int addvals(char *uname, char *sessionid, int *getfd, int *sndfd, struct user **head, struct user **tail)
{
    // Tarama için index değeri
    struct user *index = *head;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adları uyuşuyor ise
        if (strcmp(index->name, uname) == 0)
        {
            // Soketleri ata
            index->getfd = *getfd;
            index->sndfd = *sndfd;

            // SessionID değerini ata
            strncpy(index->sessionid, sessionid, sizeof(index->sessionid) - 1);
            index->sessionid[sizeof(index->sessionid) - 1] = '\0'; // Null terminator
        
            // Başarılı bit
            return 0;
        }

        index = index->next; // Bir sonraki elemana geç
    }

    // Başarısız bit
    return 1;
}


// Herhangi bir Kullanıcı Soketinden değerleri sıfırla
int reset_val2soket(int *clsoket, struct user **head, struct user **tail)
{
    // Tarama için index değeri
    struct user *index = *head;

    // Listeyi tara
    while (index != NULL)
    {
        // Soketler uyuşuyor mu?
        if (index->getfd == *clsoket || index->sndfd == *clsoket)
        {
            /* =DEĞERLERİ SIFIRLA= */
            index->active = false;

            // Soketleri kapat
            index->getfd = 0;
            index->sndfd = 0;

            // Session ID değerini sıfırla
            memset(index->sessionid, 0, strlen(index->sessionid));

            // Başarılı bit
            return 0;
        }
        
        index = index->next;
    }
    
    // Bulunamaz ise Başarısız bit
    return 1;
}


// Kullanıcı adından değerleri sıfırla
int reset_val2uname(char *uname, struct user **head, struct user **tail)
{
    // Tarama için index değeri
    struct user *index = *head;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adları uyuşuyor mu?
        if (strcmp(index->name, uname) == 0)
        {
            /* =DEĞERLERİ SIFIRLA= */
            index->active = false;

            // Soketleri kapat
            index->getfd = 0;
            index->sndfd = 0;

            // Session ID değerini sıfırla
            memset(index->sessionid, 0, strlen(index->sessionid));

            // Başarılı bit
            return 0;
        }
        
        // Bulunamaz ise sıradakine geç
        index = index->next;
    }
    
    // Bulunamadıysa başarısız bir
    return 1;
}


// Kullanıcı adından kullanıcıyı bul
int auth_name(char *uname, struct user *head, struct user *tail)
{
    // Tarama için index değeri
    struct user *index = head;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adları uyuşuyor mu?
        if (strcmp(uname, index->name) == 0)
        {
            // Başarılı
            return 0;
        }
        
        // Bulunamadıysa sıradaki Kullanıcıya
        index = index->next;
    }
    
    // Hiç bir şekilde bulunmaz ise
    return 1;
}


// Kullanıcı ID den kullanıcıyı bul
int auth_id(char *id, struct user *head, struct user *tail)
{
    // Tarama için index değeri
    struct user *index = head;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adları uyuşuyor mu?
        if (strcmp(id, index->sessionid) == 0)
        {
            // Başarılı
            return 0;
        }
        
        // Bulunamadıysa sıradaki Kullanıcıya
        index = index->next;
    }
    
    // Hiç bir şekilde bulunmaz ise
    return 1;
}


