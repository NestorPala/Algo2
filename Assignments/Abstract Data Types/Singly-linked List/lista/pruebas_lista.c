#include "lista.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdlib.h> //rand()
#include <stdio.h> //printf()
#include <time.h> //srand()


// Cantidad masiva de pruebas
const size_t N = 1000;


void pruebas_lista_vacia() {
    printf("---------------------------------------------/PRUEBAS LISTA VACIA/---------------------------------------------\n");
    lista_t* lista = lista_crear();
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("No se puede ver el primer elemento", !lista_ver_primero(lista));
    print_test("No se puede ver el ultimo elemento", !lista_ver_ultimo(lista));
    print_test("El largo de la lista es cero", lista_largo(lista) == 0);
    print_test("No se puede borrar el primer elemento", !lista_borrar_primero(lista));
    lista_destruir(lista, NULL);
}


void pruebas_unitarias_lista_crear(lista_t* lista, int* x[]) {
    printf("---------------------------------------------/PRUEBAS UNITARIAS LISTA - CREAR/---------------------------------------------\n");

    print_test("Se pudo insertar el primer elemento en la lista al final", lista_insertar_ultimo(lista, x[0]));
    print_test("El primer elemento insertado es el correcto", *(int*)lista_ver_ultimo(lista) == *x[0]);
    print_test("El tamanio de la lista es 1", lista_largo(lista) == 1);

    print_test("Se pudo insertar el segundo elemento en la lista al final", lista_insertar_ultimo(lista, x[1]));
    print_test("El segundo elemento insertado es el correcto", *(int*)lista_ver_ultimo(lista) == *x[1]);
    print_test("El tamanio de la lista es 2", lista_largo(lista) == 2);

    print_test("Se pudo insertar el tercer elemento en la lista al final", lista_insertar_ultimo(lista, x[2]));
    print_test("El tercer elemento insertado es el correcto", *(int*)lista_ver_ultimo(lista) == *x[2]);
    print_test("El tamanio de la lista es 3", lista_largo(lista) == 3);
}


void pruebas_unitarias_lista_borrar(lista_t* lista, int* x[]) {
    printf("---------------------------------------------/PRUEBAS UNITARIAS LISTA - BORRAR/---------------------------------------------\n");
    
    print_test("Se pudo borrar el primer elemento de la lista", *(int*)lista_borrar_primero(lista) == *x[0]);
    print_test("El tamanio de la lista es 1", lista_largo(lista) == 2);

    print_test("Se pudo borrar el segundo elemento de la lista", *(int*)lista_borrar_primero(lista) == *x[1]);
    print_test("El tamanio de la lista es 1", lista_largo(lista) == 1);

    print_test("Se pudo borrar el tercer elemento de la lista", *(int*)lista_borrar_primero(lista) == *x[2]);
    print_test("El tamanio de la lista es 1", lista_largo(lista) == 0);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
}


void pruebas_unitarias_lista() {

    lista_t* lista = lista_crear();

    int num[] = {12,32,44};
    int* x[] = {&num[0], &num[1], &num[2]};

    pruebas_unitarias_lista_crear(lista, x);
    pruebas_unitarias_lista_borrar(lista, x);

    lista_destruir(lista, NULL);
}


int* obtener_numeros(size_t n) {
    int* numeros = malloc(n * sizeof(int));

    for (size_t i = 0; i < n; i++) {
        numeros[i] = rand() % 100;
    }
    
    return numeros;
}


int** obtener_numeros_punteros(int* numeros, size_t n) {
    int** numeros_punteros = malloc(n * sizeof(int*));

    for (size_t i = 0; i < n; i++) {
        numeros_punteros[i] = &numeros[i];
    }

    return numeros_punteros;
}


