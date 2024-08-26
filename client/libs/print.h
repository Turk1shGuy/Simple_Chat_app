// Renkler için enum yapısı
typedef enum
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN
} color_t;


// Renkli Çıktı vermek için
void printColor(color_t color, char *text);