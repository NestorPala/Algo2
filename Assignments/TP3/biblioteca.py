import math
from grafo import Grafo
from collections import deque
import sys


sys.setrecursionlimit(150000)


# Recorre todas las zonas del grafo a las que se pueden llegar desde un vertice especifico.
# En caso de querer recorrer el grafo completo se debe llamar a esta funcion CANTIDAD_DE_VERTICES veces.
def bfs(grafo: Grafo, origen):
    padres = dict()
    orden = dict()
    visitados = set()
    cola = deque()

    padres[origen] = None
    orden[origen] = 0
    visitados.add(origen)
    cola.appendleft(origen)

    while cola:
        v = cola.pop()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                cola.appendleft(w)

    return padres, orden


# Construye una lista con el camino desde el origen del grafo hasta el vertice elegido.
def obtener_camino(padres, vertice):
    camino = list()

    while (vertice != None):
        camino.append(vertice)
        try:
            vertice = padres[vertice]
        except KeyError:
            vertice = None

    camino.reverse()
    return camino


# Dado un camino de vertices de un grafo, imprime su recorrido con flechas.
def imprimir_camino(camino, mostrar_costo):
    largo_camino = len(camino)

    if largo_camino > 1:
        for i in range(largo_camino):
            print(camino[i], end="")
            if i < largo_camino - 1:
                print(" -> ", end="")

        print("\n",end="")

        if mostrar_costo:
            print(f"Costo: {largo_camino - 1}\n", end="")    
    else:
        if mostrar_costo:
            print(f"No se encontro recorrido\n", end="")
        else:
            print(camino[0])


'''
    Dado un diccionario de orden, obtiene el primer vertice con el mayor orden, y su orden.

    Hablamos de "primer" porque desde Python 3.7 los elementos se conservan en 
    el diccionario en el orden en el que se insertaron.
'''
def obtener_vertice_diametro_maximo(orden):

    vertice_max = None
    orden_max = -math.inf

    for v in orden:
        if orden[v] > orden_max:
            orden_max = orden[v]
            vertice_max = v

    return vertice_max, orden_max


# AUXILIAR OBTENER_CFCS
def obtener_cfcs_(grafo, v, cfcs, elemento_cfc, visitados, apilados, pila, contador_orden, orden, mas_bajo):

    orden[v] = contador_orden[0]
    contador_orden[0] += 1
    mas_bajo[v] = orden[v]
    visitados.add(v)

    pila.append(v)
    apilados.add(v)

    for w in grafo.adyacentes(v):

        # Evitamos las autoreferencias
        if v == w:
            continue

        if w not in visitados:
            obtener_cfcs_(grafo, w, cfcs, elemento_cfc, visitados, apilados, pila, contador_orden, orden, mas_bajo)
        if w in apilados:
            #mas_bajo[v] = min(mas_bajo[v], mas_bajo[w])
            if mas_bajo[w] < mas_bajo[v]:
                mas_bajo[v] = mas_bajo[w]
    
    if orden[v] == mas_bajo[v] and len(pila) > 0:
        cfc = list()

        while True:
            w = pila.pop(len(pila) - 1)
            cfc.append(w)
            elemento_cfc[w] = len(cfcs)
            apilados.remove(w)

            if v == w:
                break

        cfcs.append(cfc)
        

def obtener_cfcs(grafo, origen, cfcs, elemento_cfc):

    visitados = set()
    apilados = set()
    pila = list()

    contador_orden = list() #de un solo elemento, aprovechamos el pasaje por referencia :D
    contador_orden.append(0)

    orden = dict()
    mas_bajo = dict()

    obtener_cfcs_(grafo, origen, cfcs, elemento_cfc, visitados, apilados, pila, contador_orden, orden, mas_bajo)


# AUXILIAR
def obtener_grados_entrada(grafo: Grafo):
    grent = dict()

    for v in grafo:
        grent[v] = 0

    for v in grafo:
        for w in grafo.adyacentes(v):
            grent[w] += 1

    return grent


# Obtiene el orden topologico de todo el grafo, o de un subconjunto del mismo
def orden_topologico(grafo: Grafo, vertices: set):
    grent = obtener_grados_entrada(grafo)
    cola = deque()

    for v in grafo:
        if grent[v] == 0:
            cola.appendleft(v)

    orden = list()

    while len(cola) > 0:
        v = cola.pop()

        if v in vertices:
            orden.append(v)

        for w in grafo.adyacentes(v):
            grent[w] -= 1

            if grent[w] == 0:
                cola.appendleft(w)

    return orden
