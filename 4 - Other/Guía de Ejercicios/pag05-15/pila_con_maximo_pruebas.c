#include <stdio.h>
#include "pila_con_maximo.h"


void ver_maximo(pila_con_maximo_t* pila) {

    int maximo_pila = 0;

    if (pila_ver_maximo(pila, &maximo_pila)) {
        printf("El máximo de la pila es: %d\n", maximo_pila);
    } else {
        printf("La pila no existe o está vacía.\n");
    }
}


void ver_tope(pila_con_maximo_t* pila) {

    int tope_pila = 0;

    if (pila_ver_tope(pila, &tope_pila)) {
        printf("El tope de la pila es: %d\n", tope_pila);
    } else {
        printf("La pila no existe o está vacía.\n");
    }
}


void borrar_elemento(pila_con_maximo_t* pila) {

    int tope_pila = 0;

    if (pila_desapilar(pila, &tope_pila)) {
        printf("Se ha eliminado un elemento de la pila. El número eliminado fue: %d\n", tope_pila);
    } else {
        printf("La pila no existe o está vacía.\n");
    }
}


void agregar_elemento(pila_con_maximo_t* pila) {

    int valor = 0;

    printf("Ingrese el número que desea agregar a la pila:\n");
    scanf("%d", &valor);
    
    if (pila_apilar(pila, valor)) {
        printf("\nSe ha agregado el número %d a la pila.\n", valor);
    } else {
        printf("No se ha podido agregar el número %d a la pila.\n", valor);
    }
}


int main() {

    int estado = 0;
    pila_con_maximo_t* pila = pila_crear();

    while(estado != -1) {
        printf("----------------------- Pila con máximo -----------------------\n");
        printf("1  -  Agregar elemento\n");
        printf("2  -  Borrar elemento\n");
        printf("3  -  Ver tope\n");
        printf("4  -  Ver máximo\n");
        scanf("%d", &estado);

        if (estado == 1) {
            agregar_elemento(pila);
        } else if (estado == 2) {
            borrar_elemento(pila);
        } else if (estado == 3) {
            ver_tope(pila);
        } else if (estado == 4) {
            ver_maximo(pila);
        }
    }

    pila_destruir(pila);

    return 0;
}
