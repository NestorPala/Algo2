from grafo import Grafo
from collections import deque


# AUXILIAR
def obtener_grados_entrada(grafo: Grafo):
    grent = dict()

    for v in grafo:
        grent[v] = 0

    for v in grafo:
        for w in grafo.adyacentes(v):
            grent[w] += 1

    return grent


def orden_topologico(grafo: Grafo):
    grent = obtener_grados_entrada(grafo)
    cola = deque()

    for v in grafo:
        if grent[v] == 0:
            cola.appendleft(v)

    orden = list()

    while len(cola) > 0:
        v = cola.pop()
        orden.append(v)

        for w in grafo.adyacentes(v):
            grent[w] -= 1

            if grent[w] == 0:
                cola.appendleft(w)

    return orden
