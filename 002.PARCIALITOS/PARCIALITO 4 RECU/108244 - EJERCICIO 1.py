from grafo import Grafo


def caminos_largo_n_(grafo: Grafo, n, v, contador_pasos, caminos, camino_actual):

    # Solución encontrada
    if contador_pasos > n:
        return camino_actual

    camino_actual = list()
    camino_actual.append(v)

    # Avanzo si puedo
    # La solucion parcial válida sería que el vértice actual tenga adyacentes y contador_pasos < n
    for w in grafo.adyacentes(v):
        camino_actual.append(w)
        aux_camino = list()
        aux_camino = caminos_largo_n_(grafo, n, w, contador_pasos + 1, caminos, camino_actual)

        if aux_camino != []:
            caminos.append(aux_camino)

    # No encontré una solucion en este camino
    if contador_pasos < n:
        return []


def caminos_largo_n(grafo: Grafo, n):
    caminos = list()
    caminos_largo_n_(grafo, n, grafo.vertice_aleatorio(), 0, caminos, None)
    return caminos
