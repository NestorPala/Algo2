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
#define NADIE_LOGUEADO -1


typedef struct red_social {
    int logueado;
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


char* entrada_usuario() {
    char* buffer;
    size_t buf_tam = 32;
    buffer = (char *)malloc(buf_tam * sizeof(char));

    printf("\nHOLA: "); //debug
    ssize_t input = getline(&buffer, &buf_tam, stdin);
    printf("\nINGRESADO: %s", buffer);  //debug

    return buffer;
}


void logout(algogram_s* red) {
    printf("\nLOGOUT"); //debug
}


void post_ver_likes(algogram_s* red) {
    printf("\nPOST VER LIKES"); //debug
}


void post_likear(algogram_s* red) {
    printf("\nPOST LIKEAR"); //debug
}


void post_ver_siguiente(algogram_s* red) {
    printf("\nPOST VER SIGUIENTE"); //debug
}


void post_publicar(algogram_s* red) {
    printf("\nPOST PUBLICAR"); //debug
}


void login(algogram_s* red) {
    printf("\nLOGIN"); //debug
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

    char* string = entrada_usuario();

    if (!es_comando(string)) {
        printf("Comando inválido.");
        return;
    }
    
    ejecutar_comando(string, red);
    free(string);
}


vd_t* algogram_cargar_usuarios(FILE* archivo_usuarios) {

    vd_t* lista_usuarios = vd_crear(5);

    if (!lista_usuarios) {
        return NULL;
    }

    size_t n = 16, i = 0;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, archivo_usuarios) != EOF) {
        if (i == vd_largo(lista_usuarios)) {
            vd_redimensionar(lista_usuarios, 2*i);
        }

        printf("USUARIO A GUARDAR: %s\n", buffer); //debug
        vd_guardar(lista_usuarios, i, buffer);

        i++;
    }

    free(buffer);

    return lista_usuarios;
}


algogram_s* algogram_crear(FILE* usuarios) {
    algogram_s* algogram = malloc(sizeof(algogram_s));

    if (!algogram) {
        return NULL;
    }

    algogram->logueado = NADIE_LOGUEADO;
    algogram->usuarios = algogram_cargar_usuarios(usuarios);

    // hash_t* usuarios_feed = hash_crear(heap_destruir);
    // if (!usuarios_feed) {
    //     free(algogram);
    //     return NULL;
    // }
    // algogram->usuarios_feed = usuarios_feed;

    for (size_t i=0; i<vd_largo(algogram->usuarios); i++) {
        void* usuario = vd_obtener(algogram->usuarios, i, NULL);
        usuario ? printf("%s\t", (char*)vd_obtener(algogram->usuarios, i, NULL)) : printf("NULL\t");
    }

    algogram->posts = NULL;
    algogram->posts_contador = 0;

    return algogram;
}


void algogram(FILE* usuarios) {
    algogram_s* red = algogram_crear(usuarios);
    if (!red) return;

    while (true) {
        algogram_ingresar_comandos(red);
    }
}
