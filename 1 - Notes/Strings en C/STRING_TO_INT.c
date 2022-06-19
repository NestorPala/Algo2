#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_to_string(int num, char** dest) {
    int largo = snprintf( NULL, 0, "%d", num );
    char string[largo + 1];
    snprintf( string, (size_t)(largo + 1), "%d", num );
    strcpy(*dest, string);
}

int main() {

    // Método 1, no pasa valgrind
    int num = 1;
    char* string_num;
    int_to_string(num, &string_num);
    printf("El numero convertido a string es: %s", string_num);

    // Método 2, pasa valgrind
    int a = 123456;
    char pepe[999];
    sprintf(pepe, "%d", a);
    printf("\nEl numero convertido a string es: %s", pepe);

    return 0;
}
