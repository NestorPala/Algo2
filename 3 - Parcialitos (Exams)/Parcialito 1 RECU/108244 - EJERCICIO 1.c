#include <stdio.h>
#include <string.h>
#include "pila.h"


// Complejidad: O(n), siendo n la cantidad de caracteres de la cadena. Por cada caracter apilamos o desapilamos segun corresponda,
// y ambas operaciones se realizan en O(1), luego comparamos los caracteres, tambien O(1), por lo que el orden final se mantiene en O(n).

// Explicacion del algoritmo: recorro caracter a caracter la cadena. Si el caracter actual es un parentesis de apertura, lo apilo. 
// Si el caracter es un parentesis de cierre, desapilo la pila y comparo ese elemento con el caracter actual. Si no coinciden 
// significa que, o el caracter desapilado no tiene cierre, o el caracter actual no tiene apertura. Y devuelvo false. Si
// termino con toda la cadena, no encontre errores, devuelvo true.


bool chequear_balanceo_parentesis(char* cadena) {
    pila_t* parentesis = pila_crear();
    if (!parentesis) {
        return false;
    }

    bool balanceado = true;

    size_t largo = strlen(cadena);

    for (size_t i=0; i<largo; i++) {
        if (cadena[i] == '[' || cadena[i] == '(' || cadena[i] == '{') {
            pila_apilar(parentesis, &cadena[i]);
        }

        if (cadena[i] == ']' || cadena[i] == ')' || cadena[i] == '}') {

            if (pila_esta_vacia(parentesis)) {
                balanceado = false;
                break;
            }

            char* actual = pila_desapilar(parentesis);

            if ( (cadena[i] == ']' && cadena[i] != *actual) && 
                 (cadena[i] == ')' && cadena[i] != *actual) &&
                 (cadena[i] == '}' && cadena[i] != *actual)
            ){
                balanceado = false;
                break;
            }
        }
    }

    pila_destruir(parentesis);
    
    return balanceado;
}


// int main(){
//     char* cadena[] = {"[(])", "(())}", "{(}", "[({}{}[(){}])]"};

//     for (size_t i=0; i<4; i++) {
//         if (chequear_balanceo_parentesis(cadena[i])) {
//             printf("true\n");
//         } else {
//             printf("false\n");
//         }
//     }

//     return 0;
// }
