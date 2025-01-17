# Cppong

Este es un juego de **Pong** implementado en **C++** utilizando la librería **Raylib**. Es mi primer proyecto en C++ y sirvió como un ejercicio para aprender conceptos de programación de juegos, manejo de entradas, colisiones y generación de objetos dinámicos.

---

## TECNOLOGÍAS Y HERRAMIENTAS

- **Lenguaje de programación:** C++
- **Librería gráfica:** [Raylib](https://www.raylib.com) - Utilizada para el manejo de gráficos, entradas y ventanas.
- **Compilador recomendado:** GCC o cualquier compilador compatible con C++11 o superior.

---

## ESTRUCTURA DEL CÓDIGO

El proyecto está estructurado en un solo archivo fuente (`main.cpp`). Aquí se define la lógica principal del juego, incluyendo el manejo de jugadores, pelotas y la lógica de colisiones. El juego tiene dos modalidades:

- **One Ball**: Un solo balón en el juego.
- **Multi Ball**: Cada vez que una pelota toca una paleta, se genera una nueva pelota. El número de pelotas en el juego crece a medida que avanzas.

### Clases y Estructuras

1. **Player**: Estructura que maneja la posición, tamaño y vidas de los jugadores.
   ```cpp
   typedef struct Player {
       Vector2 position;
       Vector2 size;
       int life;
   } Player;

2. **Codigo Usado**: Eh usado codigo de el repositorio de raysan5
   ```
   https://github.com/raysan5/raylib-games/blob/master/classics/src/arkanoid.c
