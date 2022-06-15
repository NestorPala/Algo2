# (ES) - Algoritmo de Tarjan para hallar componentes fuertemente conexas (CFCs) en un grafo dirigido.
# (EN) - Tarjan's Algorithm to find strongly connected components (SCCs) in a directed graph.


import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
 

##########################################################


from grafo import Grafo


# Complejidad: O(V + E), con V la cantidad de vértices del grafo y E su cantidad de aristas.
# Debido a la implementación recursiva, la cantidad de parámetros se vuelve muy grande.
def obtener_cfcs_(grafo: Grafo, v: str, cfcs: dict, elemento_cfc: dict, visitados: set, 
                    apilados: set, pila: list, contador_orden: list, orden: dict, mas_bajo: dict) -> None:

    orden[v] = contador_orden[0]
    contador_orden[0] += 1
    mas_bajo[v] = orden[v]
    visitados.add(v)

    pila.append(v)
    apilados.add(v)

    for w in grafo.adyacentes(v):
        # Evitamos las autoreferencias
        if v == w:
            continue

        if w not in visitados:
            obtener_cfcs_(grafo, w, cfcs, elemento_cfc, visitados, apilados, pila, contador_orden, orden, mas_bajo)
        if w in apilados:
            mas_bajo[v] = min(mas_bajo[v], mas_bajo[w])
    
    if orden[v] == mas_bajo[v] and len(pila) > 0:
        cfc = list()

        while True:
            w = pila.pop(len(pila) - 1)
            cfc.append(w)
            elemento_cfc[w] = len(cfcs)
            apilados.remove(w)

            if v == w:
                break

        cfcs[str(len(cfcs))] = cfc
        

# Devuelve el conjunto de todos los CFCs del grafo que se pueden encontrar según el vértice de inicio.
# Además, devuelve un diccionario indicando, para cada vértice del grafo, a qué CFC pertenece.
def obtener_cfcs(grafo: Grafo, origen: str) -> set:
    cfcs = dict()
    elemento_cfc = dict()
    visitados = set()
    apilados = set()
    pila = list()
    contador_orden = list() #de un solo elemento, aprovechamos el pasaje por referencia :D
    orden = dict()
    mas_bajo = dict()

    contador_orden.append(0)

    obtener_cfcs_(grafo, origen, cfcs, elemento_cfc, visitados, apilados, pila, contador_orden, orden, mas_bajo)

    return cfcs, elemento_cfc


def main() -> None:
    grafo = Grafo(es_dirigido = True, lista_vertices = [str(i) for i in range(8)])
    grafo.agregar_arista("0", "1", peso = None)
    grafo.agregar_arista("1", "3", peso = None)
    grafo.agregar_arista("3", "4", peso = None)
    grafo.agregar_arista("4", "5", peso = None)
    grafo.agregar_arista("5", "7", peso = None)
    grafo.agregar_arista("7", "6", peso = None)
    grafo.agregar_arista("6", "4", peso = None)
    grafo.agregar_arista("3", "2", peso = None)
    grafo.agregar_arista("2", "1", peso = None)

    origen = "0"
    cfcs, elemento_cfc = obtener_cfcs(grafo, origen)

    print("Los CFCs del grafo encontrados a partir del vertice de salida '" + origen + "' son:")
    print(cfcs)

    print("\nLa lista de pertenencia de cada elemento es la siguiente ({'vertice' : N° CFC}):")
    print(elemento_cfc)


main()
