import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
 

##########################################################


from grafo import Grafo
import heapq


# Complejidad: O(E * log V), con E = aristas y V = vertices
# (Cambiar 'es_dirigido' a 'True' para tests)
def mst_prim(grafo: Grafo) -> Grafo:
    arbol = Grafo(False, grafo.obtener_vertices()) 

    heap = list()

    v = grafo.vertice_aleatorio()

    visitados = set()
    visitados.add(v)

    for w in grafo.adyacentes(v):
        heapq.heappush(heap, [grafo.peso_arista(v, w), (v, w)])

    while heap:
        arista  = heapq.heappop(heap)
        peso    = arista[0]
        (v, w)  = arista[1]

        if w in visitados:
            continue

        arbol.agregar_arista(v, w, peso)
        visitados.add(w)

        # Me fijo en la arista de destino (w)
        for x in grafo.adyacentes(w):
            if x not in visitados:
                heapq.heappush(heap, [grafo.peso_arista(w, x), (w, x)])
    
    return arbol


def main() -> None:
    grafo = Grafo(False, ["0", "1", "2", "3", "4", "5", "6", "7"])

    grafo.agregar_arista("0", "3", 6)
    grafo.agregar_arista("0", "2", 3)
    grafo.agregar_arista("2", "5", 3)
    grafo.agregar_arista("1", "6", 4)
    grafo.agregar_arista("1", "5", 1)
    grafo.agregar_arista("5", "7", 8)
    grafo.agregar_arista("6", "4", 2)
    grafo.agregar_arista("1", "0", 6)
    grafo.agregar_arista("2", "6", 5)
    grafo.agregar_arista("2", "4", 6)

    mst = mst_prim(grafo)

    print(mst.map)


main()
