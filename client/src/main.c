#include "../inc/global.h"
#include "../inc/adress.h"
#include "../libs/print.h"
#include "../inc/ui.h"
#include "../inc/log.h"
#include "../inc/getmsg.h"

int main(int argc, char const *argv[])
{
    // Head ve Tail değerleri
    struct user *head = NULL;
    struct user *tail = NULL;

    if (readAdressBook(&head, &tail) != 0)
    {
        // Renkli yazdır
        printColor(RED, "Adres Defteri boş\n\n");
    }


    // Soketler için Yapı
    struct sockaddr_in getst, sndst;

    // Mesaj al
     getst.sin_addr.s_addr = INADDR_ANY;
     getst.sin_family = AF_INET;
     getst.sin_port = htons(8082);

    // Mesaj gönder
     sndst.sin_addr.s_addr = INADDR_ANY;
     sndst.sin_family = AF_INET;
     sndst.sin_port = htons(8181);

    // Soket boyutları
    socklen_t getstlen = sizeof(getst);
    socklen_t sndstlen = sizeof(sndst);


    // Soket oluştur
    int getfd = socket(AF_INET, SOCK_STREAM, 0),
        sndfd = socket(AF_INET, SOCK_STREAM, 0);


    // Bağlantı sağla
    if (connect(getfd, (struct sockaddr*)&getst, getstlen) != 0 || connect(sndfd, (struct sockaddr*)&sndst, sndstlen) != 0)
    {
        /* HATA DURUMUNDA */

        // Soketleri Kapat
        close(getfd);
        close(sndfd);

        // Adres defterini yoket
        deldb(*head);

        // Çıkış yap
        return 1;
    }
    
    // Renkli Çıktı ver
    printColor(GREEN, "BAĞLANTI BAŞARILI\n");


    // Giriş bilgilerini al
    char id[128];
    getcr(id, &getfd, &getst, &sndfd, &sndst);


    // Kullanıcı UI ını başlat sonsuz bir döngüdür [Not]: dinleme işlemide buradan yapılacak
    startUI(id, head, tail, &getfd, &getst, &sndfd, &sndst);


    // Soketleri Kapat
    close(getfd);
    close(sndfd);


    // Başarılı
    return 0;
}
