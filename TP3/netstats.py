import sys
import biblioteca
from grafo import Grafo
# python3 ./netstats.py wiki-reducido-75000.tsv


# borrar luego las funciones que no implementemos
COMANDOS = ("listar_operaciones", "camino", "diametro", "rango", "navegación", "comunidad", "conectados", "lectura", "clustering", "mas_importantes",  "ciclo")


def ciclo(grafo, parametros):
    pass


def mas_importantes(grafo, parametros):
    pass


def clustering(grafo, parametros):
    pass


def lectura(grafo, parametros):
    pass


def conectados(grafo, parametros):
    pass


def comunidad(grafo, parametros):
    pass


def navegacion(grafo, parametros):
    pass


def rango(grafo, parametros):
    pass


def diametro(grafo):
    pass


def camino(grafo, parametros):
    padres, orden = biblioteca.bfs(grafo, parametros[0])

    valor = parametros[1]
    camino = list()

    while (valor != None):
        camino.append(valor)
        try:
            valor = padres[valor]
        except KeyError:
            valor = None

    camino.reverse()
    largo_camino = len(camino)

    if largo_camino > 1:
        for i in range(largo_camino):
            print(camino[i], end="")
            if i < largo_camino - 1:
                print(" -> ", end="")
        print("\n",end="")

    if largo_camino > 1:
        print(f"Costo: {largo_camino - 1}\n", end="")
    else:
        print(f"No se encontro recorrido\n", end="")


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


def obtener_parametros(cadena):
    comando = ""
    inicio_parametros = 0

    for caracter in cadena:
        if caracter == "\n" and " " not in cadena:
            inicio_parametros = -1
            break

        if caracter == " ": 
            break

        comando += caracter
        inicio_parametros += 1

    if inicio_parametros == -1:
        return comando, list()

    cadena_parametros = cadena[inicio_parametros + 1:]
    parametros = cadena_parametros.split(",")

    return comando, parametros
    

def main():
    inputs = sys.argv
    arguments = inputs[1:]
    ruta_archivo = arguments[0]
    wiki = cargar_grafo_wiki(ruta_archivo)

    #print(wiki.map) #debug

    while(True):
        cadena = input()
        comando, parametros = obtener_parametros(cadena)

        #print(comando, parametros) #debug

        if comando not in COMANDOS:
            continue
        
        # borrar luego las funciones que no implementemos
        if   comando == "listar_operaciones": 
            listar_operaciones()
        elif comando == "camino":             
            camino(wiki, parametros)
        elif comando == "diametro":           
            diametro(wiki)
        elif comando == "rango":              
            rango(wiki, parametros)
        elif comando == "navegación":         
            navegacion(wiki, parametros)
        elif comando == "comunidad":          
            comunidad(wiki, parametros)
        elif comando == "conectados":         
            conectados(wiki, parametros)
        elif comando == "lectura":            
            lectura(wiki, parametros)
        elif comando == "clustering":         
            clustering(wiki, parametros)
        elif comando == "mas_importantes":    
            mas_importantes(wiki, parametros)
        elif comando == "ciclo":              
            ciclo(wiki, parametros)


main()
