import os, sys
currentdir = os.path.dirname(os.path.realpath(__file__))
parentdir = os.path.dirname(currentdir)
sys.path.append(parentdir)
 

##########################################################


from grafo import Grafo


# ES CONEXO
def dfs(grafo: Grafo, v: str, visitados: set) -> None:
    for w in grafo.adyacentes(v):
        if w not in visitados:
            visitados.add(w)
            dfs(grafo, w, visitados)


# ES CONEXO
def es_conexo(grafo: Grafo) -> bool:
    if not grafo: 
        return True

    visitados = set()

    dfs(grafo, grafo.vertice_aleatorio(), visitados)
    
    return len(visitados) == len(grafo)


def main() -> None:

    # Grafo no dirigido conexo
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

    print(es_conexo(grafo))
    
    #---------------------------------------

    # Grafo no dirigido no conexo (2 componentes conexas)
    grafo_2 = Grafo(False, [str(i) for i in range(8)])

    grafo_2.agregar_arista("0", "3", 8)
    grafo_2.agregar_arista("3", "5", 9)
    grafo_2.agregar_arista("5", "7", 6)
    grafo_2.agregar_arista("7", "4", 1)
    grafo_2.agregar_arista("4", "2", 1)
    grafo_2.agregar_arista("4", "0", 9)
    grafo_2.agregar_arista("2", "0", 3)
    grafo_2.agregar_arista("2", "1", 9)
    grafo_2.agregar_arista("1", "5", 4)
    grafo_2.agregar_arista("5", "2", 7)

    print(es_conexo(grafo_2))


main()
