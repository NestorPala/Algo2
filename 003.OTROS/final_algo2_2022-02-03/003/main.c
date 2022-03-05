#include <stdio.h>
#include "003_hash_con_iterador_interno.h"


bool sumar(const char* clave, void* dato, void* extra) {
    *(int*)extra += *(int*)dato;
    return true;
}


int main() {

    hash_t* hash = hash_crear(NULL);
    int num1 = 1, num2 = 2, num3 = 3;
    int resultado = 0;

    hash_guardar(hash, "pepe", &num1);
    hash_guardar(hash, "juan", &num2);
    hash_guardar(hash, "carlos", &num3);

    hash_iterar(hash, sumar, &resultado);

    printf("EL RESULTADO DE LA SUMA ES: %d\n", resultado);

    hash_destruir(hash);

    return 0;
}
