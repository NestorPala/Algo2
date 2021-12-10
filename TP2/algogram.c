#define _POSIX_C_SOURCE 200809L
#define LOGIN "login"
#define LOGOUT "logout"
#define PUBLICAR_POST "publicar"
#define VER_SIGUIENTE_FEED "ver_siguiente_feed"
#define LIKEAR_POST "likear_post"
#define MOSTRAR_LIKES "mostrar_likes"
#define NADIE_LOGUEADO -1
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include "algogram.h"
#include "cola.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"
#include "vd.h" //vector dinámico


typedef struct red_social {
    size_t logueado;
    vd_t* usuarios_id;       
    hash_t* usuarios_feed; 
    vd_t* posts;
    size_t contador_posts;
} algogram_s;


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


// AUXILIAR
int convertir_cadena_a_numero(char* cadena) {
    return (int) strtol(cadena, NULL, 10);
}


// AUXILIAR
// Normaliza las cadenas recibidas por el usuario en una cadena nueva y devuelve la cadena nueva.
char* quitar_barra_n(char* cadena, bool ingresar_usuarios) {

    int i = 0, largo = 0;
    largo = strlen(cadena);
    size_t u = ingresar_usuarios ? 2 : 1;
    
    char* nueva_cadena;
    nueva_cadena = malloc(largo - u + 1);
 
    for(i = 0; i < largo-u; i++){
        nueva_cadena[i] = cadena[i]; 
    }

    nueva_cadena[largo - u] = '\0';

    return nueva_cadena;
}


// AUXILIAR
// Recibe los entradas del usuario por consola.
char* entrada_usuario() {
    char* buffer;
    size_t buf_tam = 32;
    buffer = malloc(buf_tam * sizeof(char));

    getline(&buffer, &buf_tam, stdin);
    char* nuevo_buffer = quitar_barra_n(buffer, false);

    free(buffer);

    return nuevo_buffer;
}


// AUXILIAR
// Wrapper de vd_guardar con redimension automática.
void vd_insertar(vd_t* vector, size_t pos, void* dato) {
    
    if (vd_cantidad(vector) >= vd_largo(vector)) {
        vd_redimensionar(vector, 2 * vd_largo(vector));
    }

    vd_guardar(vector, pos, dato);
}


// AUXILIAR
// Devuelve true si ya existe alguien utilizando la red social, y false si no.
bool hay_logueado(algogram_s* algogram) {
    return algogram->logueado != NADIE_LOGUEADO;
}


// AUXILIAR  
// Compara dos posts para saber cual debe mostrarse primero en el feed del usuario.
// Devuelve 0 si los dos posts pertenecen al mismo usuario y fueron publicados al mismo tiempo (prácticamente imposible), 
// un número mayor a cero si el usuario del post A está más cerca del logueado que B o ambos usuarios son el mismo pero el post A fue publicado antes que el B,
// y un número menor a cero si el usuario del post B está más cerca del logueado que A o ambos usuarios son el mismo pero el post B fue publicado antes que el A.
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
// Calcula la distancia en el archivo usuarios.txt que hay entre dos usuarios de la plataforma.
size_t calcular_distancia_usuarios(size_t usuario_a, size_t usuario_b) {

    return (size_t) abs( (int)usuario_a - (int)usuario_b );
}


// AUXILIAR
// Crea un wrapper de Post que contiene, adicionalmente, la distancia del usuario que lo publicó al usuario que contiene la referencia del post en su feed.
// Pre: el post existe.
// Postcondición: el post "wrappereado" está listo para ser guardado en el feed de un usuario de esta plataforma.
postdist_s* postdist_crear(algogram_s* algogram, size_t usuario_actual, post_s* post) {

    postdist_s* post_d = malloc(sizeof(postdist_s));

    if (!post) {
        return NULL;
    }

    post_d->post = post;
    post_d->dist = calcular_distancia_usuarios(usuario_actual, algogram->logueado);

    return post_d;
}


// AUXILIAR
// Borra un post y libera todos los datos que éste contiene.
// Postcondición: el post ya no existe.
void post_destruir(post_s* post) {

    if (!post) {
        return;
    }

    free(post->contenido);
    abb_destruir(post->likes);
    free(post);
}


