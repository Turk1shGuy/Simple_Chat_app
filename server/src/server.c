#include "../include/global.h"
#include "../include/server.h"
#include "../include/list.h"
#include "../lib/print.h"


// Veritabanını oku
int readdb(struct user *head, struct user *tail)
{
    // Veritabanı dosyası aç
    FILE *database = fopen("users.json", "r");

    // Dosyanın açıldığına dair kontrol
    if (!database)
    {
        // Dosya yolunu kapat
        fclose(database);

        // Hatayı ekrana bas
        printColor(RED, "Hata: Veritabanı dosyası (users.json) açılamadı\nDetaylı Hata çıktısı:");
        perror("");

        // Dosya açılamadıysa hata döndür
        return -1;
    }


    // Buffer değeri
    char buffer[4096];

    // Dosyayı oku
    fread(buffer, sizeof(char), sizeof(buffer), database);
    
    // Dosyayı Kapat
    fclose(database);


    /* Gerekli JSON Objeleri */
    struct json_object *parsed_json = json_tokener_parse(buffer); // Dağıtık JSON
    struct json_object *user_array; // Kullanıcı dizisi
    struct json_object *user_obj; // Kullanıcı objesi
    struct json_object *uname_obj; // Kullanıcı adı
    struct json_object *passh_obj; // Kullanıcı Şifresi


    // Kullanıcı dizisini al
    json_object_object_get_ex(parsed_json, "users", &user_array);
    size_t user_count = json_object_array_length(user_array); // Dizi boyutu


    // Her bir kullanıcıyı işle
    for (size_t i = 0; i < user_count; i++)
    {
        // i değeri kadar array 'den verileri user_onj sine ata
        user_obj = json_object_array_get_idx(user_array, i);

        // Kullanıcı adını al
        json_object_object_get_ex(user_obj, "uname", &uname_obj);
        
        // Kullanıcı Şifre hash 'ini al
        json_object_object_get_ex(user_obj, "passHash", &passh_obj);


        // Değerleri Veritabanına ekle
        addusr((char*)json_object_get_string(uname_obj), (char*)json_object_get_string(passh_obj), &head, &tail);
    }


    // JSON objelerini serbest bırak
    json_object_put(parsed_json);
    json_object_put(user_array);
    json_object_put(user_obj);
    json_object_put(uname_obj);
    json_object_put(passh_obj);


    return 0; // Başarılı
}


// Veritabanını yok et
int freedb(struct user *head, struct user *tail)
{
    // Şu anki kullanıcı
    struct user *current = head;
    
    // Sıradaki Kullanıcı
    struct user *next;


    // Kullanıcı listesini dolaş
    while (current != NULL)
    {
        next = current->next; // Bir sonraki kullanıcıyı kaydet

        // Kullanıcıyı serbest bırak
        free(current);

        // Sıradakine geç
        current = next; // Bir sonraki kullanıcıya geç
    }


    // Başlangıç ve bitiş işaretçilerini NULL yap
    head = NULL;
    tail = NULL;


    // Başarılı bit
    return 0;
}


// Konfigürasyonu oku
int readconf(struct conf *sconf, struct user *head, struct user *tail)
{
    // Sunucu Konfigürasyon dosyasını aç
    FILE *config = fopen("server.json", "r");

    // Buffer değeri
    char buffer[128];
    
    // Dosyayı oku
    fread(buffer, sizeof(buffer), 1, config);

    // Dosyayı kapat
    fclose(config);


    // Gerekli JSON Objeleri
    struct json_object *parsed = json_tokener_parse(buffer);
    struct json_object *server_name;
    struct json_object *rooms;

    
    // Değerleri ver
    json_object_object_get_ex(parsed, "srvName", &server_name);
    json_object_object_get_ex(parsed, "rooms", &rooms);


    // Kullanıcı sayısını al
    int ksayi = num_users(head);


    // Konfigürasyona dğerleri ekle
    strcpy(sconf->srvName, json_object_get_string(server_name));
    sconf->rooms = json_object_get_boolean(rooms);
    sconf->numUsrs = ksayi;


    // JSON Objelerini bırak
    json_object_put(parsed);
    json_object_put(server_name);
    json_object_put(rooms);


    // Başarılı bit
    return 0;

    /* Örnek Konfigürasyon
        {
            "srvName":"Mint",
            "rooms":ture
        }
    */
}


// Hatayı dosyaya kayıt et
void logError(char *text)
{
    // Dosyayı aç
    FILE *log_file = fopen("error.log", "a");
    
    // Hata kontrol
    if (log_file == NULL)
    {
        /* =HATA_DURUMUNDA= */
        printColor(RED, "Log dosyası açılamadı"); // Hatayı ekrana bas
        return; // Durdur
    }

    // Şimdi
    time_t now;
    time(&now); // ata

    // Dosyaya hatayı zaman ile birlikte kayıt et
    fprintf(log_file, "%s - %s", text, ctime(&now));

    // Dosyayı kapat
    fclose(log_file);
}


// Giriş bilgilerini dosyaya kayıt et
void logLogin(char *text, char *uname)
{
    // Dosyayı aç
    FILE *log_file = fopen("error.log", "a");
    
    // Hata kontrol
    if (log_file == NULL)
    {
        /* =HATA_DURUMUNDA= */
        printColor(RED, "Log dosyası açılamadı"); // Hatayı ekrana bas
        return; // Durdur
    }

    // Şimdi
    time_t now;
    time(&now); // ata

    // Dosyaya hatayı zaman ile birlikte kayıt et
    fprintf(log_file, "%s - %s", text, ctime(&now));

    // Dosyayı kapat
    fclose(log_file);
}


// Gelen parolayı hash'le
void hashpasswd(char *pass)
{
	// Çıktı için char değer oluştur
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char hashOutput[SHA256_DIGEST_LENGTH * 2 + 1]; // Hash çıktısı için yeterli alan

	// SHA-256 hash'ini hesapla
	SHA256((unsigned char*)pass, strlen(pass), hash);
    
	// Hash değerini hex formatında hashOutput'a yaz
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(hashOutput + (i * 2), "%02x", hash[i]);
	}

    // Hash değerini pass değişkenine kopyala
    strcpy(pass, hashOutput);
}

