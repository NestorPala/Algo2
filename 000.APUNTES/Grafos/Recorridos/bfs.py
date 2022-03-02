# Formato del grafo: {    V1: {A11: P11,  A12: P12,  ...},    V2: {A21: P21,  A22: P22,  ...},    ...    }
# (V: vertice, A: arista, P: peso)


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
                cola.appendleft(w)
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)

    return padres, orden
