/* GEREKLI KUTUPHANE VE BASLIK DOSYALARINI EKLE */
#include "../include/global.h"
#include "../include/user.h"
#include "../include/list.h"


// ID 'yi üret
void genid(char *id)
{
    // Alfabe
    char alph[] = "!\"#$%%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    // Ragele değerleri tutması için değişken
    int randint;

    // 128 Karakterli ID'yi üret
    for (int i = 0; i < 128; i++)
    {
        // Rasgele oluşum
        randint = rand() % (sizeof(alph) - 1); // Alfabenin boyutunu kullan

        // Değeri ata
        id[i] = alph[randint];
    }

    // Dizi sonlandırma
    id[128] = '\0';
}


// ID 'yi alıcısına gönder
int sndid(char *id, int *recvierfd)
{
    // ID yi gönder
    if (send(*recvierfd, id, strlen(id), 0) != 0)
    {
        // Bağlantıyı kopart
        close(*recvierfd);

        // Başarısız bit
        return 1;
    }
    
    // Başarılı bit
    return 0;
}


// Kullanıcı giriş bilgilerini al (Mesaj gönderme Soket 'i buradan)
int getlogincr(int *clgetfd, int *clsndfd, int *srvsndfd, struct sockaddr_in *clsndst, char *uname, char *pass, char *id, struct user *head)
{
    // Kullanıcı adını al
    if (recv(*clgetfd, uname, sizeof(uname), 0) != 0)
    {
        // Alınamadı
        return 1;
    }
    
    // Parola hash 'ini al
    if (recv(*clgetfd, pass, sizeof(pass), 0) != 0)
    {
        // Alınamadı
        return 1;
    }
    
    
    // Giriş bilgilerini doğrula
    if (ulogin(uname, pass, head) != 0)
    {
        // Soketi kapat
        close(*clgetfd);

        // Başarısız
        return -1;
    }
    

    // id üret
    genid(id);


    // ID 'yi alıcısına gönder
    if (send(*clgetfd, id, strlen(id), 0) != 0)
    {
        // Soketleri Kapat
        close(*clgetfd);

        // Başarısız bit
        return 1;
    }


    // Soket boyutu
    socklen_t length = sizeof(*clsndst);

    // istemciden Mesaj alma Soketini bağla
    *clsndfd = accept(srvsndfd, (struct sockaddr*)clsndst, &length);


    // Başarılı
    return 0;
}


// Kullanıcı adı ve Şifre doğrula
int ulogin(char *uname, char *passh, struct user *head)
{
    // Tarama için index değeri
    struct user *index = head;


    // Listeyi sonuna kadar tara
    while (index != NULL)
    {
        // Kullanıcı adı ve Şifresini kontrol et
        if (strcmp(index->name, uname) == 0 && strcmp(index->passHash, passh) == 0)
        {
            // Kullanıcı adı ve Şifre uyuşuyor
            return 0;
        }
        
        // Sıradaki user 'a geç
        index = index->next;
    }
    
    // Buluunamaz ise
    return 1;
}


// Mesajı al
int getMsg(char *msg, int *clsndfd, struct user *head, struct user *tail)
{
    // Mesajı al
    if (recv(*clsndfd, msg, sizeof(msg), 0) != 0)
    {
        // Mesajı alma başarısız
        return 1;
    }

    // Başarılı
    return 0;
}


// Mesajı çevir (ID ve Gönderen adını doğrula)
int convert(char **msg, char *recvierName, struct user *head, struct user *tail)
{
    /*
        Örnek Mesaj JSON yapısı:
        
        Gönderen
        {
            "id": "I*y,O]n-wZt,+){ZbsS\\rr;9#~`}J||s%v!PRn^GFSSO\\P*<A]x1N1JM.,KX&HM(=JXl744]ge*A34]ToTc;e.ep72J;WtCq?x[S.p/rS92fKo88A{S%(9u?H@W}5zoR",
            "sender": "mehmet",
            "recvier": "ali",
            "msg": "Merhaba, Ali!"
        }

        Alıcı
        {
            "sender": "mehmet",
            "msg": "Merhaba Ali!"
        }
    */


    struct json_object *parsed;
    struct json_object *id;
    struct json_object *sender;
    struct json_object *receiver; // "recvier" yerine "receiver"
    struct json_object *_msg_;

    // Değeri parçala
    parsed = json_tokener_parse(*msg); // *msg yerine *msg

    // Hata kontrol
    if (!parsed)
    {
        return 1; // JSON parse hatası
    }


    if // Değerleri ata
    (
        !json_object_object_get_ex(parsed, "id", &id) ||
        !json_object_object_get_ex(parsed, "sender", &sender) ||
        !json_object_object_get_ex(parsed, "recvier", &receiver) ||
        !json_object_object_get_ex(parsed, "msg", &_msg_)
    )
    {
        // Hatalara karşı serbest bırak
        json_object_put(parsed);

        // Anahtar bulunamadı
        return 1;
    }


    if // ID ve Gönderen adını doğrula
    (
        auth_name((char *)json_object_get_string(sender), head, tail) != 0 ||
        auth_id((char *)json_object_get_string(id), head, tail) != 0
    )
    {
        json_object_put(parsed);
        return 1; // Başarısız dön
    }

    /* MESAJI ALICI FORMATINA DONUSTUR */
    struct json_object *receiver_msg = json_object_new_object();
    json_object_object_add(receiver_msg, "sender", json_object_get(sender));
    json_object_object_add(receiver_msg, "msg", json_object_get(_msg_));


    // Yeni JSON nesnesini string 'e ata
    const char *new_msg = json_object_to_json_string(receiver_msg);
    if (new_msg)
    {
        // Eski mesajı serbest bırak
        free(*msg);

        // Yeni mesajı kopyala
        *msg = strdup(new_msg);

        // Hata kontrol
        if (*msg == NULL)
        {
            // Mesajları serbest bırak
            json_object_put(receiver_msg);
            json_object_put(parsed);

            // Bellek ayırma hatası
            return 1;
        }
    }


    // Bellek sızıntısını önlemek için JSON nesnelerini serbest bırak
    json_object_put(receiver_msg);
    json_object_put(parsed);


    // Başarıyla dön
    return 0;
}


// Mesajı gönder
int sndMsg(char *msg, char *recvierName, struct user *head, struct user *tail)
{
    /*
        Alıcı
        {
            "sender": "mehmet"
            "msg": "Merhaba Ali!"
        }
    */

    // TODO: Listeden alıcı adına göre mesajı ilet


    // Tarama için alici
    struct user *alici = head;

    // Listeyi tara ve alıcıyı bul
    while (alici != NULL)
    {
        // Kullanıcı adını kontrol et
        if (strcmp(alici->name, recvierName) == 0)
        {
            /* =KULLANICI ADI BULUNDUYSA= */

            // Soketi kontrol et
            if (alici->getfd == 0)
            {
                // Soket açık değil
                return 1;
            }
            

            // Mesajı alıcısının Soketine ilet
            if (send(alici->getfd, msg, strlen(msg), 0) != 0)
            {
                // Hata durumunda 
                return -1;
            }
            

            // Gönderildi başarılı bit
            return 0;            
        }
        
        // Sıradaki kullanıcıya geç
    }
    

    // Alıcı bulunamadıysa
    return 0;
}


