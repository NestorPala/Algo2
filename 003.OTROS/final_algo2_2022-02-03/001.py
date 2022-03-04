# fiuba map: recibo grafo materias + materia --> devuelvo todas las materias que hay que cursar antes
# (grafo dirigido, sin ciclos, no pesado)


from grafo import Grafo
from collections import deque


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


def bfs(grafo: Grafo, origen: str) -> set:
    cola = deque()
    cola.appendleft(origen)

    orden = dict()
    orden[origen] = 0

    padres = dict()
    padres[origen] = None

    visitados = set()
    visitados.add(origen)

    while cola:
        v = cola.pop()
        
        for w in grafo.adyacentes(v):
            if w not in visitados:
                cola.appendleft(w)
                orden[w] = orden[v] + 1
                padres[w] = v
                visitados.add(w)

    return orden, padres


def grafo_invertir_aristas(grafo: Grafo) -> Grafo:
    grafo_nuevo = Grafo(True, grafo.obtener_vertices())

    for v in grafo:
        for w in grafo.adyacentes(v):
            if w:
                grafo_nuevo.agregar_arista(w, v)
    
    return grafo_nuevo


# Formato del dict: {    V1: {A11: P11,  A12: P12,  ...},    V2: {A21: P21,  A22: P22,  ...},    ...    }
# (V: vertice, A: arista, P: peso)
def convertir_dict_a_grafo(grafo: dict, es_dirigido: bool = False) -> Grafo:
    grafo_nuevo = Grafo(es_dirigido, grafo.keys())

    for v in grafo:
        if grafo[v]:
            grafo_nuevo.agregar_arista(v, grafo[v])

    return grafo_nuevo


def fiuba_map_materias_necesarias(listado_materias: Grafo, materia_hasta: str = None) -> list: #ORDEN TOTAL: O(M + C)
    if not listado_materias: #O(1)
        return [] #O(1)
    
    if not materia_hasta: #O(1)
        return listado_materias.obtener_vertices() #O(M)

    if materia_hasta not in listado_materias: #O(1)
        return None #O(1)
    
    listado_materias = grafo_invertir_aristas(listado_materias) #O(M + C)

    _ , dependencias_materia_hasta = bfs(listado_materias, materia_hasta) #O(M + C)
    dependencias_materia_hasta = convertir_dict_a_grafo(dependencias_materia_hasta, True) #O(M)

    materias_necesarias = orden_topologico(dependencias_materia_hasta) #O(M + C)

    return materias_necesarias[0 : len(materias_necesarias)-1 : 1] #O(1)

    
def main() -> None:
    fiuba_map = Grafo(True, ["cbc", "f1", "f2", "estruct", "am2", "alg2", 
                            "numerico1", "algo1", "algo2", "algo3", "sistop"])

    fiuba_map.agregar_arista("cbc", "f1")
    fiuba_map.agregar_arista("f1", "f2")
    fiuba_map.agregar_arista("f2", "estruct")
    fiuba_map.agregar_arista("estruct", "sistop")
    fiuba_map.agregar_arista("cbc", "am2")
    fiuba_map.agregar_arista("am2", "alg2")
    fiuba_map.agregar_arista("alg2", "numerico1")
    fiuba_map.agregar_arista("cbc", "algo1")
    fiuba_map.agregar_arista("algo1", "algo2")
    fiuba_map.agregar_arista("algo2", "algo3")
    fiuba_map.agregar_arista("am2", "f2")
    fiuba_map.agregar_arista("algo2", "estruct")
    fiuba_map.agregar_arista("algo2", "numerico1")

    print(fiuba_map_materias_necesarias(fiuba_map, "sistop"))


main()
    