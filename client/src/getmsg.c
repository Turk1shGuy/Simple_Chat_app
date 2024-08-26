#include "../inc/global.h"
#include "../inc/getmsg.h"
#include "../libs/print.h"


// Mesaj al
void *getmsg(void *args)
{
    // Gerekli değerleri çek
    struct all *_all = (struct all*)args;

    // Mesajı saklmak için değişken
    char msg[2048];

    // Mesaj için gerekli JSON objeleri
    struct json_object *parse;
    struct json_object *sender = NULL;
    struct json_object *msgg = NULL;


    // Mesaj almak için sonsuz göndü
    while (1)
    {
        // Değeri Sıfırla
        memset(msg, 0, sizeof(msg));
        json_object_put(parse);

        // Mesajı al
        if (recv(*_all->getfd, msg, sizeof(msg), 0) <= 0)
        {
            /* ==HATA DURUMUNDA== */
            printColor(RED, "\nHATA: MESAJ ALIRKEN HATA OLUŞTU\nKapatılıyor...\n\n");
            fprintf(stderr, "Error: ");

            // Bitir
            return NULL;
        }

        // Gelen JSON Objesini parçala
        parse = json_tokener_parse(msg);

        // Hata kontrol
        if (parse == NULL)
        {
            fprintf(stderr, "Error parsing JSON: %s\n", msg);
            continue;
        }

        // JSON Objelerini ata
        if (!json_object_object_get_ex(parse, "sender", &sender) || !json_object_object_get_ex(parse, "msg", &msgg))
        {
            /* ==HATA DURUMUNDA== */
            fprintf(stderr, "Error: Missing 'sender' or 'msg' in JSON\n");
            json_object_put(parse);
            continue;
        }

        // Mesajı yazdır
        printf("\nMesaj (%s): %s", json_object_get_string(sender), json_object_get_string(msgg));
    }
}



// Giriş bilgilerini al
void getcr(char *id, int *getdf, struct sockaddr_in *getst, int *sndfd, struct sockaddr_in *sndst)
{
    // Gerekli değerler
    char uname[15 + 1], pass[40 + 1], tof[2];

    // Kullanıcı adını al
    printf("Kullanıcı adınız: ");
    fgets(uname, sizeof(uname), stdin);

    // Yeni satır karakteri (\n) kaldır
    size_t len = strlen(uname);
    if (len > 0 && uname[len - 1] == '\n') uname[len - 1] = '\0';


    // Kullanıcı adını gönder
    if (send(*sndfd, uname, strlen(uname), 0) <= 0)
    {
        printColor(RED, "\nHata: Kullanıcı adı gönderilemedi\n");
        return;
    }


    // Şifre al
    printf("\nŞifreniz: ");
    fgets(pass, sizeof(pass), stdin);

    // Yeni satır karakteri (\n) kaldır
    len = strlen(pass);
    if (len > 0 && pass[len - 1] == '\n') pass[len - 1] = '\0';


    // Parolayı gönder
    if (send(*sndfd, pass, strlen(pass), 0) <= 0)
    {
        printColor(RED, "\nHata: Şifre gönderilemedi\n");
        return;
    }


    // Doğrulandı mı?
    if (recv(*sndfd, tof, sizeof(tof), 0) <= 0 || tof[0] == '1')
    {
        printColor(RED, "Hatalı giriş bilgileri\n");
        return;
    }


    // ID değerini al
    if (recv(*sndfd, id, 40, 0) <= 0)
    {
        printColor(RED, "Hatalı Kullanıcı ID 'si alınamadı \n");
        return;
    }
}


