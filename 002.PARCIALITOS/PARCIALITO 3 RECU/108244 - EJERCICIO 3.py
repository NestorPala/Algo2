# Complejidad del algoritmo: O(V + E), siendo V la cantidad de vertices del grafo y E la cantidad de aristas,
# porque utilizamos recorrido BFS, que se ejecuta en tiempo lineal,
# y en el medio las operaciones adicionales de asignar a particiones cuestan todas en total O(1).
# Luego, el costo de armar el grafo es, para cada particion, O(V/2) (porque el grafo se ha partido en dos,
# literalmente, bipartito), lo cual sigue siendo O(V), lo cual no empeora la complejidad original que es O(V + E).


from collections import deque
from grafo import Grafo


def obtener_proyecciones(grafo, origen, proyecciones, proyecciones_grafos):
    visitados = set()
    cola = deque()

    visitados.add(origen)
    cola.appendleft(origen)

    particion_A = dict()
    particion_B = dict()

    i = -1  # i % 2 == 0: particion A   |   i % 2 == 1: particion B

    while cola:
        v = cola.pop()
        i += 1
        for w in grafo.adyacentes(v):
            if w not in visitados:
                visitados.add(w)
                cola.appendleft(w)
            
            # Asignamos el vertice actual a la particion "A" o "B" segun corresponda, 
            # indicando ademas su adyacente
            if i % 2 == 0:
                proyecciones[v] = "A"
                particion_A[v] = w
            else:
                proyecciones[v] = "B"
                particion_B[v] = w
    
    # Recorremos el diccionario preguntando para cada vertice, su adyacente, luego buscando en la particion del adyacente, el adyacente del adyacente, y este con el original se unen en el nuevo grafo
    for v in particion_A:
        proyecciones_grafos[ proyecciones[v] ].agregar_arista( v, particion_B[ particion_A[v] ] )
    
    for v in particion_B:
        proyecciones_grafos[ proyecciones[v] ].agregar_arista( v, particion_A[ particion_B[v] ] )
    

def obtener_proyeccion_vertice(grafo, v):
    
    # indica si el vertice se encuentra en "A" o en "B"
    proyecciones = dict()

    # contiene los grafos "A" y "B"
    proyecciones_grafos = dict()
    proyecciones_grafos["A"] = Grafo(es_dirigido = False)
    proyecciones_grafos["B"] = Grafo(es_dirigido = False)

    obtener_proyecciones(grafo, grafo.vertice_aleatorio(), proyecciones, proyecciones_grafos)

    # devuelve el grafo "A" o "B" si la proyeccion de v es "A" o "B"
    return proyecciones_grafos[ proyecciones[v] ]
