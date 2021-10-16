#include "lista.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdio.h> //printf()
#include <stdlib.h> //rand()
#include <time.h> //srand()


/*
COMO HACER LAS PRUEBAS (borrar esto luego)
----------------------

Considerar que todas las primitivas (exceptuando lista_destruir y lista_iterar) deben funcionar en tiempo constante.

Las pruebas deben incluir los casos básicos de TDA similares a los contemplados para la pila y la cola, 
y adicionalmente debe verificar los siguientes casos del iterador externo:

    -  Al insertar un elemento en la posición en la que se crea el iterador, efectivamente se inserta al principio.
    -  Insertar un elemento cuando el iterador está al final efectivamente es equivalente a insertar al final.
    -  Insertar un elemento en el medio se hace en la posición correcta.
    -  Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista.
    -  Remover el último elemento con el iterador cambia el último de la lista.
    -  Verificar que al remover un elemento del medio, este no está.
    -  Otros casos borde que pueden encontrarse al utilizar el iterador externo. Y los casos con / sin corte del iterador interno.
*/


// Cantidad masiva de pruebas
const size_t N = 1000;


void pruebas_lista_estudiante(void) {
    printf("\n-----------------------------------------------------------------------------------\n\n");
    printf("PRUEBA DE TODAS LAS PRIMITIVAS DEL TDA LISTA");
    printf("\n\n-----------------------------------------------------------------------------------\n");
    
    //prueba_crear_lista()
    //pruebas_unitarias_lista()
    //pruebas_volumen_lista()
    //prueba_destruir_lista()

    //(Otras pruebas...)
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {

    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