void pruebas_volumen_lista() {
    printf("---------------------------------------------/PRUEBAS DE VOLUMEN LISTA - CREAR E INSERTAR/---------------------------------------------\n");
    lista_t* lista = lista_crear();
    int* num = obtener_numeros(N);
    int** x = obtener_numeros_punteros(num, N);

    bool insertados = true, largo = true, correctos = true;

    for (size_t i=0; i<N; i++) {
        if (lista_largo(lista) != i) {
            largo = false;
        }
        if (!lista_insertar_ultimo(lista, x[i])) {
            insertados = false;
        }
        if (lista_ver_ultimo(lista) != x[i]) {
            correctos = false;
        }
    }

    print_test("Se pudieron insertar con exito 1000 elementos", insertados);
    print_test("Las 1000 veces el tamanio fue correcto", largo);
    print_test("Los 1000 elementos insertados eran los correctos", correctos);

    printf("---------------------------------------------/PRUEBAS DE VOLUMEN LISTA - BORRAR/---------------------------------------------\n");

    bool borrados = true, largo_borrar = true;

    for (size_t i = 0; i < N; i++) {
        if (*(int*)lista_borrar_primero(lista) != num[i]){
            borrados = false;
        }
        if (lista_largo(lista) != N-(i+1)) {
            largo_borrar = false;
        }
    }

    print_test("Se pudieron borrar con exito los 1000 elementos", borrados);
    print_test("El largo se actualizo las 1000 veces bien", largo_borrar);
    print_test("La lista esta vacia", lista_esta_vacia(lista));

    free(x);
    free(num);
    lista_destruir(lista, NULL);
}


bool sumar(void* dato, void* extra) {
    if (!dato) return true;  //no suma elementos NULL

    *(int*)extra += *(int*)dato;

    return true;
}


void pruebas_iterador_interno() {
    printf("---------------------------------------------/PRUEBAS DE ITERADOR INTERNO LISTA/---------------------------------------------\n");
    lista_t* lista = lista_crear();

    int num[] = {25, 26, 27, 28};
    int* x[] = {&num[0], &num[1], &num[2], &num[3]};

    lista_insertar_ultimo(lista, x[0]);
    lista_insertar_ultimo(lista, x[1]);
    lista_insertar_ultimo(lista, x[2]);
    lista_insertar_ultimo(lista, x[3]);

    int suma = 0;
    lista_iterar(lista, sumar, &suma);

    print_test("Se pudo iterar la lista utilizando una funcion visitar de suma", suma == num[0] + num[1] + num[2] + num[3]);

    lista_destruir(lista, NULL);
}


void lista_llenar(lista_t* lista, int** numeros, size_t n) {
    for (size_t i = 0; i < n; i++) {
        lista_insertar_ultimo(lista, numeros[i]);
    }
}


void pruebas_iterador_externo_en_medio() {
    printf("---------------------------------------------/PRUEBAS DE ITERADOR EXTERNO LISTA - MEDIO LISTA/---------------------------------------------\n");
    lista_t* lista = lista_crear();
    int* num = obtener_numeros(N);
    int** x = obtener_numeros_punteros(num, N);
    lista_llenar(lista, x, N);
    lista_iter_t* iter = lista_iter_crear(lista);

    size_t pos_arbitraria = (2/3) * N;
    int i = 0;
    int w = 666;

    while (!lista_iter_al_final(iter)) {
        
        if (i == pos_arbitraria) {
            lista_iter_insertar(iter, &w);
            break;
        }
        lista_iter_avanzar(iter);
        i++;
    }
    
    print_test("El elemento se inserto en el medio de la lista correctamente", lista_iter_ver_actual(iter) == &w);
    print_test("El elemento se ha borrado. Es el correcto.", lista_iter_borrar(iter) == &w);

    lista_iter_destruir(iter);

    lista_iter_t* iter2 = lista_iter_crear(lista);
    i = -1;

    while (!lista_iter_al_final(iter2)) {
        i++;
        if (i == pos_arbitraria) {
            print_test("El elemento efectivamente ha sido borrado de la lista", lista_iter_ver_actual(iter2) != &w);
            break;
        }
        lista_iter_avanzar(iter2);
    }

    lista_iter_destruir(iter2);
    free(x);
    free(num);
    lista_destruir(lista, NULL);
}


