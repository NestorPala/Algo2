from grafo import Grafo
from collections import deque


def bfs(grafo: Grafo, origen: str) -> set:

    cola = deque()
    cola.appendleft(origen)

    padres = dict()
    padres[origen] = None

    orden = dict()
    orden[origen] = 0

    visitados = set()
    visitados.add(origen)

    while cola:
        v = cola.pop()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                cola.appendleft(w)

    return padres, orden