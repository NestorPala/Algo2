# Recibo grafo pesado y no dirigido + arbol --> devuelvo true/false si el arbol es/no es MST del grafo
# (Nota: considerar todas las condiciones para que el arbol sea MST)


from grafo import Grafo
from collections import deque
import heapq


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


# (Es como un BFS pero sin "padres" ni "orden")
def calcular_suma_arbol(arbol: Grafo) -> int:
    suma_arbol = 0

    origen = arbol.vertice_aleatorio()

    aristas_vistadas = set()

    cola = deque()
    cola.appendleft(origen)

    visitados = set()
    visitados.add(origen)

    while cola:
        v = cola.pop()

        for w in arbol.adyacentes(v):
            if w not in visitados:
                cola.appendleft(w)
                visitados.add(w)
            
            if (v, w) not in aristas_vistadas:
                suma_arbol += arbol.peso_arista(v, w)
                aristas_vistadas.add((v, w))
                aristas_vistadas.add((w, v))

    return suma_arbol


def contiene_suma_minima_pesos(arbol: Grafo, grafo: Grafo) -> bool:
    suma_pesos_grafo = calcular_suma_arbol(mst_prim(grafo))
    suma_pesos_arbol = calcular_suma_arbol(arbol)

    return suma_pesos_arbol == suma_pesos_grafo


# (Es como un BFS pero sin "orden"; "padres" sí es necesario para detectar el ciclo)
def contiene_ciclos(grafo: Grafo) -> bool:
    origen = grafo.vertice_aleatorio()

    cola = deque()
    cola.appendleft(origen)

    padres = dict()
    padres[origen] = None

    visitados = set()
    visitados.add(origen)

    while cola:
        v = cola.pop()
        
        for w in grafo.adyacentes(v):
            if w not in visitados:
                cola.appendleft(w)
                padres[w] = v
                visitados.add(w)
            else:
                # Acá detectamos si hay ciclo
                if w != padres[v]:
                    return True
    
    return False


# (Es como un BFS pero sin "padres" ni "orden")
def contar_aristas(grafo: Grafo) -> int:
    contador_aristas = 0

    origen = grafo.vertice_aleatorio()

    cola = deque()
    cola.appendleft(origen)

    visitados = set()
    visitados.add(origen)

    while cola:
        v = cola.pop()
        
        for w in grafo.adyacentes(v):
            if w not in visitados:
                cola.appendleft(w)
                visitados.add(w)
                contador_aristas += 1

    return contador_aristas


def es_arbol(grafo: Grafo) -> bool:
    cant_aristas = contar_aristas(grafo)
    cant_vertices = len(grafo)

    if not (cant_aristas == cant_vertices - 1):
        return False
    
    if contiene_ciclos(grafo):
        return False

    return True


def contiene_vertices_grafo(arbol: Grafo, grafo: Grafo) -> bool:
    for v in arbol:
        if v not in grafo:
            return False
    
    for w in grafo:
        if w not in arbol:
            return False
    
    return True


def es_mst_del_grafo(arbol: Grafo, grafo: Grafo) -> bool:
    if not contiene_vertices_grafo(arbol, grafo):
        return False
    
    if not es_arbol(arbol):
        return False
    
    if not contiene_suma_minima_pesos(arbol, grafo):
        return False
    
    return True


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

    arbol_MST = Grafo(False, ["0", "1", "2", "3", "4", "5", "6", "7"])
    arbol_MST.agregar_arista("0", "3", 6)
    arbol_MST.agregar_arista("0", "2", 3)
    arbol_MST.agregar_arista("2", "5", 3)
    arbol_MST.agregar_arista("1", "6", 4)
    arbol_MST.agregar_arista("1", "5", 1)
    arbol_MST.agregar_arista("5", "7", 8)
    arbol_MST.agregar_arista("6", "4", 2)

    arbol_NO_MST = Grafo(False, ["0", "1", "2", "3", "4", "5", "6", "7"])
    arbol_NO_MST.agregar_arista("0", "3", 6)
    arbol_NO_MST.agregar_arista("0", "2", 3)
    arbol_NO_MST.agregar_arista("2", "5", 3)
    arbol_NO_MST.agregar_arista("1", "6", 4)
    arbol_NO_MST.agregar_arista("1", "5", 1)
    arbol_NO_MST.agregar_arista("5", "7", 8)
    arbol_NO_MST.agregar_arista("6", "2", 5)  # esta arista provoca el 'False'
    
    print(es_mst_del_grafo(arbol_MST, grafo))
    print("-------------------------------------------------")
    print(es_mst_del_grafo(arbol_NO_MST, grafo))


main()
