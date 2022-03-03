import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
 

##########################################################


from grafo import Grafo
from collections import deque


# AUXILIAR
def obtener_grados_entrada(grafo: Grafo) -> dict:
    grent = dict()

    for v in grafo:
        grent[v] = 0

    for v in grafo:
        for w in grafo.adyacentes(v):
            grent[w] += 1

    return grent


def orden_topologico(grafo: Grafo) -> list:
    grent = obtener_grados_entrada(grafo)
    cola = deque()
    orden = list()

    for v in grafo:
        if grent[v] == 0:
            cola.appendleft(v)

    while len(cola) > 0:
        v = cola.pop()
        orden.append(v)

        for w in grafo.adyacentes(v):
            grent[w] -= 1

            if grent[w] == 0:
                cola.appendleft(w)

    return orden
