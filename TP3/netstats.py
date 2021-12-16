import sys
from grafo import Grafo
# python3 ./netstats.py wiki-reducido-75000.tsv


# borrar luego los que no usemos
COMANDOS = ("listar_operaciones", "camino", "diametro", "rango", "navegación", "comunidad", "conectados", "lectura", "clustering", "mas_importantes",  "ciclo")


def ciclo(grafo):
    pass


def mas_importantes(grafo):
    pass


def clustering(grafo):
    pass


def lectura(grafo):
    pass


def conectados(grafo):
    pass


def comunidad(grafo):
    pass


def navegacion(grafo):
    pass


def rango(grafo):
    pass


def diametro(grafo):
    pass


def camino(grafo):
    print("camino") #debug

    pass


def listar_operaciones():
    for comando in COMANDOS:
        print(comando)


def cargar_grafo_wiki(ruta_archivo):

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
    
    return wiki


def main():
    inputs = sys.argv
    arguments = inputs[1:]
    ruta_archivo = arguments[0]
    wiki = cargar_grafo_wiki(ruta_archivo)

    while(True):
        comando = input()
        
        if comando not in COMANDOS:
            continue

        if   comando == "listar_operaciones": listar_operaciones()
        elif comando == "camino":             camino(wiki)
        elif comando == "diametro":           diametro(wiki)
        elif comando == "rango":              rango(wiki)
        elif comando == "navegación":         navegacion(wiki)
        elif comando == "comunidad":          comunidad(wiki)
        elif comando == "conectados":         conectados(wiki)
        elif comando == "lectura":            lectura(wiki)
        elif comando == "clustering":         clustering(wiki)
        elif comando == "mas_importantes":    mas_importantes(wiki)
        elif comando == "ciclo":              ciclo(wiki)


main()
