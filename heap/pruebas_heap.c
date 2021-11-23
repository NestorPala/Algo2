#include "heap.h"
#include "testing.h"
#include <stdio.h> //printf()
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <string.h> //strcmp()
#include <stdbool.h>

/***********************************************************************************/

// Cantidad masiva de pruebas
const size_t N = 2000;

/***********************************************************************************/




//...





/***********************************************************************************/


void pruebas_heap_estudiante() {
    // pruebas_heap_vacio();
    // pruebas_unitarias();
    // pruebas_volumen();
}


/***********************************************************************************/

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {

    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
