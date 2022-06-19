
# Si contamos la inicializacion de la matriz como parte de la complejidad, el orden del algoritmo es O(V^2), porque literalmente inicializamos
# la matriz con V * V elementos igualados a cero.

# Si NO CONTARAMOS la inicializacion de la matriz, por ejemplo, asumiendo que ya me pasan una matriz vacia,
# la complejidad del algorimo seria O(V + E), porque visito todos los vertices y aristas como maximo 
# 2 veces (asumiendo que alguna de todas las aristas pueda llegar a ser "bidireccional")


def generar_matriz_incidencia(grafo):
    cant_vertices = len(grafo.obtener_vertices())

    if cant_vertices == 0:
        return None

    matriz = []

    for i in range cant_vertices:
        matriz[i] = []

    for p in range cant_vertices:
        for q in range cant_vertices:
            matriz[p][q] = 0
    
    for v in grafo:
        for w in grafo.adyacentes(v):
            matriz[v][w] = 1
            if v in grafo.adyacentes(w):
                matriz[w][v] = 1
    
    return matriz