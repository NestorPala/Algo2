'''
Implementar grafo
    *diccionarios de diccionarios:
        clave: VERTICE V, valor: DICCIONARIO DE ADYACENTES
                                *clave: VERTICE ADYACENTE W
                                *valor: PESO DE LA ARISTA ENTRE V y W


Para un grafo no dirigido, la relacion A <---> B aparece dos veces, tanto en A como en B, es decir, B va a aparecer en
el diccionario de adyacentes de A, y viceversa:
    * Si el grafo es no dirigido, en ambos casos aparecerá el mismo peso
    * Si el grafo es dirigido, en ambos casos podrían aparecer pesos diferentes


Metodos:

    * agregar_vertice(self, v)
    * borrar_vertice(self, v)
    * obtener_vertices(self)                        ==>   Devuelve una lista con todos los vértices del grafo
    * vertice_aleatorio(self)                       ==>   Devuelve un vertice cualquiera de un grafo
    * agregar_arista(self, v, w, peso = 1)          ==>   el resultado será v <--> w
    * borrar_arista(self, v, w)

    * adyacentes(self, v)                           ==>   obtener todos los adyacentes de un vertice
    * estan_unidos(self, v, w)                      ==>   ver si dos vertices son adyacentes
    * peso_arista(self, v, w)


Costo computacional:
    * Memoria: O(V+E)

    * Agregar vertice: O(1)
    * borrar vertice: O(V)
    * Agregar arista: O(1)
    * borrar arista: O(1)
    * Obtener adyacentes de un vertice: O(V)
    * Ver si A esta conectado con B: O(1)
'''

import random


PESO_ARISTA_DEFAULT = "U"
PESO_ARISTA_DESCONECTADOS = None


class Grafo:


    # Peso no conectados: None
    # Peso por defecto:   "PESO_DEFAULT"


    def __init__(self, es_dirigido = False, lista_vertices = None):
        self.map = dict()
        self.es_dirigido = es_dirigido

        if (lista_vertices):
            for v in lista_vertices:
                self.agregar_vertice(v)


    def __len__(self):
        return len(self.map)

    
    def __contains__(self, v):
        return v in self.map


    def __iter__(self):
        return iter(self.map)

    
    def __next__(self):
        return next(self.__iter__)


    def agregar_vertice(self, v): #O(1)
        if v in self:
            return False

        self.map[v] = dict()

        return True


    def borrar_vertice(self, v): #O(V)
        if v not in self:
            return False
        
        self.map.pop(v)

        for i in self:
            self.map[i].pop(v, None)

        return True


    def obtener_vertices(self):
        vertices = list()

        for v in self:
            vertices.append(v)

        return vertices               
       

    def vertice_aleatorio(self):
        vertices = self.obtener_vertices()
        return random.choice(vertices)


    def agregar_arista(self, v, w, peso = PESO_ARISTA_DEFAULT): #O(1)
        if v not in self:
            self.map[v] = dict()
        
        self.map[v][w] = peso


    def borrar_arista(self, v, w): #O(1)
        if v not in self or w not in self:
            return False

        if w not in self.map[v]:
            return False
        
        # obtengo el valor de la arista  v --> w
        peso_ida = self.map[v][w] 

        self.map[v].pop(w)

        if v not in self.map[w]:
            return True
        
        # si el grafo no es dirigido, tanto la arista de ida como de vuelta son la misma (es compartida), entonces la borro de los dos lados
        if not self.es_dirigido:
            self.map[w].pop(v)

        return True


    def adyacentes(self, v): #O(V)
        adyacentes = list()

        for w in self.map[v]:
            adyacentes.append(w)

        return adyacentes     


    def estan_unidos(self, v, w): #O(1)
        if v not in self or w not in self:
            return False

        return w in self.map[v]


    def peso_arista(self, v, w):
        if v not in self or w not in self:
            return PESO_ARISTA_DESCONECTADOS

        return self.map[v][w]



# pruebas (sacar luego)

grafo = Grafo(False, ("a", "b", "c", "d", "e"))

for v in grafo:
    print(f"{v}, ", end = '')

print("\n")
print("a" in grafo)
print("z" in grafo)

print(len(grafo))

#...

