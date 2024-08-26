#include "../include/global.h"
#include "../include/start.h"
#include "../include/user.h"
#include "../include/list.h"


// Kullanıcıyı başlat
void *st4rt(void *args)
{
    // Soketleri, Head ve Tail yapısını al
    struct all *__all__ = (struct all*)args;


    // head ve tail
    struct user *head = __all__->head;
    struct user *tail = __all__->tail;



    int // Daha Kolay olması için
        *getfd = __all__->getfd, // [SUNUCU] Kullanıcı Mesaj alma Soket'i
        *sndfd = __all__->sndfd; // [SUNUCU] Kullanıcı Mesaj gönderme Soket'i


    int // Daha kolay olması için
        *clgetfd = __all__->clgetfd, // Kullanıcı Sunucudan Mesaj alma Soketi
        *clsndfd; // Kullanıcıdan Sunucuya Mesaj gönderme Soketi
        struct sockaddr_in clsndst;


    char // Kullanıcı adı & Şifre
        *pass = (char*)malloc((15 + 1) * sizeof(char)),
        *uname = (char*)malloc((40 + 1) * sizeof(char)),
        *id = (char*)malloc(128 * sizeof(char));



    /* GIRIŞ BILGILERINI AL */
    if (getlogincr(clgetfd, clsndfd, sndfd, &clsndfd, uname, pass, id, head) != 0)
    {
        // Soket 'i kapat
        close(*getfd);

        // Giriş bilgilerini logla
        logLogin("Kullanıcı giriş denemesi başarısız", uname);

         // Belleği serbest bırak
         free(uname);
         free(pass);

        // Başarısız bit
        return;
    }



    // Listeye değerleri ekle
    if (addvals(uname, id, clgetfd, clsndfd, head, tail) != 0)
    {
        // Başarısız bit
        return NULL;
    }
    


    // Belleği serbest bırak
    free(pass);
    free(uname);


    char
        // Mesaj
        *msg = (char*)malloc((2048 + 1) * sizeof(char)),
        
        // Alcıcı adı
        *rname = (char*)malloc((15 + 1) * sizeof(char));

        if (msg == NULL || rname == NULL)
        {
            // Ne olur ne olmaz boşalt
            free(msg);
            free(rname);

            // Döngünün bir sonraki tekrarına geç
        }


    while (true)
    {   
        if // Soket işlemleri
        (
            // Mesajı al
            getMsg(msg, sndfd, head, tail) != 0 ||
            
            // Mesajı işle/Çevir
            convert(msg, rname, head, tail) != 0 ||

            // Mesajı alıcısına ilet
            sndMsg(msg, rname, head, tail) != 0
        )
        {   /* HATA: DURUMUNDA */

            // Hata kontrol
            if (strcmp(rname, "Err0r") == 0)
            {
                // Hata ile dön
                return NULL;
            }

            // Değerleri Sıfırla
            memset(msg, 0, strlen(msg));
            memset(rname, 0, strlen(rname));

            // Döngünün bir Sonraki tekrarına
            continue;
        }

        // Değerleri Sıfırla
        memset(msg, 0, strlen(msg));
        memset(rname, 0, strlen(rname));
    }
    

    // Belleği boşalt
    free(msg);
    free(rname);

    // Hata ile dön
    return NULL;
}