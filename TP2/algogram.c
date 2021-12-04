#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "heap.h"
#include "algogram.h"
// #include "post.h"
#include "vd.h"  // (¡NUEVO!) Vector Dinámico V2 (¡ahora con voids asteriscos!)


// No es un TDA
typedef struct red_social {
    hash_t* funciones;
    size_t logueado;
    vd_t* usuarios;
    hash_t* usuarios_feed;
    vd_t* posts;
    size_t posts_contador;
} algogram_s;


// No es un TDA
typedef struct post {
    size_t autor;
    char* comentario;
    int fecha_creacion;
    size_t cant_likes;      // Algogram todavia no tiene dislikes (?
    vd_t* likers;        // O para los amigos, "personas que dieron me gusta"
} post_s;


void logout() {};
void post_ver_likes() {};
void post_likear() {};
void post_ver_proximo() {};
void post_publicar() {};
void login() {};


void algogram_ingresar_comandos(algogram_s* red) {
    //...
    return;
}


algogram_s* algogram_cargar_usuarios(FILE* usuarios) {

    algogram_s* red = malloc(sizeof(algogram_s));
    if (!red) return NULL;

    size_t n = 16;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, usuarios) != EOF) {
        //printf("%s\n", buffer);
    }

    free(buffer);

    return red;
}


void algogram(FILE* usuarios) {
    algogram_s* red = algogram_cargar_usuarios(usuarios);
    if (!red) return;

    while (true) {
        algogram_ingresar_comandos(red);
    }
}
