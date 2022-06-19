#include <stdio.h>


const size_t CANT_MAX_LETRAS_ENG = 26;


void imprimir_arreglo_cadenas(char* cadenas[], size_t n) {
    printf("\n[");

    for (size_t i=0; i<n; i++) {
        if (i < n-1) {
            printf("%s, ", cadenas[i]);
        } else {
            printf("%s", cadenas[i]);
        }
    }

    printf("]\n");
}


// ASCII: 'A' = 65, 'B' = 66, ..., 'Z' = 90
int letra_a_numero(char letra) {
    return (int)letra - 65;
}


// COUNTING SORT
// Pre: el índice "digito" tiene que estar dentro del rango.
void cadenas_counting_sort(char* cadenas[], size_t n, size_t digito) {

    // Orden: O(N + K), con  N = cantidad de cadenas  y  K = cantidad de letras
    size_t k = CANT_MAX_LETRAS_ENG;
    
    // Índices 0 - 25: letras 'A' - 'Z' 
    int frecuencias[k]; 

    int acu[k];
    char* ordenados[n];

    // Inicializamos los arreglos en 0
    for (size_t i=0; i<k; i++) {
        frecuencias[i] = 0;
        acu[i] = 0;
    }

    // Guardo las frecuencias de las letras
    for (size_t i=0; i<n; i++) {
        size_t x = letra_a_numero(cadenas[i][digito]);
        
        frecuencias[x] += 1;
    }

    // Guardo las frecuencias acumuladas
    for (size_t i=0; i<k; i++) {
        if (i+1 < k) {
            acu[i + 1] = acu[i] + frecuencias[i];
        }
    }

    // Realizo el ordenamiento
    for (size_t i=0; i<n; i++) {
        size_t letra = letra_a_numero(cadenas[i][digito]);
        size_t x = acu[letra];

        ordenados[x] = cadenas[i];
        acu[letra]++;
    }

    // Traslado los resultados al arreglo original
    for (size_t i=0; i<n; i++) {
        cadenas[i] = ordenados[i];
    }
}


// RADIX SORT: Aplico counting-sort por cada dígito
void ordenar_cadenas(char* cadenas[], size_t n, size_t cant_digitos) {

    // Bucle descendente
    for (size_t i=0; i<cant_digitos; i++) {
        cadenas_counting_sort(cadenas, n, cant_digitos - 1 - i);
    }
}


// C99
int main() {
    char* aeropuertos[] = {"LAS", "EZE", "MDQ", "WAW", "JFK", "LAX", "ERE", "FRA", "MDP", "MAX"};
    size_t n = 10;
    size_t cant_digitos = 3;

    // Antes: [LAS, EZE, MDQ, WAW, JFK, LAX, ERE, FRA, MDP, MAX]
    imprimir_arreglo_cadenas(aeropuertos, n);

    ordenar_cadenas(aeropuertos, n, cant_digitos);

    // Después: [ERE, EZE, FRA, JFK, LAS, LAX, MAX, MDP, MDQ, WAW]
    imprimir_arreglo_cadenas(aeropuertos, n);

    return 0;
}
