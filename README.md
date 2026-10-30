# Torneo de Komikan Concurrente
 
Tarea 2 - Benjamín Paredes y Christián Pereira
 
## Compilar con:
 
```
g++ -Wall -Wextra -std=c++17 torneo.cpp -o torneo -pthread
```
 
## Ejecutar con:
 
```
./torneo N M [config.txt]
```
 
- `N`: cantidad de jugadores que llegan al torneo
- `M`: cantidad maxima de mesas jugando al mismo tiempo
- `config.txt`: archivo de configuracion (opcional)

## Archivo de configuracion
 
Cada linea tiene un nombre y un valor:
 
```
min_ms 50
max_ms 300
seed 42
puma 15
perro 13
perro 17
perro 18
...
```
 
- `min_ms` y `max_ms`: retardo minimo y maximo de cada movimiento (en milisegundos)
- `seed`: semilla del generador aleatorio
- `puma`: nodo donde parte el puma
- `perro`: nodo donde parte un perro (se escribe una linea por cada uno, tienen que ser 12)
Si el archivo no trae `puma` ni `perro`, o no se entrega config, se usa una
posicion inicial fija (puma al centro y perros abajo). Si trae la disposicion pero
esta mal (nodo fuera del tablero, no son 12 perros o dos fichas en el mismo nodo),
el programa avisa y termina.
 
Hay un ejemplo con otra disposicion en `config_prueba.txt`.
 
## Tablero
 
El tablero tiene 28 nodos: la grilla de 5x5 mas el triangulo (trono) de arriba.
 
- nodo 0: trono
- nodos 1 y 2: nivel de abajo del trono
- nodos 3 a 27: grilla de 5x5, por filas (nodo = 3 + fila * 5 + columna)
Cada partida (mesa) tiene su propio tablero.
 