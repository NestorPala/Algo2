import sys
import biblioteca
from grafo import Grafo
import math
import os #debug
# python3 ./netstats.py wiki-reducido-75000.tsv


# borrar luego las funciones que no implementemos
COMANDOS = ("listar_operaciones", "camino", "diametro", "rango", "navegacion", "comunidad", "conectados", "lectura", "clustering", "mas_importantes",  "ciclo")

#para navegacion()
RECURSION_MAXIMA = 20   

#debug
    # i += 1 #debug
    # porcentaje = round(i/len(grafo)*100) #debug
    # print(f"({porcentaje} %) probando vertice {i}/{len(grafo)}: {v}\n",end="") #debug


#debug
def limpiar_pantalla() -> None:
    if os.name == "posix":
        os.system ("clear")
    elif os.name == "ce" or os.name == "nt" or os.name == "dos":
        os.system ("cls")


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


def navegacion(grafo: Grafo, parametros):
    vertice_origen = parametros[0]
    camino = list()
    i = 0

    camino.append(vertice_origen)

    try:
        siguiente = (grafo.adyacentes(vertice_origen))[0]
    except:
        siguiente = None
    
    while siguiente:
        if i == 20: break
        camino.append(siguiente)

        try:
            siguiente = (grafo.adyacentes(siguiente))[0]
        except:
            siguiente = None
        
        i += 1
    
    biblioteca.imprimir_camino(camino, False)


def rango(grafo, parametros):
    vertice_inicial = parametros[0]
    cant_saltos = parametros[1]

    padres, orden = biblioteca.bfs(grafo, vertice_inicial)

    vertices_cant_saltos = 0

    for v in orden:
        if orden[v] == int(cant_saltos):
            vertices_cant_saltos += 1

    print(f"{vertices_cant_saltos}\n",end="")


def diametro(grafo: Grafo):
    if not grafo:
        return 0

    diametro_maximo = -math.inf
    vertice_diametro_maximo = ""
    padres_orden_max = None

    for v in grafo:
        padres, orden = biblioteca.bfs(grafo, v)
        w, w_orden = biblioteca.obtener_vertice_diametro_maximo(orden)

        if w_orden > diametro_maximo:
            diametro_maximo = w_orden
            vertice_diametro_maximo = w
            padres_orden_max = padres

    camino = biblioteca.obtener_camino(padres_orden_max, vertice_diametro_maximo)
    biblioteca.imprimir_camino(camino, True)


def camino(grafo, parametros):
    padres, orden = biblioteca.bfs(grafo, parametros[0])
    camino = biblioteca.obtener_camino(padres, parametros[1])
    biblioteca.imprimir_camino(camino, True)


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
        elif comando == "navegacion":         
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
