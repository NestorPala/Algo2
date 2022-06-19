#include <stdlib.h>
#include <stdio.h> // sprintf
#include <string.h>
#include "fraccion.h"
#include <stdbool.h>


struct fraccion {
    int numerador;
    int denominador;
};


fraccion_t* fraccion_crear(int numerador, int denominador) {
    
    fraccion_t* frac = malloc(sizeof(fraccion_t));

    if (frac == NULL || denominador == 0) {
        return NULL;
    }

    frac -> numerador  =  numerador;
    frac -> denominador  =  denominador;

    return frac;
}


//pre: el denominador nunca es cero (porque así se crean las fracciones)
fraccion_t* fraccion_sumar(fraccion_t* f1, fraccion_t* f2) {

    fraccion_t* frac = malloc(sizeof(fraccion_t));

    if (frac == NULL) {
        return NULL;
    }

    frac->numerador  =  f1->numerador  +  f1->numerador;
    frac->denominador  =  f1->denominador  +  f1->denominador;

    free(f1);
    free(f2);

    return frac;
}


fraccion_t* fraccion_mul(fraccion_t* f1, fraccion_t* f2) {

    fraccion_t* frac = malloc(sizeof(fraccion_t));

    if (frac == NULL) {
        return NULL;
    }

    frac->numerador  =  f1->numerador  *  f1->numerador;
    frac->denominador  =  f1->denominador  *  f1->denominador;

    free(f1);
    free(f2);

    return frac;
}


int fraccion_parte_entera(fraccion_t* fraccion) {

    return (int)(fraccion->numerador / fraccion->denominador);
}


char* fraccion_representacion(fraccion_t* fraccion) {

    bool signo = false, entero = false, cero = false;
    int numerador_aux, denominador_aux;
    int numero_mayor, numero_menor;
    int mcd;
    int numerador_simpl, denominador_simpl;
    size_t tam_string;
    char* frac_simpl;

    // -2/5; 2/-5;  -->>  "-2/5"
    if ((fraccion->numerador < 0  &&  fraccion->denominador > 0) || (fraccion->numerador > 0  &&  fraccion->denominador < 0)) {
       signo = true;
    }
    
    // 0/5; 0/-5  -->>  "0/5"
    if (fraccion->numerador == 0) {
        cero = true;
        signo = false;
    }

    // 2/1  -->>  "2"
    // 2/-1  -->>  "-2"
    if (cero || fraccion->denominador == -1  ||  fraccion->denominador == 1) {
        entero = true;
    }

    // Quitamos los signos
    if (signo  &&  fraccion->numerador < 0) {
        numerador_aux = fraccion->numerador * -1;
    }

    if (signo && fraccion->denominador < 0) {
        denominador_aux = fraccion->denominador* -1;
    }

    // elijo el menor de los e itero hasta la mitad de ese numero más 1, buscando un divisor, luego pregunto si es divisor del mas grande y si es lo guardo en una variable aux

    if (numerador_aux >= denominador_aux) {
        numero_mayor = numerador_aux;
        numero_menor = denominador_aux;
    } else {
        numero_mayor = denominador_aux;
        numero_menor = numerador_aux;
    }

    for (int i=1; i < numero_menor; i++) {
        if (numero_menor % i == 0  &&  numero_mayor % i == 0) {
            mcd = i;
        }
    }

    numerador_simpl = numerador_aux / mcd;
    denominador_simpl = denominador_aux / mcd;

    //Casos: 0/-5, 0/5, -2/1, 2/1, -2/5, 2/5,  
    if (cero) {
        frac_simpl = malloc(2 * sizeof(char));
        frac_simpl[0] = '0';
        frac_simpl[1] = '\0';
        return frac_simpl;
    }

    // arreglar concatenacion de strings
    if (entero) {

        char* numerador_s  =  numerador_simpl + '0';
        tam_string = strlen(numerador_s) + 1;
        if (signo) tam_string++;

        frac_simpl = malloc(tam_string * sizeof(char));

        if (signo) {
            frac_simpl = '-' + numerador_s + '\0';
        } else {
            frac_simpl = numerador_s + '\0';
        }

        return frac_simpl;

    } else {

        char* numerador_s  =  numerador_simpl + '0';
        char* denominador_s  =  denominador_simpl + '0';

        if (signo) {
            tam_string  =  1 + strlen(numerador_s) + 1 + strlen(denominador_s) + 1;
        } else {
            tam_string  =  strlen(numerador_s) + 1 + strlen(denominador_s) + 1;
        }

        frac_simpl = malloc(tam_string * sizeof(char));

        if (signo) {
            frac_simpl  =  '-' + numerador_s + '/' + denominador_s + '\0';
        } else {
            frac_simpl  =  numerador_s + '/' + denominador_s + '\0';
        }
        
        return frac_simpl;
    }
}


void fraccion_destruir(fraccion_t* fraccion) {
    free(fraccion);
}
