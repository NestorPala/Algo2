#!/usr/bin/env python3
import sys
import biblioteca
from grafo import Grafo
import math
import random


# borrar luego las funciones que no implementemos
COMANDOS = ("listar_operaciones", "camino", "diametro", "rango", "navegacion", "comunidad", "conectados")
RECURSION_MAXIMA_NAVEGACION = 20   


def conectados(grafo, parametros, cfcs, elemento_cfc):
    pagina = parametros[0]

    if len(cfcs) == 0:
        biblioteca.obtener_cfcs(grafo, pagina, cfcs, elemento_cfc)
    
    cfc_pagina = cfcs[elemento_cfc[pagina]]

    for v in cfc_pagina:
        print(f"{v}, ", end="")
    print("\n",end="")


# Prototipo: una sola iteración tarda más de una hora. 
# En una versión temprana de este algoritmo, usando los adyacentes en vez de las entradas, 
# aunque el algoritmo no es el correcto, 
# extrañamente el resultado obtenido es casi igual al ejemplo y sale casi al instante.
def comunidad(grafo: Grafo, parametros):
    pagina = parametros[0]
    labels = dict()

    for v in grafo:
        labels[v] = v

    i = 0 #definimos una cantidad fija de iteraciones para encotrar las comunidades

    while i < 5:
        recorrido = random.shuffle(grafo.obtener_vertices())

        for x in recorrido:
            frecuencias = dict()
            frec_max = (-math.inf, " ")

            for w in grafo: 
                if grafo.estan_unidos(w, x):
                    label = labels[w]
                    if label not in frecuencias:
                        frecuencias[label] = 1
                    else:
                        frecuencias[label] += 1

                    if frecuencias[label] > frec_max[0]:
                        frec_max = (frecuencias[label], label)
            
            labels[x] = frec_max[1]
        
        miembros_comunidad_pagina = list()
        i += 1
    
    for z in labels:
        if labels[z] == labels[pagina]:
            miembros_comunidad_pagina.append(z)
    
    for miembro in miembros_comunidad_pagina:
        if miembro:
            print(f"{miembro}, ",end="")


def navegacion(grafo: Grafo, parametros):
    vertice_origen = parametros[0]
    camino = list()
    i = 0

    camino.append(vertice_origen)

    try:
        siguiente = (grafo.adyacentes(vertice_origen))[0]
    except:
        siguiente = None
    
    while siguiente:
        if i == RECURSION_MAXIMA_NAVEGACION: break
        camino.append(siguiente)

        try:
            siguiente = (grafo.adyacentes(siguiente))[0]
        except:
            siguiente = None
        
        i += 1
    
    biblioteca.imprimir_camino(camino, False)


def rango(grafo, parametros):
    vertice_inicial = parametros[0]
    cant_saltos = parametros[1]

    padres, orden = biblioteca.bfs(grafo, vertice_inicial)

    vertices_cant_saltos = 0

    for v in orden:
        if orden[v] == int(cant_saltos):
            vertices_cant_saltos += 1

    print(f"{vertices_cant_saltos}\n",end="")


def diametro(grafo: Grafo):
    if not grafo:
        return 0

    diametro_maximo = -math.inf
    vertice_diametro_maximo = ""
    padres_orden_max = None

    for v in grafo:
        padres, orden = biblioteca.bfs(grafo, v)
        w, w_orden = biblioteca.obtener_vertice_diametro_maximo(orden)

        if w_orden > diametro_maximo:
            diametro_maximo = w_orden
            vertice_diametro_maximo = w
            padres_orden_max = padres

    camino = biblioteca.obtener_camino(padres_orden_max, vertice_diametro_maximo)
    biblioteca.imprimir_camino(camino, True)


def camino(grafo, parametros):
    padres, orden = biblioteca.bfs(grafo, parametros[0])
    camino = biblioteca.obtener_camino(padres, parametros[1])
    biblioteca.imprimir_camino(camino, True)


def listar_operaciones():
    for comando in COMANDOS:
        print(comando)


def cargar_grafo_wiki(ruta_archivo):

    wiki = Grafo(True)

    try:
        with open(ruta_archivo, 'r', 200, 'utf-8') as archivo:

            # cargamos los vertices
            for linea in archivo:
                clave = ""

                for caracter in linea:
                    if caracter == "\t" or caracter == "\n":
                        if clave != "":
                            wiki.agregar_vertice(clave)
                        break

                    clave += caracter

            archivo.seek(0)
            
            # cargamos las aristas
            for linea in archivo:
                contador_palabras = 0
                vertice = ""
                nodo_actual = ""

                for caracter in linea:
                    if caracter == "\t" or caracter == "\n":
                        contador_palabras += 1
                        
                        if contador_palabras == 1:
                            vertice = nodo_actual
                        else:
                            wiki.agregar_arista(vertice, nodo_actual)

                    nodo_actual += caracter

                    if caracter == "\t":
                        nodo_actual = ""
    except Exception:
        wiki = None
    
    return wiki


def obtener_parametros(cadena):
    comando = ""
    inicio_parametros = 0

    for caracter in cadena:
        if caracter == "\n" and " " not in cadena:
            inicio_parametros = -1
            break

        if caracter == " ": 
            break

        comando += caracter
        inicio_parametros += 1

    if inicio_parametros == -1:
        return comando, list()

    cadena_parametros = cadena[inicio_parametros + 1:]
    parametros = cadena_parametros.split(",")

    return comando, parametros
    

def main():
    inputs = sys.argv
    arguments = inputs[1:]
    ruta_archivo = arguments[0]

    wiki = cargar_grafo_wiki(ruta_archivo)
    if not wiki: return

    # para la funcion conectados
    cfcs = list()
    elemento_cfc = dict()

    while(True):

        try:
            cadena = input()
        except EOFError:
            return

        comando, parametros = obtener_parametros(cadena)

        if comando not in COMANDOS:
            continue
        
        if   comando == "listar_operaciones": 
            listar_operaciones()
        elif comando == "camino":             
            camino(wiki, parametros)
        elif comando == "diametro":           
            diametro(wiki)
        elif comando == "rango":              
            rango(wiki, parametros)
        elif comando == "navegacion":         
            navegacion(wiki, parametros)
        elif comando == "comunidad":          
            comunidad(wiki, parametros)
        elif comando == "conectados":         
            conectados(wiki, parametros, cfcs, elemento_cfc)


if __name__ == '__main__':
    sys.exit(main())
