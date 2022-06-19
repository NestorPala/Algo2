from grafo import Grafo
from collections import deque


# Descripcion del orden del algoritmo utilizando un grafo implementado con matriz de adyacencias:
# (Ver anotaciones en el código)

# V = cantidad de vertices del grafo
# E = cantidad de aristas del grafo

# Complejidad final: O(V^3)


def altamente_conectado_(grafo, origen): #O(1)
    visitados = set() #O(1)
    cola = deque() #O(1)

    visitados.add(origen) #O(1)
    cola.appendleft(origen) #O(1)

    resultado = True #O(1)

    while cola: #O(V), porque al final terminamos encolando todos los vértices, tarde o temprano.
        v = cola.pop() #O(1)

        # En matriz de adyacencia esto cuesta O(V^2), siendo V la cantidad de vértices del grafo. Esto ocurre
        # porque por cada fila (vertice) tenemos que recorrer todas sus columnas (adyacentes).
        # Podemos hacer una optimizacion sabiendo que el grafo es no dirigido y por cada fila aumentamos en uno
        # la columna inicial para buscar los adyacentes, asi solo recorremos el "triangulo" superior (o
        # el inferior si en vez de aumentar el inicio aumentamos el final), sin
        # embargo esto deja la complejidad en O( 1/2 * V^2 ), que sigue siendo O(V^2).
        ady = grafo.adyacentes(v) 


        cant_ady = len(ady) #O(V)
        largo = len(grafo) #O(V)

        for w in ady: #O(V - 1) = O(V), suponiendo que el grafo sea completo.
            if w not in visitados: #O(1)
                visitados.add(w) #O(1)
                cola.appendleft(w) #O(1)
            
            indice_altamente_conectado = (0.4) * (1/2) * (largo-1) * largo #O(1)
            
            if cant_ady < indice_altamente_conectado: #O(1)
                resultado = False #O(1)

    return resultado #O(1)


def altamente_conectado(grafo): #O(1)
    return altamente_conectado_(grafo, grafo.vertice_aleatorio()) #O(1)
