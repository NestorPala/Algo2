#GRADOS DE ENTRADA Y GRADOS DE SALIDA EN GRAFOS DIRIGIDOS


from grafo import Grafo


def grado_salida(grafo):
    grados_salida = dict()

    for v in grafo:
        grados_salida[v] = len(grafo.adyacentes(v))
    
    return grados_salida


def grado_entrada(grafo):
    grados_entrada = dict()

    for v in grafo:
        grados_entrada[v] = 0
    
    for v in grafo:
        for w in grafo.adyacentes(v):
            if w not in grados_entrada:
                grados_entrada[w] = 1
            else:
                grados_entrada[w] += 1
    
    return grados_entrada
    