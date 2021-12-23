
def numerar(grafo, v, visitados, pesos_restantes, numerados):

    if not pesos_restantes:
        return true

    for w in grafo.adyacentes(v):
        if w not in visitados:
            if not asignar_peso_no_consecutivo(numerados, v, w): #no tuve tiempo para codearlo
                return false
            if not numerar(grafo, w, visitados, pesos_restantes, numerados):
                visitados.remove(w)
                numerados[w] = None
                return false
            return true
            

def numerar_aristas(grafo):

    if not grafo or len(grafo) == 1:
        return {}

    origen = grafo.obtener_vertice_aleatorio()
    visitados = set()
    pesos_restantes = list()
    numerados = dict()

    for i in range len(grafo):
        pesos_restantes.append(i + 1)

    visitados.add(origen)

    numerar(grafo, origen, visitados, pesos_restantes, numerados)
    return numerados
