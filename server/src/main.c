/* GEREKLI BAŞLIK DOSYALARI VE KÜTÜPAHNELERI DAHİL ET */
#include "../include/global.h"
#include "../include/server.h"
#include "../include/list.h"
#include "../include/user.h"
#include "../include/start.h"
#include "../lib/print.h"


/* Ana Fonksiyon */
int main(int argc, char const *argv[])
{
    /* head & tail */
    struct user *head = NULL;
    struct user *tail = NULL;
    
    // Sunucu konfigürasyonu
    struct conf sconf;

    // Veritabanını ve Sunuvu yapılandırmasını oku (Hata durumlarında ekrana çıktı verilmekte)
    if (readdb(head, tail) != 0 || readconf(&sconf, head, tail) != 0)
    {
        // Veritabanını bırak
        freedb(head, tail);
        free(head);
        free(tail);

        // Hata ile bitir
        return 1;
    }


    /* SUNUCU SOKET VE SOKET YAPILARI */
    struct sockaddr_in getst, sndst;

    socklen_t // Soketlerin boyutları
        getstsize = sizeof(getst),
        sndstsize = sizeof(sndst);

    int // getfd ve sndfd gönderme Soket'i
        getfd = socket(AF_INET, SOCK_STREAM, 0),
        sndfd = socket(AF_INET, SOCK_STREAM, 0);    


    // Soket hata kontrol
    if (getfd == -1 || sndfd == -1)
    {
        // Soketleri Kapat
        close(getfd);
        close(sndfd);

        // Veritabanını serbest bırak
        freedb(head, tail);

        // Hatayı bas
        printColor(RED, "Hata: Soketler oluşturulamadı\nDetaylı çıktı:");
        perror("");

        // Başarısız bitir
        return 1;
    }
    


    // getfd ve sndfd için Bind işlemleri
    if (bind(getfd, (struct sockaddr*)&getst, getstsize) == -1 || bind(sndfd, (struct sockaddr*)&sndst, sndstsize) == -1)
    {
        // Soketleri Kapat
        close(getfd);
        close(sndfd);

        // Veritabanını serbest bırak
        freedb(head, tail);

        // Hatayı bas
        printColor(RED, "Hata: bind işlemi yapılamadı\nDetaylı çıktı:");
        perror("");

        // Başarısız bitir
        return 1;
    }



    // Soketleri dinle
    if (listen(getfd, sconf.numUsrs) == -1 || listen(sndfd, sconf.numUsrs) == -1)
    {
        // Soketleri Kapat
        close(getfd);
        close(sndfd);

        // Veritabanını serbest bırak
        freedb(head, tail);

        // Hatayı bas
        printColor(RED, "Hata: dinleme işlemi yapılamadı\nDetaylı çıktı:");
        perror("");

        // Başarısız bitir
        return 1;
    }
    

    // thread oluşturmak için all yapısı
    struct all ALL;
     ALL.getfd = &getfd;
     ALL.sndfd = &sndfd;
     ALL.head = head;
     ALL.tail = tail;


    /* While true döngüsü */    
    while (true)
    {
        // Kullanıcı Soket yapısı
        struct sockaddr_in clgetst;

        // Kullanıcı Soket boyutu
        socklen_t *cllen = (socklen_t*)malloc(sizeof(socklen_t));
    
        // Kullancı soketi
        int clgetfd = accept(getfd, (struct sockaddr*)&clgetst, cllen);

        // istemci Soket hata kontrol
        if (clgetfd == -1)
        {
            // Soket'i kapat
            close(clgetfd);

             // Bellekten bırak
             free(cllen);

            // Döngünün bir Sonraki tekrarına geç
            continue;
        }
        

        // ALL Yapısına yeni değerleri ekle
        ALL.clgetfd = &clgetfd;
        ALL.clgetst = &clgetst;


        pthread_t thread; // Thread oluştur
        if (pthread_create(&thread, NULL, st4rt, (void*)&ALL) != 0)
        {
            /* =HATA_DURUMUNDA= */
        
            // Soket'i kapat
            close(clgetfd);

             // Bellekten bırak
             free(cllen);

            // ALL 'dan değerleri sil
            ALL.clgetfd = NULL;
            ALL.clgetst = NULL;

            // Döngünün bir Sonraki tekrarına geç
            continue;
        }


        // Bitti bir Sonraki tekrara geç
        continue;
    }


    // Başarılı bitir
    return 0;
}
