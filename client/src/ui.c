#include "../inc/ui.h"
#include "../inc/global.h"
#include "../inc/getmsg.h"
#include "../inc/list.h"


// Kullanıcı UI 'ını başlat
void startUI(char *id, struct user **head, struct user **tail, int *getfd, struct sockaddr_in *getst, int *sndfd, struct sockaddr_in *sndst)
{
    // Mesajı tutmak için değer
    char msg[2048], msg2[2000], alici[15 + 1];

    // Her şey yapısı
    struct all _all = {getfd, sndfd, getst, sndst};

    // Mesaj almak için thread oluştur
    pthread_t thread; // Thread
    if (pthread_create(&thread, NULL, getmsg, (void*)&_all) != 0) // Hata durumunda
    {
        // Soketleri Kapat
        close(*getfd);
        close(*sndfd);

        // Adres defterini yoket
        deldb(head); // Düzeltildi: head yerine &head kullanıldı

        // Çıkış yap
        return;
    }

    // Örnek Mesaj girdisi
    printf("Örnek Mesaj: Ahmet, Merhaba Ahmet!\n\n");

    // Mesaj gönder
    while (1)
    {
        // Mesaj al
        printf("Mesaj giriniz: ");
        fgets(msg, sizeof(msg), stdin);

        // Yeni satır karakterini sil
        size_t len = strlen(msg);
        if (len > 0 && msg[len - 1] == '\n')
        {
            msg[len - 1] = '\0';
        }

        // Mesajı Parçala
        sscanf(msg, "%49[^,],%49[^\n]", alici, msg2);

        // Bağlantı bitirme komutu ise
        if (strcmp(msg2, "close") == 0)
        {
            // Soketleri Kapat
            close(*getfd);
            close(*sndfd);

            // Adres defterini yoket
            deldb(head); // Düzeltildi: head yerine &head kullanıldı

            // Çıkış yap
            return;
        }

        // Mesajı gönder
        if (send(*_all.sndfd, msg, strlen(msg), 0) != 0) // Düzeltildi: _all.getfd yerine *_all.sndfd kullanıldı
        {
            // Soketleri Kapat
            close(*getfd);
            close(*sndfd);

            // Adres defterini yoket
            deldb(head); // Düzeltildi: head yerine &head kullanıldı

            // Çıkış yap
            return;
        }

        // Sıradaki tekrara
        continue;
    }
}
