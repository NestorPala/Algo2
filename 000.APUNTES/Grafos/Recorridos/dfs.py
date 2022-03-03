# Formato del grafo: {    V1: {A11: P11,  A12: P12,  ...},    V2: {A21: P21,  A22: P22,  ...},    ...    }
# (V: vertice, A: arista, P: peso)


from grafo import Grafo


def _dfs(grafo: Grafo, v: str, orden: dict, padres: dict, visitados: set) -> None:
    for w in grafo.adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            padres[w] = v
            visitados.add(w)

            _dfs(grafo, w, orden, padres, visitados)


def dfs(grafo, origen) -> set:

    orden = dict()
    orden[origen] = 0

    padres = dict()
    padres[origen] = None

    visitados = set()
    visitados.add(origen)

    _dfs(grafo, origen, orden, padres, visitados)

    return orden, padres
