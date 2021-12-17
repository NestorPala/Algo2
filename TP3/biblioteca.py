import math
from grafo import Grafo
from collections import deque


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
