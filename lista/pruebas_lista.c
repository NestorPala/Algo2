#include "lista.h"
#include "testing.h"
#include <stddef.h>  //NULL
#include <stdio.h> //printf()
#include <stdlib.h> //rand()
#include <time.h> //srand()
#include <string.h> //strcmp()


// Cantidad masiva de pruebas
const size_t N = 1000;


int toInt(void *num){

  return *(int *)num;
}


void destruir_dato(void *dato)
{
  free(dato);
}


bool free_imprimir_un_item(void *elemento, void *extra)
{
  int *contador = extra;
  printf("liberados: %d\n", ++(*contador));
  free(elemento);

  return true; // seguir iterando
}


bool imprimir_un_item(void *elemento, void *extra)
{
  // Sabemos que ‘extra’ es un entero, por tanto le podemos hacer un cast.
  int *contador = extra;
  printf("%d. %s\n", ++(*contador), (char *)elemento);

  return true; // seguir iterando
}


void imprimir_iter_interno(lista_t *lista)
{
  int num_items = 0;
  lista_iterar(lista, imprimir_un_item, &num_items);
  printf("Tengo que comprar %d ítems\n", num_items);
}


void imprimir_iter_externo(lista_t *lista)
{
  lista_iter_t *iter = lista_iter_crear(lista);
  int num_items = 0;

  while (!lista_iter_al_final(iter))
  {
    char *elemento = lista_iter_ver_actual(iter);
    printf("%d. %s\n", ++num_items, elemento);

    lista_iter_avanzar(iter);
  }
  printf("Tengo que comprar %d ítems\n", num_items);
  lista_iter_destruir(iter);
}


void pruebas_unitarias_lista() {

    printf("\n==========PRUEBAS UNITARIAS DE LISTA==========\n\n");

    lista_t* lista = lista_crear();
    int numeros[] = {23, 0, -17};

    print_test("Insertando el primer numero al principio >>>  ", 
        lista_insertar_primero(lista, &numeros[0]));

    print_test("El largo de la lista es 1  >>>  ", 
        lista_largo(lista) == 1);

    print_test("El primer numero de la lista es igual que el ingresado >>>  ", 
        *(int*)lista_ver_primero(lista) == numeros[0]);

    print_test("El último número de la lista es igual al primero  >>>  ", 
        *(int*)lista_ver_primero(lista) == *(int*)lista_ver_ultimo(lista));

    print_test("Insertando el segundo numero al principio  >>>  ", 
        lista_insertar_primero(lista, &numeros[1]));

    print_test("El largo de la lista es 2  >>>  ", 
        lista_largo(lista) == 2);

    print_test("El segundo numero de la lista es igual que el ingresado  >>>  ", 
        *(int*)lista_ver_primero(lista) == numeros[1]);

    print_test("El último número de la lista NO es igual al primero  >>>  ", 
        *(int*)lista_ver_primero(lista) != *(int*)lista_ver_ultimo(lista));

    print_test("Insertando NULL como tercer elemento  >>>  ", 
        lista_insertar_primero(lista, NULL));

    print_test("El primer elemento de la lista es NULL  >>>  ", 
        lista_ver_primero(lista) == NULL);

    lista_destruir(lista, NULL);

    printf("\n-----------------------------------------------------------------------------\n");
}


void prueba_crear_lista() {

    printf("\n==========PRUEBA CREAR LISTA==========\n\n");

    lista_t* lista = lista_crear();

    print_test("La lista está vacía  >>>  ", lista_esta_vacia(lista));
    print_test("El largo de la lista es cero  >>>  ", lista_largo(lista) == 0);
    print_test("No se pueden borrar elementos en una lista vacía  >>>  ", !lista_borrar_primero(lista));

    lista_destruir(lista, NULL);

    printf("\n-----------------------------------------------------------------------------\n");
}

void pruebas_volumen_lista()
{
  printf("\n==========PRUEBAS DE VOLUMEN LISTA==========\n");
  lista_t *lista = lista_crear();

  int size = 1000;
  printf("Prueba insertar %d elementos al comienzo\n", size);
  bool lista_insertar_ok = true;
  for (int i = 0; i < size && lista_insertar_ok; i++)
  {
    lista_insertar_ok = lista_insertar_primero(lista, &i);
  }
  print_test("Se insertaron bien al principio", lista_insertar_ok == true);
  print_test("Cantidad al final == 1000", lista_largo(lista) == size);

  lista_destruir(lista, NULL);

  printf("Prueba insertar %d elementos al final\n", size);
  lista_t *lista2 = lista_crear();
  bool lista_insertar_ok2 = true;
  for (int i = 0; i < size && lista_insertar_ok2; i++)
  {
    lista_insertar_ok2 = lista_insertar_ultimo(lista2, &i);
  }
  print_test("Se insertaron bien al final", lista_insertar_ok2 == true);
  print_test("Cantidad al final == 1000", lista_largo(lista2) == size);
  lista_destruir(lista2, NULL);

  printf("Prueba insertar %d elementos alocando memoria\n", size);
  lista_t *lista3 = lista_crear();
  bool lista_inserta_ok3 = true;
  for (int i = 0; i < size && lista_inserta_ok3; i++)
  {
    int *vector = malloc(20 * sizeof(int));
    lista_inserta_ok3 = lista_insertar_ultimo(lista3, vector);
  }
  print_test("Se insertaron bien al final", lista_inserta_ok3 == true);
  print_test("Cantidad al final == 1000", lista_largo(lista3) == size);
  printf("Destruir datos con función propia\n");
  lista_destruir(lista3, destruir_dato);
}