// AUXILIAR
// Destruye el wrapper de un post.
// Pre: el postdist existe.
// Postcondición: el wrapper ya no existe. El post original no se ha destruido.
void postdist_destruir(postdist_s* post_d) {
    free(post_d);
}


// AUXILIAR
// Wrapper para destruir un post dentro de un feed.
void postdist_destruir_aux(void* post_d) {
    postdist_destruir(post_d);
}


// AUXILIAR 
// Crea un post nuevo, que incluye el id del autor, el comentario realizado, los likes y la fecha de creación.
// Pre: el id existe.
// Post: el post está listo para pasar a la siguiente fase del proceso de publicación.
post_s* post_crear(algogram_s* algogram, size_t id, char* comentario) {

    post_s* post = malloc(sizeof(post_s));

    if (!post) {
        return NULL;
    }

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

    if (id_post < 0) {
        return false;
    }

    bool ok = false;
    post_s* post = vd_obtener(algogram->posts, id_post, &ok);

    return ok && post;
}


// AUXILIAR
// Luego de publicado el post, agrega el mismo a los feeds de todos los usuarios, menos el logueado, quien está realizando dicha publicación.
// Pre: el post existe.
// Postcondición: todos los usuarios menos el logueado ahora poseen 1 post más para ver.
void post_agregar_feed(algogram_s* algogram, post_s* post) {

    hash_iter_t* iter = hash_iter_crear(algogram->usuarios_feed);

    if (!iter) {
        return;
    }

    while(!hash_iter_al_final(iter)) {
        
        const char* usuario_actual_nombre = hash_iter_ver_actual(iter);
        usuario_s* usuario_actual = hash_obtener(algogram->usuarios_feed, usuario_actual_nombre);

        if (usuario_actual->id != algogram->logueado) {

            // Calculo la distancia del usuario actual al logueado; así obtengo un 'post comparable'
            postdist_s* post_d = postdist_crear(algogram, usuario_actual->id, post);

            if (!post_d) {
                return;
            }

            // Agrego el post comparable al feed del usuario
            heap_encolar(usuario_actual->feed, post_d);

        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
}


// AUXILIAR
// Se utiliza para "recolectar" los likes de un post.
// Pre: la clave existe.
// Post: la clave original no se destruye.
bool post_recolectar_likes(const char* clave, void* dato, void* likes) {

    cola_encolar((cola_t*)likes, strdup(clave));
    return true;
}


// AUXILIAR
// Crea un nuevo usuario, el cual contiene su ID y su respectivo feed de publicaciones.
// Pre: el ID es válido.
// Post: la red social Algogram tiene un nuevo miembro en su comunidad.
usuario_s* usuario_crear(size_t id) {

    usuario_s* usuario = malloc(sizeof(usuario_s));

    if (!usuario) {
        return NULL;
    }

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
// Borra el usuario de la red social Algogram y libera todo su contenido.
// Pre: el usuario existe.
// Post: el usuario ya no existe en la red social Algogram. Los posts originales no se han destruido.
void usuario_destruir(void* usuario) {

    if (!usuario) {
        return;
    }

    heap_destruir( ((usuario_s*)usuario) -> feed, postdist_destruir_aux );
    free(usuario);
}


// Termina la sesión de la persona actual utilizando la red social.
// En caso de que no haya un usuario logueado, se muestra un mensaje de error por pantalla y se termina la operación actual.
void logout(algogram_s* algogram) {

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    algogram->logueado = NADIE_LOGUEADO;

    printf("Adios\n");
}


// Muestra las personas que dieron like a un determinado post. No hace falta estar logueado para realizar esta operación.
// Si el post no existe o no posee likes, se muestra un mensaje de error por pantalla y se termina la operación actual.
void post_ver_likes(algogram_s* algogram) {

    char* cadena = entrada_usuario();
    int id_post = convertir_cadena_a_numero(cadena);
    post_s* post = vd_obtener(algogram->posts, id_post, NULL);

    if (!post_existe(algogram, id_post) || post->cant_likes == 0) {
        printf("Error: Post inexistente o sin likes\n");
        return;
    }

    cola_t* likes = cola_crear();

    if (!likes) {
        return;
    }

    abb_in_order(post->likes, post_recolectar_likes, likes);

    printf("El post tiene %zu likes:\n", post->cant_likes);

    while(!cola_esta_vacia(likes)) {
        char* like_actual = cola_desencolar(likes);
        printf("\t%s\n", like_actual);
    }

    cola_destruir(likes, free);
    free(cadena);
}


// Permite dejar un "like" (me gusta) a un post en particular.
// Si el post no existe o no hay un usuario logueado, se muestra un mensaje de error por pantalla y se termina la operación actual.
void post_likear(algogram_s* algogram) {

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


// Permite ver el siguiente post en el feed de publicaciones. 
// Si no hay más posts para ver o no hay un usuario logueado, se muestra un mensaje de error por pantalla y se termina la operación actual.
void post_ver_siguiente(algogram_s* algogram) {

    if (!hay_logueado(algogram)) {
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }

    const char* usuario_nombre = vd_obtener(algogram->usuarios_id, algogram->logueado, NULL);

    heap_t* usuario_feed = ((usuario_s*) hash_obtener(algogram->usuarios_feed, usuario_nombre)) -> feed;

    if (heap_esta_vacio(usuario_feed)) {
        printf("Usuario no loggeado o no hay mas posts para ver\n");
        return;
    }

    postdist_s* postdist_siguiente = heap_desencolar(usuario_feed);
    post_s* post_siguiente = postdist_siguiente->post;

    size_t post_id          =  post_siguiente->id;
     char* post_autor       =  vd_obtener(algogram->usuarios_id, post_siguiente->autor, NULL);
     char* post_comentario  =  post_siguiente->contenido;
    size_t post_cant_likes  =  post_siguiente->cant_likes;

    printf("Post ID %zu\n", post_id);
    printf("%s dijo: %s\n", post_autor, post_comentario);
    printf("Likes: %zu\n", post_cant_likes);
}


// Permite generar un nuevo post con un nuevo comentario.
// En caso de que no haya un usuario logueado, se muestra un mensaje de error por pantalla y se termina la operación actual.
void post_publicar(algogram_s* algogram) {

    if (!hay_logueado(algogram)) {
        printf("Error: no habia usuario loggeado\n");
        return;
    }

    char* comentario = entrada_usuario();

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

    free(comentario);
}


// Permite a un usuario ingresar a la red social Algogram.
// En caso de que ya haya un usuario logueado o el usuario no exista, se muestra un mensaje de error por pantalla y se termina la operación actual.
void login(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (hay_logueado(algogram)) {
        printf("Error: Ya habia un usuario loggeado\n");
        return;
    }

    if (!hash_pertenece(algogram->usuarios_feed, cadena)) {
        printf("Error: usuario no existente\n");
        return;
    }

    algogram->logueado = ((usuario_s*) hash_obtener(algogram->usuarios_feed, cadena)) -> id;
    printf("Hola %s\n", cadena);

    free(cadena);
}


// AUXILIAR
// Ejecuta la parte correspondiente del programa a la acción solicitada por el usuario.
bool ejecutar_comando(char* comando, algogram_s* algogram) {

    if (strcmp(comando, "exit") == 0) {
        return false;
    }
    else if (strcmp(comando, LOGIN) == 0) {
        login(algogram);
    }
    else if (strcmp(comando, LOGOUT) == 0) {
        logout(algogram);
    }
    else if (strcmp(comando, PUBLICAR_POST) == 0) {
        post_publicar(algogram);
    } 
    else if (strcmp(comando, VER_SIGUIENTE_FEED) == 0) {
        post_ver_siguiente(algogram);
    } 
    else if (strcmp(comando, LIKEAR_POST) == 0) {
        post_likear(algogram);
    } 
    else if (strcmp(comando, MOSTRAR_LIKES) == 0) {
         post_ver_likes(algogram);
    }

    return true;
}


// AUXILIAR
// Devuelve true si la cadena ingresada por el usuario es un comando válido de Algogramm, y false si no.
bool es_comando(char* cadena) {
    return     strcmp(cadena, LOGIN)               == 0
            || strcmp(cadena, LOGOUT)              == 0
            || strcmp(cadena, PUBLICAR_POST)       == 0
            || strcmp(cadena, VER_SIGUIENTE_FEED)  == 0
            || strcmp(cadena, LIKEAR_POST)         == 0
            || strcmp(cadena, MOSTRAR_LIKES)       == 0
            || strcmp(cadena, "exit")              == 0;
}


// AUXILIAR
// Permite al usuario ingresar una cadena de caracteres por entrada estándar, y si esa cadena representa un comando válido, ésta acción se realiza.
bool algogram_ingresar_comandos(algogram_s* algogram) {

    char* cadena = entrada_usuario();

    if (!es_comando(cadena)) {

        printf("Comando inválido.\n");

        free(cadena);
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
// Crea los usuarios de la red social a partir de la lista de nombres recogidos de usuarios.txt.
// Pre: la lista de usuarios no está vacía.
// Post: los usuarios están completamente creados en el sistema.
hash_t* algogram_cargar_usuarios(vd_t* usuarios_id) {

    hash_t* usuarios = hash_crear(usuario_destruir);

    if (!usuarios) {
        return NULL;
    }

    size_t largo = vd_largo(usuarios_id);

    for (size_t i=0; i<largo; i++) {
        const char* usuario_actual_nombre = vd_obtener(usuarios_id, i, NULL);

        if (!usuario_actual_nombre) {
            continue;
        }

        // Creo un usuario que contiene el ID usuario y su respectivo feed (vacio por ahora)
        usuario_s* usuario_actual = usuario_crear(i);

        if (!usuario_actual) {
            return NULL;
        }

        hash_guardar(usuarios, usuario_actual_nombre, usuario_actual);
    }

    return usuarios;
}


// AUXILIAR
// Crea la lista de nombres de usuarios de la red social a partir del archivo usuarios.txt. 
// Pre: el archivo usuarios.txt es válido.
// Post: los usuarios ya están cargados en el sistema.
vd_t* algogram_cargar_usuarios_id(FILE* archivo_usuarios) {

    vd_t* usuarios_id = vd_crear(5);

    if (!usuarios_id) {
        return NULL;
    }

    size_t n = 16, i = 0;
    char* buffer = (char*)malloc(n * sizeof(char));

    while(getline(&buffer, &n, archivo_usuarios) != EOF) {
        char* nuevo_buffer = quitar_barra_n(buffer, true);

        if (strcmp(nuevo_buffer, "") == 0) {
            continue;
        }

        vd_insertar(usuarios_id, i, nuevo_buffer);
        i++;
    }

    free(buffer);
    return usuarios_id;
}


// AUXILIAR 
// Crea la estructura principal de la red social Algogram.
// Pre: el archivo usuarios.txt es válido.
// Post: la red social está lista para ser utilizada.
algogram_s* algogram_crear(FILE* usuarios) {
    
    algogram_s* algogram = malloc(sizeof(algogram_s));

    if (!algogram) {
        return NULL;
    }

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

    algogram->posts = NULL;
    algogram->contador_posts = 0;

    return algogram;
}


// AUXILIAR
// Wrapper para destruir los posts.
void post_destruir_aux(void* post) {
    post_destruir(post);
}


// AUXILIAR
// Destruye todos los datos de la red social Algogram.
// Post: se acabó todo, todillo.
void algogram_destruir(algogram_s* algogram) {
    hash_destruir(algogram->usuarios_feed);
    vd_destruir(algogram->usuarios_id, free);
    vd_destruir(algogram->posts, post_destruir_aux);
    free(algogram);
}


///////////////////////////////////////////////////////////////


// INICIADOR 
// Recibe el archivo usuarios.txt, crea la red social e inicia un bucle de recibir comandos y realizar acciones dentro de la red social Algogram.
// Pre: el archivo usuarios.txt es válido.
void algogram(FILE* usuarios) {

    algogram_s* algogram = algogram_crear(usuarios);

    if (!algogram) {
        printf("ERROR FATAL\n");
        return;
    }

    while (algogram_ingresar_comandos(algogram));

    algogram_destruir(algogram);
}
