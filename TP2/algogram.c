#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "string.h" 
#include "hash.h"
#include "heap.h"
#include "algogram.h"
#include "vd.h" 


#define LOGIN "login\n"
#define LOGOUT "logout\n"
#define PUBLICAR_POST "publicar\n"
#define VER_SIGUIENTE_FEED "ver_siguiente_feed\n"
#define LIKEAR_POST "likear_post\n"
#define MOSTRAR_LIKES "mostrar_likes\n"


typedef struct red_social {
    size_t logueado;
    vd_t* usuarios;
    hash_t* usuarios_feed;
    vd_t* posts;
    size_t posts_contador;
} algogram_s;


typedef struct post {
    size_t autor;
    char* comentario;
    int fecha_creacion;
    size_t cant_likes; 
    vd_t* likers;        
} post_s;


void logout(algogram_s* red) {
    printf("\nLOGOUT");
}


void post_ver_likes(algogram_s* red) {
    printf("\nPOST VER LIKES");
}


void post_likear(algogram_s* red) {
    printf("\nPOST LIKEAR");
}


void post_ver_siguiente(algogram_s* red) {
    printf("\nPOST VER SIGUIENTE");
}


void post_publicar(algogram_s* red) {
    printf("\nPOST PUBLICAR");
}


void login(algogram_s* red) {
    printf("\nLOGIN");
}


void ejecutar_comando(char* comando, algogram_s* red) {
         if (strcmp(comando, LOGIN)              == 0)  login(red);
    else if (strcmp(comando, LOGOUT)             == 0)  logout(red);
    else if (strcmp(comando, PUBLICAR_POST)      == 0)  post_publicar(red);
    else if (strcmp(comando, VER_SIGUIENTE_FEED) == 0)  post_ver_siguiente(red);
    else if (strcmp(comando, LIKEAR_POST)        == 0)  post_likear(red);
    else if (strcmp(comando, MOSTRAR_LIKES)      == 0)  post_ver_likes(red);
}


bool es_comando(char* string) {
    return     strcmp(string, LOGIN)               == 0
            || strcmp(string, LOGOUT)              == 0
            || strcmp(string, PUBLICAR_POST)       == 0
            || strcmp(string, VER_SIGUIENTE_FEED)  == 0
            || strcmp(string, LIKEAR_POST)         == 0
            || strcmp(string, MOSTRAR_LIKES)       == 0;
}


void algogram_ingresar_comandos(algogram_s* red) {

    char* buffer;
    size_t buf_tam = 32;
    buffer = (char *)malloc(buf_tam * sizeof(char));

    printf("\nHOLA: "); //debug
    getline(&buffer, &buf_tam, stdin);
    printf("\nINGRESADO: %s", buffer);  //debug

    if (!es_comando(buffer)) {
        printf("Comando inválido.");
        return;
    }
    
    ejecutar_comando(buffer, red);
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
