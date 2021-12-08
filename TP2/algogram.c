#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include "string.h" 
#include "algogram.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "vd.h" //vector dinamico

#define LOGIN "login"
#define LOGOUT "logout"
#define PUBLICAR_POST "publicar"
#define VER_SIGUIENTE_FEED "ver_siguiente_feed"
#define LIKEAR_POST "likear_post"
#define MOSTRAR_LIKES "mostrar_likes"
#define NADIE_LOGUEADO " "


typedef struct red_social {
    char* logueado;     // Usamos char* para poder buscar al usuario en O(1) en el hash de feed
    vd_t* usuarios_id;  // Se utiliza para crear los feeds de los usuarios
    hash_t* usuarios;   // Se utiliza para obtener en O(1) los feeds de los usuarios
    vd_t* posts;
    size_t contador_posts;
} algogram_s;


// Sirve para poder obtener en O(1) las posiciones adyacentes al usuario en el vector usuarios_id
typedef struct usuario {
    size_t id;
    heap_t* feed;
} usuario_s;


typedef struct post {
    algogram_s* red;
    char* autor;
    char* contenido;
    int fecha_creacion;
    size_t cant_likes; 
    abb_t* likes;        
} post_s;


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
        abb_t* likes      = ((post_s*)vd_obtener(algogram->posts, i, NULL)) -> likes;

        printf("AUTOR:  %s\n", autor);
        printf("CONTENIDO:  %s\n", contenido);
        printf("FECHA DE CREACION:  %d\n", fecha_creac);
        printf("CANTIDAD DE LIKES: %zu\n", cant_likes);

        // printf("LIKES: ");
        // vd_print(likes);
    }
}


// DEBUG
// tipo_dato: 0: int, 1: char*, >1: void*
void vd_print(vd_t* vector, size_t tipo_dato) {

    size_t largo = vd_largo(vector);
    void* actual = NULL;

    printf("\n[");

    for (size_t i=0; i<largo; i++) {
        
        actual = vd_obtener(vector, i, NULL);

        if (tipo_dato == 0) {
            actual ? printf("%d, ", *(int*)actual) : printf("NULL, ");
        } else if (tipo_dato == 1) {
            actual ? printf("%s, ", (char*)actual) : printf("NULL, ");
        } else {
            actual ? printf("%p, ", actual) : printf("NULL, ");
        }
    }

    printf("]\n");
}


// Wrapper de vd_guardar con redimension automatica
// AUXILIAR
void vd_insertar(vd_t* vector, size_t pos, void* dato) {
    if (vd_cantidad(vector) == vd_largo(vector)) {
        vd_redimensionar(vector, 2 * vd_largo(vector));
    }

    vd_guardar(vector, pos, dato);
}


// AUXILIAR
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


// AUXILIAR
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


// AUXILIAR
bool hay_logueado(algogram_s* algogram) {
    return algogram->logueado != NADIE_LOGUEADO;
}


// AUXILIAR
int obtener_fecha_actual() {
    //...
    return 0;
}


// AUXILIAR 
post_s* post_crear(algogram_s* algogram, char* comentario) {

    post_s* post = malloc(sizeof(post_s));

    post->red = algogram;
    post->autor = strdup(algogram->logueado);
    post->cant_likes = 0;
    post->contenido = strdup(comentario);
    post->fecha_creacion = obtener_fecha_actual(); //

    post->likes = abb_crear(strcmp, NULL);
    if (!post->likes) {
        free(post);
        return NULL;
    }

    return post;
}


// AUXILIAR  
// Sirve para comparar dos posts para saber cual va primero en el feed
int postcmp(const void* a, const void* b) {

    const post_s* post_a = a;
    const post_s* post_b = b;

    //char* logueado = post_a->red->logueado;
    
    //...

    return 0;
}


// AUXILIAR
void post_agregar_feed(algogram_s* algogram, post_s* post) {

    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);
    if (!iter) return;

    while(!hash_iter_al_final(iter)) {
        const char* usuario_actual = hash_iter_ver_actual(iter);
        heap_t* feed_actual = hash_obtener(algogram->usuarios, usuario_actual);

        // Si el feed esta vacio
        if (!feed_actual) { 
            heap_t* feed = heap_crear(postcmp);
            if (!feed) return;
            hash_guardar(algogram->usuarios, usuario_actual, feed);
        }

        // Agrego el post al feed del usuario
        heap_encolar(feed_actual, post);

        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
}