bool _print_item(void *item, void *extra)
{
  int *cont = extra;
  cont++;
  printf("%s-", (char *)item);
  return true;
}

void pruebas_generales_iterador_interno()
{
  printf("\n==========INICIO DE PRUEBAS ITERADOR INTERNO LISTA==========\n");
  lista_t *lista = lista_crear();

  printf("Itero con la lista vacía\n");
  imprimir_iter_interno(lista);

  print_test("Se insertaron bien al final", lista_insertar_ultimo(lista, "leche") &&
      lista_insertar_ultimo(lista, "huevos") &&
      lista_insertar_ultimo(lista, "pan") &&
      lista_insertar_ultimo(lista, "mermelada"));
  imprimir_iter_interno(lista);
  lista_destruir(lista, NULL);

  lista_t *lista2 = lista_crear();
  int *vector = malloc(20 * sizeof(int));
  lista_insertar_primero(lista2, vector);
  int num_items = 0;
  lista_iterar(lista2, free_imprimir_un_item, &num_items);
  print_test("Numero de elementos eliminados == 1", num_items == 1);
  lista_destruir(lista2, NULL);
}

void pruebas_generales_iterador_externo()
{
  printf("\n==========INICIO DE PRUEBAS ITERADOR EXTERNO LISTA==========\n");
  lista_t *lista = lista_crear();

  printf("Itero con la lista vacía\n");
  imprimir_iter_interno(lista);

  print_test("Se insertaron bien al final", lista_insertar_ultimo(lista, "leche") &&
      lista_insertar_ultimo(lista, "huevos") &&
      lista_insertar_ultimo(lista, "pan") &&
      lista_insertar_ultimo(lista, "mermelada"));
  imprimir_iter_interno(lista);


  lista_iter_t *iter = lista_iter_crear(lista);
  char item1[4] = "foca";

  // Avanzo hasta el final (NULL) para insertar "foca" a lo último
  for (size_t i=0; i<lista_largo(lista); i++) {
    //printf("\n%s\n", (char*)lista_iter_ver_actual(iter));
    lista_iter_avanzar(iter);
  }

  print_test("Se insertaron bien al final", lista_iter_insertar(iter, item1));
  print_test("Ultimo == foca", lista_ver_ultimo(lista) == item1);
  print_test("Elemento actual == foca", strcmp((char*)lista_iter_ver_actual(iter), "foca") == 0);
  print_test("Elemento borrado == foca", lista_iter_borrar(iter) == item1);
  print_test("Primero != foca", lista_ver_primero(lista) != item1);
  lista_iter_t *iter2 = lista_iter_crear(lista);
  print_test("Primero != foca", lista_iter_ver_actual(iter2));
  lista_iter_destruir(iter);
  lista_iter_destruir(iter2);
  lista_destruir(lista, NULL);


}

void prueba_iterador_externo_hasta_final()
{
  lista_t *lista = lista_crear();

  int num = 1;
  print_test("Se insertaron bien al final", lista_insertar_ultimo(lista, &num) &&
    lista_insertar_ultimo(lista, &num) &&
    lista_insertar_ultimo(lista, &num) &&
    lista_insertar_ultimo(lista, &num));
  lista_iter_t *iterador_externo = lista_iter_crear(lista);
  while (!lista_iter_al_final(iterador_externo))
  {
    printf("%d\n", toInt(lista_iter_ver_actual(iterador_externo)));
    lista_iter_avanzar(iterador_externo);
  }

  print_test("Ver actual == NULL", lista_iter_ver_actual(iterador_externo) == NULL);
  lista_iter_destruir(iterador_externo);
  lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante(void) {
    printf("\n-----------------------------------------------------------------------------------\n\n");
    printf("PRUEBA DE TODAS LAS PRIMITIVAS DEL TDA LISTA");
    printf("\n\n-----------------------------------------------------------------------------------\n");
    
    prueba_crear_lista();
    pruebas_unitarias_lista();
    pruebas_volumen_lista();
    pruebas_generales_iterador_interno();
    pruebas_generales_iterador_externo();
    prueba_iterador_externo_hasta_final();
    //prueba_destruir_lista()

    //(Otras pruebas...)

    // PROBAR:
    // * ORDEN DE INSERCIÓN CON ITERADOR EXTERNO EN INSERTAR PRIMERO
    // * ERROR DESPUÉS DE BORRAR PRIMERO CON ITERADOR EXTERNO
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
