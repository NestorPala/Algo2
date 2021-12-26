#!/usr/bin/env python3
import sys
import biblioteca
from grafo import Grafo
import math
import random


COMANDOS = ("listar_operaciones", "camino", "diametro", "navegacion", "comunidad", "conectados")
RECURSION_MAXIMA_NAVEGACION = 20   


def conectados(grafo, parametros, cfcs, elemento_cfc):
    pagina = parametros[0]

    if len(cfcs) == 0:
        biblioteca.obtener_cfcs(grafo, pagina, cfcs, elemento_cfc)
    
    cfc_pagina = cfcs[elemento_cfc[pagina]]

    largo_cfc = len(cfc_pagina)

    for i in range(largo_cfc):
        if i < largo_cfc - 1:
            print(f"{cfc_pagina[i]}, ", end="")
        else:
            print(f"{cfc_pagina[i]} ", end="")

    print("\n",end="")


def comunidad(grafo: Grafo, parametros):
    pagina = parametros[0]
    labels = dict()

    for v in grafo:
        labels[v] = v

    # Obtenemos el diccionario de anti adyacentes (vertices de entrada) para reducir el tiempo de procesamiento
    anti_ady = dict()

    for a in grafo:
        for b in grafo.adyacentes(a):
            if b not in anti_ady:
                anti_ady[b] = list()
            else:
                anti_ady[b].append(a)

    vertices = grafo.obtener_vertices()
    random.shuffle(vertices)
    
    i = 0 
    max_iter = 8 #definimos una cantidad fija de iteraciones para encotrar las comunidades

    while i < max_iter:
        for x in vertices:
            frec_max = (-math.inf, " ")
            frecuencias = dict()

            for s in anti_ady[x]: 
                label = labels[s]
                if label not in frecuencias:
                    frecuencias[label] = 1
                else:
                    frecuencias[label] += 1

                if frecuencias[label] > frec_max[0]:
                    frec_max = (frecuencias[label], label)
            
            labels[x] = frec_max[1]

        i += 1
    
    miembros_comunidad_pagina = list()

    for z in labels:
        if labels[z] == labels[pagina]:
            miembros_comunidad_pagina.append(z)

    largo_comunidad = len(miembros_comunidad_pagina)
        
    for i in range(largo_comunidad):
        if i < largo_comunidad - 1:
            print(f"{miembros_comunidad_pagina[i]}, ", end="")
        else:
            print(f"{miembros_comunidad_pagina[i]} ", end="")


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
        if comando != "listar_operaciones":
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

    # for linea in comandos:
    while True:
        try:
            linea = input()
        except EOFError:
            break

        comando, parametros = obtener_parametros(linea)

        if comando not in COMANDOS:
            continue
        
        if   comando == "listar_operaciones": 
            listar_operaciones()
        elif comando == "camino":             
            camino(wiki, parametros)
        elif comando == "diametro":           
            diametro(wiki)
        elif comando == "navegacion":         
            navegacion(wiki, parametros)
        elif comando == "comunidad":          
            comunidad(wiki, parametros)
        elif comando == "conectados":         
            conectados(wiki, parametros, cfcs, elemento_cfc)


if __name__ == '__main__':
    sys.exit(main())
