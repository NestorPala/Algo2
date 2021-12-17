import random


PESO_ARISTA_DEFAULT = "U"
PESO_ARISTA_DESCONECTADOS = None


class Grafo:

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

    
    def __bool__(self):
        return len(self.map) > 0


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


    def obtener_vertices(self): #O(V)
        vertices = list()

        for v in self:
            vertices.append(v)

        return vertices               
       

    def vertice_aleatorio(self): #O(1)
        vertices = self.obtener_vertices()
        return random.choice(vertices)


    def agregar_arista(self, v, w, peso = PESO_ARISTA_DEFAULT): #O(1)
        if v not in self:
            return
        
        self.map[v][w] = peso

        if w in self and not self.es_dirigido:
            self.map[w][v] = peso
        

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


    def peso_arista(self, v, w): #O(1)
        if v not in self or w not in self:
            return PESO_ARISTA_DESCONECTADOS

        try:
            return self.map[v][w]
        except KeyError:
            return PESO_ARISTA_DESCONECTADOS
