/* GENERADOR DE CADENAS Y NUMEROS ALEATORIOS */
/* PARCIALMENTE HECHO POR NESTOR FABIAN PALAVECINO ARNOLD */


/*  
    Crea un arreglo de palabras de longitud variable con caracteres aleatorios. Devuelve memoria dinámica.
    Esta función fue sacada de:
    https://stackoverflow.com/questions/47064165/how-to-create-an-array-of-100-random-strings-in-c
*/
char** arreglo_cadenas_crear(size_t cant);

// Destruye un arreglo de cadenas, siendo tanto el arreglo como su contenido de memoria dinámica.
void arreglo_cadenas_destruir(char** str, size_t n);

// Crea un arreglo de enteros aleatorios entre 0 y 1000. Devuelve memoria dinámica.
int** arreglo_numeros_crear(size_t n);

// Destruye un arreglo de enteros, siendo tanto el arreglo como su contenido de memoria dinámica.
void arreglo_numeros_destruir(int** numeros, size_t n);
