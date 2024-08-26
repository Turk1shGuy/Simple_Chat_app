#include "../inc/adress.h"
#include "../inc/list.h"
#include "../inc/global.h"

// Adres defterini oku
int readAdressBook(struct user **head, struct user **tail)
{
    // Adres defteri için dosyayı aç
    FILE *fl = fopen(".addrbook.json", "r");
    if (fl == NULL) // Hata kontrol
    {
        // Hatayı bastır
        fprintf(stderr, "\nHata: Adres defteri açılamadı\n");

        // Hata ile bitir
        return 1;
    }


    // Adres defteri veilerini tutması için Buffer değeri
    char buffer[4096];

    // Adres Defterini oku
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fl);
    buffer[bytesRead] = '\0'; // Null karakter ekle

    // Dosyayı kapat
    fclose(fl);


    // Gerekli JSON Objeleri
    struct json_object *parsed;
    struct json_object *users;
    struct json_object *user;


    // JSON Yapısını parçala
    parsed = json_tokener_parse(buffer);
    if (parsed == NULL) // Hata kontrol
    {
        // Hatayı bastır
        fprintf(stderr, "Hata: JSON parse hatası\n");

        // Hata ile bitir
        return 1;
    }


    // "users", "Key" değerini al
    json_object_object_get_ex(parsed, "users", &users);
    size_t user_count = json_object_array_length(users); // Kullanıcı sayısını al
    

    // Kullanıcı sayısınca devam et
    for (size_t i = 0; i < user_count; i++)
    {
        // Kullanıcı index'ine göre user değer oluştur
        user = json_object_array_get_idx(users, i);

        // Kullanıcı adı için JSON objesi
        struct json_object *uname;
        

        // "uname", "Key" değerini bul
        if (json_object_object_get_ex(user, "uname", &uname))
        {
            // Yeni kullanıcı için yer ayırt
            struct user *new_user = (struct user*)malloc(sizeof(struct user));
            if (new_user == NULL) // Hata kontrol
            {
                // Hatayı bastır
                fprintf(stderr, "Hata: Bellek ayırma hatası\n");

                // Hata ile bitir
                return 1;
            }

            // Kullanıcı adını yeni Kullanıcıya ekle
            strcpy(new_user->uname, json_object_get_string(uname));


            // Sıradaki Kullanıcıyı NULL yap
            new_user->next = NULL;


            /* ==LISTEYE EKLE== */

            // Liste boş ise
            if (*head == NULL)
            {
                // Bütün Değerleri eşitle
                *head = new_user;
                *tail = new_user;
            }

            // Listede En az bir Kullanıcı var ise
            else
            {
                // Tail 'ın next değerini bu Kullanıcı yap
                (*tail)->next = new_user;

                // Tail 'a yeni kullanıcı değerini ata
                *tail = new_user;
            }
        }
    }


    // Belleği boşalt
    json_object_put(parsed);


    // Başarılı bitir
    return 0;
}


/*
    {
        "users":
        [
            {
                "uname": "Kullanıcı1"
            },
            {
                "uname": "Kullanıcı2"
            },
            {
                "uname": "Kullanıcı3"
            }
        ]
    }
*/