void pruebas_iterador_externo_al_final() {
    printf("---------------------------------------------/PRUEBAS DE ITERADOR EXTERNO LISTA - FINAL LISTA/---------------------------------------------\n");
    lista_t* lista = lista_crear();
    int* num = obtener_numeros(N);
    int** x = obtener_numeros_punteros(num, N);
    lista_llenar(lista, x, N);
    lista_iter_t* iter = lista_iter_crear(lista);

    while(!lista_iter_al_final(iter)) {
        lista_iter_avanzar(iter);
    }

    print_test("No se puede avanzar un iterador al final de la lista", !lista_iter_avanzar(iter));

    int a = 8214;
    void* viejo_ultimo = lista_ver_ultimo(lista);
    print_test("Se puede insertar un elemento al final usando el iterador", lista_iter_insertar(iter, &a));
    print_test("El elemento insertado efectivamente esta al final de la lista", lista_iter_ver_actual(iter) == lista_ver_ultimo(lista));
    print_test("Se puede borrar el ultimo elemento de la lista usando iterador", lista_iter_borrar(iter) == &a);
    print_test("El ultimo elemento de la lista volvio a ser el de antes", lista_ver_ultimo(lista) == viejo_ultimo);



    lista_iter_destruir(iter);
    free(x);
    free(num);
    lista_destruir(lista, NULL);
}


void pruebas_iterador_externo_al_inicio() {
    printf("---------------------------------------------/PRUEBAS DE ITERADOR EXTERNO LISTA - PRINCIPIO LISTA/---------------------------------------------\n");
    lista_t* lista = lista_crear();
    int* num = obtener_numeros(N);
    int** x = obtener_numeros_punteros(num, N);
    lista_llenar(lista, x, N);
    lista_iter_t* iter = lista_iter_crear(lista);

    print_test("El iterador apunta al primer elemento de la lista", lista_iter_ver_actual(iter) == lista_ver_primero(lista));

    int valor1 = -12877;

    print_test("Se pudo insertar corremkjkjctamente un elemento en la lista", lista_iter_insertar(iter, &valor1));
    print_test("Se pudo insertar un elemento en la primera posicion de la lista", *(int*)lista_iter_ver_actual(iter) == valor1);
    print_test("Se pudo borrar correctamente el primer elemento de la lista", *(int*)lista_iter_borrar(iter) == valor1);
    print_test("El primer elemento de la lista es el original nuevamente", *(int*)lista_iter_ver_actual(iter) == num[0]);

    bool iterado_bien = true;
    int i = -1;

    while(!lista_iter_al_final(iter)) {
        i++;
        if (lista_iter_ver_actual(iter) != x[i]) {
            iterado_bien = false;
        }
        lista_iter_avanzar(iter);
    }

    print_test("Todos los elementos se iteran bien", iterado_bien);
    
    lista_iter_destruir(iter);
    free(x);
    free(num);
    lista_destruir(lista, NULL);
}


void lista_destruir_2(void* lista) {
    lista_destruir(lista, NULL);
}


void pruebas_destruir_lista() {
    lista_t* lista1 = lista_crear();
    lista_t* lista2 = lista_crear();

    int* num = obtener_numeros(3);
    int** x = obtener_numeros_punteros(num, 3);

    lista_insertar_ultimo(lista1, x[0]);
    lista_insertar_ultimo(lista1, x[1]);
    lista_insertar_ultimo(lista1, x[2]);

    lista_insertar_ultimo(lista2, x[0]);
    lista_insertar_ultimo(lista2, x[1]);
    lista_insertar_ultimo(lista2, x[2]);

    lista_t* lista = lista_crear();

    lista_insertar_ultimo(lista, lista1);
    lista_insertar_ultimo(lista, lista2);

    free(x);
    free(num);
    lista_destruir(lista, lista_destruir_2);
}


void pruebas_lista_estudiante() {
    pruebas_lista_vacia();
    pruebas_unitarias_lista();
    pruebas_volumen_lista();
    pruebas_iterador_interno();
    pruebas_iterador_externo_al_inicio();
    pruebas_iterador_externo_al_final();
    pruebas_iterador_externo_en_medio();
    pruebas_destruir_lista();
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
