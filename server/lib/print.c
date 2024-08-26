// Başlık dosyasını ekle
#include "../lib/print.h"
#include <stdio.h>

void printColor(color_t color, char *text)
{
    switch (color)
    {
        case RED: // Kırmızı
            printf("\033[31m%s\033[0m\n", text);
            break;

        case GREEN: // Yeşil
            printf("\033[32m%s\033[0m\n", text);
            break;
        
        case BLUE: // Mavi
            printf("\033[34m%s\033[0m\n", text);
            break;
        
        case YELLOW: // Sarı
            printf("\033[33m%s\033[0m\n", text);
            break;
        
        case MAGENTA: // Mor
            printf("\033[35m%s\033[0m\n", text);
            break;
        
        case CYAN: // Açık Mavi
            printf("\033[36m%s\033[0m\n", text);
            break;
        
        default:
            printf("%s", text); // Normal baskı
    }
}