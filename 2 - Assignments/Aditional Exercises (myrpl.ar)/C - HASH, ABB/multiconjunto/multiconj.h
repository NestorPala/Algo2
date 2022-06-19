#ifndef MULTICONJ_H
#define MULTICONJ_H

typedef struct multiconj multiconj_t;

/* Crea un multiconjunto. Devuelve NULL en caso de error al crear */
multiconj_t* multiconj_crear();

/* guarda un elemento en el multiconjunto. Devuelve true si se pudo 
guardar el elemento correctamente, false en caso contrario. */
bool multiconj_guardar(multiconj_t* multiconj, const char* elem);

/* devuelve true si el elemento aparece al menos una vez en el conjunto. */
bool multiconj_pertenece(const multiconj_t* multiconj, const char* elem);

/* elimina una aparición del elemento dentro del conjunto. Devuelve true 
si se eliminó una aparición del elemento. */
bool multiconj_borrar(multiconj_t* multiconj, char* elem);

/* destruye el TDA liberando toda la memoria asociada */
void multiconj_destruir(multiconj_t* multiconj);

#endif