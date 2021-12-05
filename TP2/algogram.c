#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "string.h" 
#include "hash.h"
#include "heap.h"
#include "algogram.h"
#include "vd.h" //vector dinamico
#define LOGIN "login"
#define LOGOUT "logout"
#define PUBLICAR_POST "publicar"
#define VER_SIGUIENTE_FEED "ver_siguiente_feed"
#define LIKEAR_POST "likear_post"
#define MOSTRAR_LIKES "mostrar_likes"
#define NADIE_LOGUEADO " "


typedef struct red_social {
    char* logueado;
    hash_t* usuarios;
    vd_t* posts;
    size_t contador_posts;
} algogram_s;


typedef struct post {
    size_t autor;
    char* contenido;
    int fecha_creacion;
    size_t cant_likes; 
    vd_t* likes;        
} post_s;


char* quitar_barra_n(char* cadena, bool ingresar_usuarios) {
    int i, largo;
    largo = strlen(cadena);
    char* nueva_cadena;
    size_t u = ingresar_usuarios ? 2 : 1;
 
    nueva_cadena = malloc(largo - u);
 
    for(i = 0; i < largo-u; i++){
        nueva_cadena[i] = cadena[i]; 
    }

    nueva_cadena[largo - u] = '\0';
 
    return nueva_cadena;
}


char* entrada_usuario() {
    char* buffer;
    size_t buf_tam = 32;
    buffer = (char *)malloc(buf_tam * sizeof(char));

    //printf("\n>>>>>  "); //debug

    getline(&buffer, &buf_tam, stdin);
    char* nuevo_buffer = quitar_barra_n(buffer, false);

    //printf("\nINGRESADO: %s", nuevo_buffer);  //debug

    return nuevo_buffer;
}


void logout(algogram_s* algogram) {
    printf("\nLOGOUT"); //debug
}


void post_ver_likes(algogram_s* algogram) {
    printf("\nPOST VER LIKES"); //debug
}


void post_likear(algogram_s* algogram) {
    printf("\nPOST LIKEAR"); //debug
}


void post_ver_siguiente(algogram_s* algogram) {
    printf("\nPOST VER SIGUIENTE"); //debug
}


void post_publicar(algogram_s* algogram) {
    printf("\nPOST PUBLICAR"); //debug
}


void login(algogram_s* algogram) {
    printf("\nLOGIN\n"); //debug

    char* cadena = entrada_usuario();

    if (algogram->logueado != NADIE_LOGUEADO) {
        printf("Error: Ya habia un usuario loggeado");
        return;
    }

    if (!hash_pertenece(algogram->usuarios, cadena)) {
        printf("Error: usuario no existente");
        return;
    }

    // if (algogram->logueado) {
    //     free(algogram->logueado);
    // }

    algogram->logueado = strdup(cadena);

    printf("Hola %s\n", algogram->logueado);

    free(cadena);
}


void ejecutar_comando(char* comando, algogram_s* algogram) {
         if (strcmp(comando, LOGIN)              == 0)  login(algogram);
    else if (strcmp(comando, LOGOUT)             == 0)  logout(algogram);
    else if (strcmp(comando, PUBLICAR_POST)      == 0)  post_publicar(algogram);
    else if (strcmp(comando, VER_SIGUIENTE_FEED) == 0)  post_ver_siguiente(algogram);
    else if (strcmp(comando, LIKEAR_POST)        == 0)  post_likear(algogram);
    else if (strcmp(comando, MOSTRAR_LIKES)      == 0)  post_ver_likes(algogram);
    else if (strcmp(comando, "clear")          == 0)  exit(0); //debug
}


bool es_comando(char* cadena) {
    return     strcmp(cadena, LOGIN)               == 0
            || strcmp(cadena, LOGOUT)              == 0
            || strcmp(cadena, PUBLICAR_POST)       == 0
            || strcmp(cadena, VER_SIGUIENTE_FEED)  == 0
            || strcmp(cadena, LIKEAR_POST)         == 0
            || strcmp(cadena, MOSTRAR_LIKES)       == 0
            || strcmp(cadena, "clear")           == 0;  //debug
}


void algogram_ingresar_comandos(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (!es_comando(cadena)) {
        printf("Comando inválido.");
        return;
    }
    
    ejecutar_comando(cadena, algogram);
    free(cadena);
}


void heap_destruir_aux(void* heap) {
    heap_destruir(heap, NULL);
}


hash_t* algogram_cargar_usuarios(FILE* archivo_usuarios) {

    hash_t* usuarios = hash_crear(heap_destruir_aux);
    if (!usuarios) {
        return NULL;
    }

    size_t n = 16;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, archivo_usuarios) != EOF) {
        char* nuevo_buffer = quitar_barra_n(buffer, true);
        hash_guardar(usuarios, nuevo_buffer, NULL);
    }

    free(buffer);

    return usuarios;
}


algogram_s* algogram_crear(FILE* usuarios) {
    algogram_s* algogram = malloc(sizeof(algogram_s));

    if (!algogram) {
        return NULL;
    }

    algogram->logueado = NADIE_LOGUEADO;
    algogram->usuarios = algogram_cargar_usuarios(usuarios);

    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);    //
    while (!hash_iter_al_final(iter)) {                         //
        const char* elem_actual = hash_iter_ver_actual(iter);   //
        printf("%s, ", elem_actual);                            //
        hash_iter_avanzar(iter);                                //
    }                                                           //
    hash_iter_destruir(iter);                                   //

    algogram->posts = NULL;
    algogram->contador_posts = 0;

    return algogram;
}


void algogram(FILE* usuarios) {
    algogram_s* algogram = algogram_crear(usuarios);
    if (!algogram) return;

    while (true) {
        algogram_ingresar_comandos(algogram);
    }
}
