# Torneo de Komikan Concurrente
 
Tarea 2 - Benjamín Paredes y Christián Pereira
 
Simulación de un torneo de Komikan en tiempo real: llegan N jugadores, un matchmaker los empareja
de a 2 (Puma y Perros) y los asigna a una de M mesas como máximo. En cada mesa, el Puma y los 12 Perros
son hilos que se mueven con retardos aleatorios, sin turnos, y un juez detecta quién gana.
 
## Compilar con:
 
```
g++ -Wall -Wextra -std=c++17 torneo.cpp -o torneo -pthread
```
 
## Ejecutar con:
 
```
./torneo N M [config.txt]
```
 
- `N`: cantidad de jugadores que llegan al torneo
- `M`: cantidad máxima de mesas jugando al mismo tiempo
- `config.txt`: archivo de configuración (opcional)
Ejemplos:
 
```
./torneo 6 2 config.txt
./torneo 2 1 config_prueba.txt
./torneo 2 1 config_abandono.txt
```
 
## Archivo de configuración
 
Cada línea tiene un nombre y un valor:
 
```
min_ms 50
max_ms 300
seed 42
abandona 1
puma 15
perro 13
perro 17
perro 18
...
```
 
- `min_ms` y `max_ms`: retardo mínimo y máximo de cada movimiento (en milisegundos)
- `seed`: semilla del generador aleatorio
- `abandona`: número del jugador que falla durante su partida (opcional)
- `puma`: nodo donde parte el puma
- `perro`: nodo donde parte un perro (una línea por cada uno, tienen que ser 12)
Si el archivo no trae `puma` ni `perro`, o no se entrega config, se usa una posición inicial fija
(puma al centro y perros abajo). Si trae la disposición, pero está mal, el programa avisa y termina.
 
Archivos de ejemplo incluidos:
 
- `config.txt`: la disposición por defecto escrita explícitamente
- `config_prueba.txt`: otra disposición inicial
- `config_abandono.txt`: el jugador 1 abandona su partida
## Tablero
 
El tablero tiene 28 nodos: la grilla de 5x5 más el triángulo (trono) de arriba.
 
- nodo 0: trono
- nodos 1 y 2: nivel de abajo del trono
- nodos 3 a 27: grilla de 5x5, por filas (nodo = 3 + fila * 5 + columna)
```
      0
    1   2
3  4  5  6  7
8  9 10 11 12
13 14 15 16 17
18 19 20 21 22
23 24 25 26 27
```
 
El Puma gana con 6 capturas y los Perros ganan si el Puma queda sin movimientos. El Puma salta en
línea recta, incluidas las rectas que pasan por el triángulo.
 
## Decisiones de sincronización:
 
- **Cola de jugadores:** buffer circular con mutex y variables de condición (main produce, el matchmaker consume).
- **Límite de mesas:** semáforo `mesas` inicializado en M.
- **Mesas:** cada partida tiene su propio tablero y un mutex propio y hay un hilo juez por mesa y 13 hilos de ficha.
- **Abandono:** el jugador marcado en `abandona` falla, el juez cierra la partida y gana el oponente por abandono.
- **Ctrl+C:** se dejan de aceptar jugadores, las partidas en curso terminan ordenadamente y se liberan
  semáforo, mutex, variables de condición y memoria.

## Cómo probar:
 
- Partida normal: `./torneo 2 1 config.txt`
- Varias mesas: `./torneo 7 3 config.txt` (queda 1 jugador sin pareja)
- Abandono: `./torneo 2 1 config_abandono.txt`
- Ctrl+C: `./torneo 20 2 config.txt` y apretar Ctrl+C a los 2 segundos

## Informe

El informe de diseño y análisis está en [Informe_S_O_Tarea_2.pdf](Informe_S_O_Tarea_2.pdf).