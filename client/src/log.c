#include "../inc/log.h"
#include "../inc/global.h"
#include "../libs/print.h"

// Hatayı kayıt et
void logError(char *text)
{
    // Dosyayı aç
    FILE *fl = fopen(".error.log", "a");
    if (fl == NULL) // Hata Kontrol
    {
        // Hatayı bas
        printColor(RED, "Hata: .error.log Dosyası açılamadı\n");

        // Başarısız çık
        return;
    }

    // Dosya yazdır
    fprintf(fl, "%s\n", text);

    // Tamamdır
}