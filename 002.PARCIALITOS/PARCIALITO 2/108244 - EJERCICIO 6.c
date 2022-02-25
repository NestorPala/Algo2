#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "ab.h" //como si fueran las pruebas de RPL :D


/*
    Orden de la primitiva: O(N), siendo N la cantidad de elementos deL árbol. Esto se debe a que tengo que recorrer cada nodo para insertar su correspondiente cantidad de descendientes. Utilizo recorrido post-order, porque la suma de los descendientes se realiza DESPUÉS de las llamadas recursivas. Luego, en cada return cuento la cantidad total de hijos más el "nodo" actual, y este número se va acumulando cada vez que salgo de una llamada recursiva, hasta salir del árbol por completo.

    Recorrido utilizado: post-order
*/


int insertar_desc(ab_t* ab) {

    if (!ab) return 0;

    int hi = insertar_desc(ab->izq);
    int hd = insertar_desc(ab->der);

    ab->valor = hi + hd;  //POST-ORDER

    return hi + hd + 1;
}


bool insertar_descendientes (ab_t* ab) {

    if (!ab) return false;

    if (!ab->izq && !ab->der) {
        ab->valor = 0;
        return true;
    }

    insertar_desc(ab);

    // Haciendo las pruebas en paint (adjunto paint) llegué a la conclusión de que el número final que se devolvería es la cantidad de elementos dentro del árbol. ¡Vaya dato perturbador! (Pero predecible).

    return true;
}
