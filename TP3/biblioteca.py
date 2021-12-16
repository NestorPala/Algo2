from grafo import Grafo
from collections import deque


def bfs(grafo, origen):
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

