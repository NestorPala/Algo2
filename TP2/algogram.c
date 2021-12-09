#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include "algogram.h"
#include "cola.h"
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
#define NADIE_LOGUEADO -1
#define DEBUG false //debug


typedef struct red_social {
    size_t logueado;
    vd_t* usuarios_id;       // Se utiliza para crear los feeds de los usuarios
    hash_t* usuarios_feed;   // Se utiliza para obtener en O(1) los feeds de los usuarios
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
    size_t id;
    size_t autor;
    char* contenido;
    time_t fecha_creacion;
    size_t cant_likes; 
    abb_t* likes;       
} post_s;


// Sirve para poder comparar los posts usando postcmp()
typedef struct post_autor_distancia {
    post_s* post;
    size_t dist;
} postdist_s;


// DEBUG
void mostrar_posts(algogram_s* algogram) {
    for (size_t i=0; i<algogram->contador_posts; i++) {

        bool resultado = true;
        post_s* post_actual = vd_obtener(algogram->posts, i, NULL);

        if (!resultado) {
            break;
        }

        printf("\n----------------------------------------------------\n");
        printf("POST NUMERO %zu\n", i+1);

        size_t autor      = post_actual -> autor;
        char* contenido   = post_actual -> contenido;
        int fecha_creac   = 0;
        size_t cant_likes = post_actual -> cant_likes;
        abb_t* likes      = post_actual -> likes;

        printf("AUTOR:  %zu\n", autor);
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


// DEBUG
void* vd_obtener_2(vd_t* vd, size_t pos) {
    return vd_obtener(vd, pos, NULL);
}


// DEBUG
void heap_mostrar(heap_t* heap) {

    postdist_s* actual = NULL;

    printf("\nHEAP DE FEED:   (( ");

    for (size_t i=0; i<heap->capacidad; i++) {
        actual = heap->arr[i];

        actual ? printf("%zu, ", actual->post->autor) : printf("__, ");
    }

    printf(" ))\n\n\n");
}


// DEBUG
void mostrar_feed(algogram_s* algogram, size_t id) {

    heap_t* feed = ((usuario_s*)hash_obtener(algogram->usuarios_feed, (char*)vd_obtener_2(algogram->usuarios_id, id)))->feed;
    printf("USUARIO ID %zu:",id);
    heap_mostrar(feed);
}


// DEBUG
void mostrar_usuarios(algogram_s* algogram) {
    vd_print(algogram->usuarios_id, 1);

    hash_iter_t* iter = hash_iter_crear(algogram->usuarios_feed);    

    while (!hash_iter_al_final(iter)) {                                                                 
        const char* usuario_actual = hash_iter_ver_actual(iter);                                        
        size_t id_usuario_actual = ((usuario_s*)hash_obtener(algogram->usuarios_feed, usuario_actual))->id;  
        printf("USUARIO %zu: %s\n", id_usuario_actual, usuario_actual);                                 
        hash_iter_avanzar(iter);                                                                        
    }              

    hash_iter_destruir(iter);      
                                                                        
    printf("\n"); 
    printf("CANTIDAD DE IDS DE USUARIOS: %zu\n", vd_cantidad(algogram->usuarios_id));
    printf("CANTIDAD DE FEEDS DE USUARIOS: %zu\n", hash_cantidad(algogram->usuarios_feed));                                                               
}


///////////////////////////////////////////////////////////////


// AUXILIAR
int convertir_cadena_a_numero(char* cadena) {
    return (int)strtol(cadena, NULL, 10);
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

    free(buffer);////

    //printf("\nINGRESADO: %s", nuevo_buffer);  //debug

    return nuevo_buffer;
}


// AUXILIAR
// Wrapper de vd_guardar con redimension automatica
void vd_insertar(vd_t* vector, size_t pos, void* dato) {
    if (vd_cantidad(vector) >= vd_largo(vector)) {
        vd_redimensionar(vector, 2 * vd_largo(vector));
    }

    vd_guardar(vector, pos, dato);
}


// AUXILIAR
bool hay_logueado(algogram_s* algogram) {
    return algogram->logueado != NADIE_LOGUEADO;
}


// AUXILIAR  
// Sirve para comparar dos posts para saber cual va primero en el feed del usuario.
int postcmp(const void* a, const void* b) {

    const postdist_s* post_a = a;
    const postdist_s* post_b = b;

    if (post_a->dist == post_b->dist) {
        return post_b->post->fecha_creacion - post_a->post->fecha_creacion;
    } else if (post_a->dist < post_b->dist) {
        return 1;
    } else {
        return -1;
    }
}


// AUXILIAR 
size_t calcular_distancia_usuarios(size_t usuario_a, size_t usuario_b) {
    return (size_t)abs((int)usuario_a - (int)usuario_b);
}


// AUXILIAR
postdist_s* postdist_crear(algogram_s* algogram, size_t usuario_actual, post_s* post) {
    postdist_s* postdist = malloc(sizeof(postdist_s));
    if (!post) return NULL;

    postdist->post = post;
    postdist->dist = calcular_distancia_usuarios(usuario_actual, algogram->logueado);

    return postdist;
}


// AUXILIAR
void post_destruir(post_s* post) {
    if (!post) return;
    free(post->contenido);
    abb_destruir(post->likes);
    free(post);
}


// AUXILIAR
void postdist_destruir(postdist_s* postdist) {
    postdist ? printf("POSTDIST DESTRUIR\n") : 0; //debug
    postdist ? printf("POSTDIST AUTOR: %zu\n", postdist->post->autor) : 0; //debug
    free(postdist);
}


// AUXILIAR
void postdist_destruir_aux(void* postdist) {
    postdist_destruir(postdist);
}


// AUXILIAR 
post_s* post_crear(algogram_s* algogram, size_t id, char* comentario) {

    post_s* post = malloc(sizeof(post_s));

    post->id = id;
    post->red = algogram;
    post->autor = algogram->logueado;
    post->cant_likes = 0;
    post->contenido = strdup(comentario);
    post->fecha_creacion = time(NULL);

    post->likes = abb_crear(strcmp, NULL);
    if (!post->likes) {
        free(post);
        return NULL;
    }

    return post;
}


// AUXILIAR
bool post_existe(algogram_s* algogram, int id_post) {

    if (id_post < 0) return false;

    bool ok = false;
    post_s* post = vd_obtener(algogram->posts, id_post, &ok);
    return ok && post;
}


// AUXILIAR
void post_agregar_feed(algogram_s* algogram, post_s* post) {

    hash_iter_t* iter = hash_iter_crear(algogram->usuarios_feed);
    if (!iter) return;

    while(!hash_iter_al_final(iter)) {
        const char* usuario_actual_nombre = hash_iter_ver_actual(iter);
        usuario_s* usuario_actual = hash_obtener(algogram->usuarios_feed, usuario_actual_nombre);

        if (usuario_actual->id != algogram->logueado) {

            // Calculo la distancia del usuario actual al logueado; así obtengo un 'post comparable'
            postdist_s* post_d = postdist_crear(algogram, usuario_actual->id, post);

            printf("POSTDIST CREAR\n"); //debug

            if (!post_d) return;

            // Agrego el post comparable al feed del usuario
            heap_encolar(usuario_actual->feed, post_d);
        }

        hash_iter_avanzar(iter);
    }

    hash_iter_destruir(iter);
}


// AUXILIAR
// Se utiliza para "recolectar" los likes de un post.
bool post_recolectar_likes(const char* clave, void* dato, void* likes) {
    cola_encolar((cola_t*)likes, strdup(clave));
    return true;
}


// AUXILIAR
usuario_s* usuario_crear(size_t id) {

    usuario_s* usuario = malloc(sizeof(usuario_s));
    if (!usuario) return NULL;

    heap_t* feed = heap_crear(postcmp);
    if (!feed) {
        free(usuario);
        return NULL;
    }

    usuario->id = id;
    usuario->feed = feed;

    return usuario;
}


// AUXILIAR
void usuario_destruir(void* usuario) {
    if (!usuario) return;
    printf("DESTRUYENDO USUARIO %zu\n", ((usuario_s*)usuario)->id ); //debug
    heap_destruir(((usuario_s*)usuario)->feed, postdist_destruir_aux);
    free(usuario);
}


///////////////////////////////////////////////////////////////


void logout(algogram_s* algogram) {
    DEBUG ? printf("---------------LOGOUT---------------\n") : false; //debug

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    algogram->logueado = NADIE_LOGUEADO;

    printf("Adios\n");
}


void post_ver_likes(algogram_s* algogram) {
    DEBUG ? printf("---------------POST VER LIKES---------------\n") : false; //debug

    char* cadena = entrada_usuario();
    int id_post = convertir_cadena_a_numero(cadena);
    post_s* post = vd_obtener(algogram->posts, id_post, NULL);

    if (!post_existe(algogram, id_post) || post->cant_likes == 0) {
        printf("Error: Post inexistente o sin likes\n");
        return;
    }

    cola_t* likes = cola_crear();
    if (!likes) return;

    abb_in_order(post->likes, post_recolectar_likes, likes);

    printf("El post tiene %zu likes:\n", post->cant_likes);

    while(!cola_esta_vacia(likes)) {
        char* like_actual = cola_desencolar(likes);
        printf("\t%s\n", like_actual);
    }

    cola_destruir(likes, free);
    free(cadena);
}


void post_likear(algogram_s* algogram) {
    DEBUG ? printf("---------------POST LIKEAR---------------\n") : false; //debug

    if (!hay_logueado(algogram)) {
        printf("Error: Usuario no loggeado o Post inexistente\n");
        return;
    }
   
    char* cadena = entrada_usuario();
    int id_post = convertir_cadena_a_numero(cadena);

    if (!post_existe(algogram, id_post)) {
        printf("Error: Usuario no loggeado o Post inexistente\n");
        return;
    }

    post_s* post_gustado = vd_obtener(algogram->posts, id_post, NULL);
    char* logueado = vd_obtener(algogram->usuarios_id, algogram->logueado, NULL);

    if (!abb_pertenece(post_gustado->likes, logueado)) {
        if (abb_guardar(post_gustado->likes, logueado, NULL)) {
            post_gustado->cant_likes++;
        }
    }

    printf("Post likeado\n");
    free(cadena);
}


void post_ver_siguiente(algogram_s* algogram) {
    DEBUG ? printf("---------------POST VER SIGUIENTE---------------\n") : false; //debug

    if (!hay_logueado(algogram)) {
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }

    const char* usuario_nombre = vd_obtener(algogram->usuarios_id, algogram->logueado, NULL);
    heap_t* usuario_feed = ((usuario_s*)hash_obtener(algogram->usuarios_feed, usuario_nombre))->feed;

    if (heap_esta_vacio(usuario_feed)) {
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }
    
    DEBUG ? mostrar_feed(algogram, algogram->logueado) : false;    //debug

    postdist_s* postdist_siguiente = heap_desencolar(usuario_feed);
    post_s* post_siguiente = postdist_siguiente->post;

    size_t post_id = post_siguiente->id;
    char* post_autor = vd_obtener(algogram->usuarios_id, post_siguiente->autor, NULL);
    char* post_comentario = post_siguiente->contenido;
    size_t post_cant_likes = post_siguiente->cant_likes;

    printf("Post ID %zu\n", post_id);
    printf("%s dijo: %s\n", post_autor, post_comentario);
    printf("Likes: %zu\n", post_cant_likes);
}


void post_publicar(algogram_s* algogram) {
    DEBUG ? printf("---------------POST PUBLICAR---------------\n") : false; //debug

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    char* comentario = entrada_usuario();
    DEBUG ? printf("USTED HA COMENTADO: %s\n", comentario) : false; //debug

    // Creo el post
    post_s* post = post_crear(algogram, algogram->contador_posts, comentario);
    if (!post) {
        free(comentario);
        return;
    }

    // Publico el post
    if (!algogram->posts) {
        algogram->posts = vd_crear(6);
    }

    vd_insertar(algogram->posts, algogram->contador_posts, post);
    algogram->contador_posts++;
    post_agregar_feed(algogram, post);

    printf("Post publicado\n");

    DEBUG ? mostrar_posts(algogram) : false; //debug

    for (size_t i=0; i<vd_largo(algogram->usuarios_id); i++) { //debug
        if (i != algogram->logueado) {
            mostrar_feed(algogram, i);
        }
    }
    
    free(comentario);
}


void login(algogram_s* algogram) {
    DEBUG ? printf("---------------LOGIN---------------\n") : false; //debug

    char* cadena = entrada_usuario();

    if (hay_logueado(algogram)) {
        printf("Error: Ya habia un usuario loggeado\n");
        return;
    }

    if (!hash_pertenece(algogram->usuarios_feed, cadena)) {
        printf("Error: usuario no existente\n");
        return;
    }

    algogram->logueado = ((usuario_s*)hash_obtener(algogram->usuarios_feed, cadena))->id;

    printf("Hola %s\n", cadena);

    free(cadena);
}


///////////////////////////////////////////////////////////////


// AUXILIAR
bool ejecutar_comando(char* comando, algogram_s* algogram) {

    if (strcmp(comando, "salir") == 0) { //debug
        return false;
    }
    
         if (strcmp(comando, LOGIN)              == 0)  login(algogram);
    else if (strcmp(comando, LOGOUT)             == 0)  logout(algogram);
    else if (strcmp(comando, PUBLICAR_POST)      == 0)  post_publicar(algogram);
    else if (strcmp(comando, VER_SIGUIENTE_FEED) == 0)  post_ver_siguiente(algogram);
    else if (strcmp(comando, LIKEAR_POST)        == 0)  post_likear(algogram);
    else if (strcmp(comando, MOSTRAR_LIKES)      == 0)  post_ver_likes(algogram);
    else if (strcmp(comando, "clear")            == 0)  exit(0); //debug

    return true;
}


// AUXILIAR
bool es_comando(char* cadena) {
    return     strcmp(cadena, LOGIN)               == 0
            || strcmp(cadena, LOGOUT)              == 0
            || strcmp(cadena, PUBLICAR_POST)       == 0
            || strcmp(cadena, VER_SIGUIENTE_FEED)  == 0
            || strcmp(cadena, LIKEAR_POST)         == 0
            || strcmp(cadena, MOSTRAR_LIKES)       == 0
            || strcmp(cadena, "clear")             == 0  //debug
            || strcmp(cadena, "salir")             == 0; //debug
}


// AUXILIAR
bool algogram_ingresar_comandos(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (!es_comando(cadena)) {
        printf("Comando inválido.\n");
        return true;
    }
    
    if (!ejecutar_comando(cadena, algogram)) {
        free(cadena);
        return false;
    }

    free(cadena);
    return true;
}


// AUXILIAR 
hash_t* algogram_cargar_usuarios(vd_t* usuarios_id) {

    hash_t* usuarios = hash_crear(usuario_destruir);

    if (!usuarios) {
        return NULL;
    }

    size_t largo = vd_largo(usuarios_id);

    for (size_t i=0; i<largo; i++) {
        const char* usuario_actual_nombre = vd_obtener(usuarios_id, i, NULL);

        if (!usuario_actual_nombre) continue;

        // Creo un usuario que contiene el ID usuario y su respectivo feed (vacio por ahora)
        usuario_s* usuario_actual = usuario_crear(i);
        if (!usuario_actual) return NULL;

        hash_guardar(usuarios, usuario_actual_nombre, usuario_actual);
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

        if (strcmp(nuevo_buffer, "") == 0) continue;

        vd_insertar(usuarios_id, i, nuevo_buffer);
        i++;
    }

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

    algogram->usuarios_feed = algogram_cargar_usuarios(algogram->usuarios_id);
    if (!algogram->usuarios_feed) {
        vd_destruir(algogram->usuarios_id, usuario_destruir);
        free(algogram);
        return NULL;
    }

    DEBUG ? mostrar_usuarios(algogram) : false; //DEBUG

    algogram->posts = NULL;
    algogram->contador_posts = 0;

    return algogram;
}


// AUXILIAR
void post_destruir_aux(void* post) {
    post_destruir(post);
}


// AUXILIAR
void algogram_destruir(algogram_s* algogram) {
    hash_destruir(algogram->usuarios_feed);
    vd_destruir(algogram->usuarios_id, free);
    vd_destruir(algogram->posts, post_destruir_aux);
    free(algogram);
}


///////////////////////////////////////////////////////////////


// INICIADOR 
void algogram(FILE* usuarios) {

    algogram_s* algogram = algogram_crear(usuarios);
    if (!algogram) {
        printf("ERROR FATAL\n");
        return;
    }

    while (algogram_ingresar_comandos(algogram));

    algogram_destruir(algogram);
}
