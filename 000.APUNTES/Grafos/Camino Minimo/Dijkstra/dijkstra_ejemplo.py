import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
parentdir = os.path.dirname(parentdir)
sys.path.append(parentdir)
 

##########################################################


from grafo import Grafo
from collections import deque
import math
import heapq


# DIJKSTRA ALGORITMO PRINCIPAL
# (Obtiene el camino mínimo de cada vértice y la distancia mínima de cada vértice)
# Complejidad: O(V + E log V)
def camino_minimo_dijkstra(grafo: Grafo, origen: str) -> set:
    dist = dict()
    for v in grafo: dist[v] = math.inf  
    dist[origen] = 0

    heap = list()
    heapq.heappush(heap, [0, origen])

    padres = dict()
    padres[origen] = None

    while heap:
        v = heapq.heappop(heap)[1]

        for w in grafo.adyacentes(v):
            if dist[v] + grafo.peso_arista(v, w)  <  dist[w]:
                dist[w] = dist[v] + grafo.peso_arista(v, w)
                padres[w] = v
                heapq.heappush(heap, [dist[w], w])
    
    return padres, dist


# Obtenemos uno solo de todos los caminos mínimos
#    Grafos dirigidos:  ['a', 'b', 'c']   ==>   "a"  --->  "b"  --->  "c"
# Grafos no dirigidos:  ['a', 'b', 'c']   ==>   "a"  <-->  "b"  <-->  "c"
def camino_minimo_entre(grafo: Grafo, origen: str, destino: str) -> list:
    padres, _ = camino_minimo_dijkstra(grafo, origen)

    camino_minimo = list()
    camino_minimo.append(destino)

    p = padres[destino]
    
    while p:
        camino_minimo.append(p)
        p = padres[p]
    
    camino_minimo.reverse()

    return camino_minimo


def main() -> None:
    grafo = Grafo(False, [str(i) for i in range(8)])

    grafo.agregar_arista("0", "3", 8)
    grafo.agregar_arista("3", "5", 9)
    grafo.agregar_arista("5", "7", 6)
    grafo.agregar_arista("7", "6", 2)
    grafo.agregar_arista("7", "4", 1)
    grafo.agregar_arista("6", "4", 8)
    grafo.agregar_arista("4", "2", 1)
    grafo.agregar_arista("4", "0", 9)
    grafo.agregar_arista("2", "0", 3)
    grafo.agregar_arista("2", "1", 9)
    grafo.agregar_arista("1", "5", 4)
    grafo.agregar_arista("5", "2", 7)

    padres, dist = camino_minimo_dijkstra(grafo, "0")

    print(padres)
    print(dist)

    camino = camino_minimo_entre(grafo, "0", "6")

    print(camino)


main()
