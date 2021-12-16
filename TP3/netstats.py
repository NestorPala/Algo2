import sys
from grafo import Grafo
# python3 ./netstats.py wiki-reducido-75000.tsv


def leer_archivo(ruta_archivo):

    wiki = Grafo(True)

    with open(ruta_archivo, 'r', 200, 'utf-8') as archivo:

        # cargamos los vertices
        for linea in archivo:

            clave = ""

            for caracter in linea:
                if caracter == "\t" or caracter == "\n":
                    if clave != "":
                        wiki.agregar_vertice(clave)
                    break

                clave += caracter

        archivo.seek(0)
        
        # cargamos las aristas
        for linea in archivo:

            contador_palabras = 0
            vertice = ""
            nodo_actual = ""

            for caracter in linea:

                if caracter == "\t" or caracter == "\n":
                    contador_palabras += 1
                    
                    if contador_palabras == 1:
                        vertice = nodo_actual
                    else:
                        wiki.agregar_arista(vertice, nodo_actual)

                nodo_actual += caracter

                if caracter == "\t":
                    nodo_actual = ""

            '''
            Pagina1\tLink1\tLink2\n
            Pagina2\tLink3\n
            '''

    print(wiki.map) #debug
    print(len(wiki))

    #print(wiki.adyacentes("España"))

    # grafo2 = Grafo(True)
    # grafo2.agregar_vertice("a")
    # grafo2.agregar_vertice("b")
    # grafo2.agregar_arista("a", "b")
    # print(grafo2.map)
    # grafo2.agregar_arista("b", "a")
    # print(grafo2.map)
            

inputs = sys.argv
arguments = inputs[1:]
ruta_archivo = arguments[0]

leer_archivo(ruta_archivo)