// AUXILIAR 
bool post_enviar(algogram_s* algogram, post_s* post) {
    if (!algogram->posts) {
        algogram->posts = vd_crear(5);
    }

    vd_insertar(algogram->posts, algogram->contador_posts, post);

    return true;
}


// AUXILIAR 
void heap_destruir_aux(void* heap) {
    heap_destruir(heap, NULL);
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


void post_publicar(algogram_s* algogram) {
    printf("---------------POST PUBLICAR---------------\n"); //debug

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    char* comentario = entrada_usuario();
    printf("USTED HA COMENTADO: %s\n", comentario); //debug

    // Creo el post
    post_s* post = post_crear(algogram, comentario);
    if (!post) {
        free(comentario);
        return;
    }

    // Publico el post
    if (post_enviar(algogram, post)) {
        algogram->contador_posts++;
        post_agregar_feed(algogram, post);
    }

    //mostrar_posts(algogram); //debug
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


// AUXILIAR
void ejecutar_comando(char* comando, algogram_s* algogram) {
         if (strcmp(comando, LOGIN)              == 0)  login(algogram);
    else if (strcmp(comando, LOGOUT)             == 0)  logout(algogram);
    else if (strcmp(comando, PUBLICAR_POST)      == 0)  post_publicar(algogram);
    else if (strcmp(comando, VER_SIGUIENTE_FEED) == 0)  post_ver_siguiente(algogram);
    else if (strcmp(comando, LIKEAR_POST)        == 0)  post_likear(algogram);
    else if (strcmp(comando, MOSTRAR_LIKES)      == 0)  post_ver_likes(algogram);
    else if (strcmp(comando, "clear")            == 0)  exit(0); //debug
}


// AUXILIAR
bool es_comando(char* cadena) {
    return     strcmp(cadena, LOGIN)               == 0
            || strcmp(cadena, LOGOUT)              == 0
            || strcmp(cadena, PUBLICAR_POST)       == 0
            || strcmp(cadena, VER_SIGUIENTE_FEED)  == 0
            || strcmp(cadena, LIKEAR_POST)         == 0
            || strcmp(cadena, MOSTRAR_LIKES)       == 0
            || strcmp(cadena, "clear")             == 0;  //debug
}


// AUXILIAR
void algogram_ingresar_comandos(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (!es_comando(cadena)) {
        printf("Comando inválido.\n");
        return;
    }
    
    ejecutar_comando(cadena, algogram);
    free(cadena);
}


// AUXILIAR 
hash_t* algogram_cargar_usuarios(vd_t* usuarios_id) {

    hash_t* usuarios = hash_crear(heap_destruir_aux);
    if (!usuarios) {
        return NULL;
    }

    size_t largo = vd_largo(usuarios_id);

    for (size_t i=0; i<largo; i++) {
        hash_guardar(usuarios, (const char*)vd_obtener(usuarios_id, i, NULL), NULL);
    }

    return usuarios;
}


// AUXILIAR 
vd_t* algogram_cargar_usuarios_id(FILE* archivo_usuarios) {

    vd_t* usuarios_id = vd_crear(5);
    if (!usuarios_id) return NULL;

    size_t n = 16, i = 0;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, archivo_usuarios) != EOF) {
        char* nuevo_buffer = quitar_barra_n(buffer, true);
        vd_insertar(usuarios_id, i, nuevo_buffer);
        i++;
    }

    vd_print(usuarios_id, 1); //debug

    free(buffer);

    return usuarios_id;
}


// AUXILIAR 
algogram_s* algogram_crear(FILE* usuarios) {
    algogram_s* algogram = malloc(sizeof(algogram_s));
    if (!algogram) return NULL;

    algogram->logueado = NADIE_LOGUEADO;

    algogram->usuarios_id = algogram_cargar_usuarios_id(usuarios);
    if (!algogram->usuarios_id) {
        free(algogram);
        return NULL;
    }

    algogram->usuarios = algogram_cargar_usuarios(algogram->usuarios_id);
    if (!algogram->usuarios) {
        vd_destruir(algogram->usuarios_id);
        free(algogram);
        return NULL;
    }

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


// INICIADOR 
void algogram(FILE* usuarios) {
    algogram_s* algogram = algogram_crear(usuarios);
    if (!algogram) {
        printf("ERROR FATAL\n");
        return;
    }

    while (true) {
        algogram_ingresar_comandos(algogram);
    }
}
