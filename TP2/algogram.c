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
    char* autor;
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
 
    nueva_cadena = malloc(largo - u + 1);
 
    for(i = 0; i < largo-u; i++){
        nueva_cadena[i] = cadena[i]; 
    }

    nueva_cadena[largo - u] = '\0';
 
    return nueva_cadena;
}


char* entrada_usuario() {
    char* buffer;
    size_t buf_tam = 32;
    buffer = malloc(buf_tam * sizeof(char));

    //printf("\n>>>>>  "); //debug

    getline(&buffer, &buf_tam, stdin);
    char* nuevo_buffer = quitar_barra_n(buffer, false);

    //printf("\nINGRESADO: %s", nuevo_buffer);  //debug

    return nuevo_buffer;
}


bool hay_logueado(algogram_s* algogram) {
    return algogram->logueado != NADIE_LOGUEADO;
}


void logout(algogram_s* algogram) {
    printf("---------------LOGOUT---------------\n"); //debug

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    free(algogram->logueado);
    algogram->logueado = NADIE_LOGUEADO;

    printf("Adios\n");
}


void post_ver_likes(algogram_s* algogram) {
    printf("---------------POST VER LIKES---------------\n"); //debug
}


void post_likear(algogram_s* algogram) {
    printf("---------------POST LIKEAR---------------\n"); //debug
}


void post_ver_siguiente(algogram_s* algogram) {
    printf("---------------POST VER SIGUIENTE---------------\n"); //debug
}


int obtener_fecha_actual() {
    //...
    return 0;
}


// DEBUG
void mostrar_posts(algogram_s* algogram) {
    for (size_t i=0; i<algogram->contador_posts; i++) {

        bool resultado = true;
        post_s* post = vd_obtener(algogram->posts, i, &resultado);

        if (!resultado) {
            break;
        }

        printf("\n----------------------------------------------------\n");
        printf("POST NUMERO %zu\n", i+1);

        char* autor       = ((post_s*)vd_obtener(algogram->posts, i, NULL)) -> autor;
        char* contenido   = ((post_s*)vd_obtener(algogram->posts, i, NULL)) -> contenido;
        int fecha_creac   = 0;
        size_t cant_likes = ((post_s*)vd_obtener(algogram->posts, i, NULL)) -> cant_likes;
        vd_t* likes       = ((post_s*)vd_obtener(algogram->posts, i, NULL)) -> likes;

        printf("AUTOR:  %s\n", autor);
        printf("CONTENIDO:  %s\n", contenido);
        printf("FECHA DE CREACION:  %d\n", fecha_creac);
        printf("CANTIDAD DE LIKES: %zu\n", cant_likes);

        // printf("LIKES: ");
        // vd_print(likes);
    }
}


void post_publicar(algogram_s* algogram) {
    printf("---------------POST PUBLICAR---------------\n"); //debug

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    char* comentario = entrada_usuario();
    printf("USTED HA COMENTADO: %s\n", comentario); //debug

    // Creo el post

    post_s* post = malloc(sizeof(post_s));
    if (!post) {
        free(comentario);
        return;
    }

    post->autor = strdup(algogram->logueado);
    post->cant_likes = 0;
    post->contenido = strdup(comentario);
    post->fecha_creacion = obtener_fecha_actual();
    post->likes = NULL;

    // Publico el post

    if (!algogram->posts) {
        algogram->posts = vd_crear(5);
    }

    size_t cant_posts = algogram->contador_posts;
    size_t max_posts = vd_largo(algogram->posts);

    if (cant_posts == max_posts) {
        vd_redimensionar(algogram->posts, 2 * max_posts);
    }

    vd_guardar(algogram->posts, cant_posts, post);
    algogram->contador_posts++;


    mostrar_posts(algogram); //debug
}


void login(algogram_s* algogram) {
    printf("---------------LOGIN---------------\n"); //debug

    char* cadena = entrada_usuario();

    if (hay_logueado(algogram)) {
        printf("Error: Ya habia un usuario loggeado\n");
        return;
    }

    if (!hash_pertenece(algogram->usuarios, cadena)) {
        printf("Error: usuario no existente\n");
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
    else if (strcmp(comando, "clear")            == 0)  exit(0); //debug
}


bool es_comando(char* cadena) {
    return     strcmp(cadena, LOGIN)               == 0
            || strcmp(cadena, LOGOUT)              == 0
            || strcmp(cadena, PUBLICAR_POST)       == 0
            || strcmp(cadena, VER_SIGUIENTE_FEED)  == 0
            || strcmp(cadena, LIKEAR_POST)         == 0
            || strcmp(cadena, MOSTRAR_LIKES)       == 0
            || strcmp(cadena, "clear")             == 0;  //debug
}


void algogram_ingresar_comandos(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (!es_comando(cadena)) {
        printf("Comando inválido.\n");
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
    printf("\n");                                               //

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
