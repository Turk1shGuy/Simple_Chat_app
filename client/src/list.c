#include "../inc/list.h"
#include "../inc/global.h"
#include "../libs/print.h"
#include "../inc/log.h"

// Kullanıcı ekle
int adduser(char *uname, struct user **head, struct user **tail)
{
    // Yeni Kullanıcı için bellekten yer ayırt
    struct user *new = (struct user *)malloc(sizeof(struct user));
    if (new == NULL) // Hata kontrol
    {
        // Hatayı dosyaya kayıt et
        logError("Hata: Bellekte Yer ayırtılamadı");

        // Çıktı ver
        printColor(RED, "Hata: Bellekte Yer ayırtılamadı");

        // Hata ile bit
        return 1; 
    }

    // Kullanıcıya adını ver ve NULL Terminator ekle
    strncpy(new->uname, uname, sizeof(new->uname) - 1);
    new->uname[sizeof(new->uname) - 1] = '\0';

    // Sıradaki Listeyi Boşa bırak
    new->next = NULL;

    // Linked List boş ise
    if (*head == NULL)
    {
        *head = *tail = new;
    }
    else
    {
        // Sona ekle
        (*tail)->next = new;
        // Kuytuğu güncelle
        *tail = new;
    }

    return 0; // Başarılı
}

// index değeri ile kullanıcıyı bul
int fnduser(char *uname, struct user *index, struct user **head, struct user **tail)
{
    // index değerini başa eşitle
    index = *head;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adını kontrol et
        if (strcmp(uname, index->uname) == 0)
        {
            // Başarılı bulundu
            return 0;
        }
        
        // Sıradaki Kullanıcıya geç
        index = index->next;
    }

    // Bulunamazsa başarısız döndür
    return 1;    
}

// Kullanıcıyı sil
int delusr(char *uname, struct user **head, struct user **tail)
{
    struct user *index = *head;
    struct user *prev = NULL;

    // Listeyi tara
    while (index != NULL)
    {
        // Kullanıcı adını kontrol et
        if (strcmp(uname, index->uname) == 0)
        {
            // Kullanıcı bulundu, sil
            if (prev == NULL) // İlk eleman
            {
                *head = index->next;
            }
            else
            {
                prev->next = index->next;
            }

            if (index == *tail) // Son eleman
            {
                *tail = prev;
            }

            free(index); // Belleği serbest bırak
            return 0; // Başarılı
        }
        
        prev = index;
        index = index->next;
    }
    
    // Hiç bulunmaz ise kapat
    return 1;
}

// Veritabanını yoket
int deldb(struct user **head)
{
    // Silme işlemi için index değeri
    struct user *index = *head;
    struct user *prev;

    // Listenin boş olup olmadığını kontrol et
    if (*head == NULL)
    {
        // Hatayı bastır
        printColor(RED, "Hata: Adres defteri boş");

        // Hata ile bitir
        return 1;
    }
    

    // Listeyi gez ve Yoket
    while (index != NULL)
    {
        // Bir önceki değer
        prev = index;

        // Sıradaki değer
        index = index->next;

        // Yoket
        free(prev);

        // Sıradaki tekrar
        continue;
    }

    // Başarılı bitir
    return 0;
}

