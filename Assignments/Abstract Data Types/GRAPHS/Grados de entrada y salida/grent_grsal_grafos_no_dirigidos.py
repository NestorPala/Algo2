#GRADOS DE ENTRADA/SALIDA EN GRAFOS NO DIRIGIDOS


from grafo import Grafo


def grado_vertices(grafo):
    grados = dict()

    for v in grafo:
        grados = len(grafo.adyacentes(v))
    
    return grados
    