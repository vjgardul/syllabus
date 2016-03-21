# Ayudantía 2: n-doku

En la carpeta programas podrás encontrar los programas vistos en la ayudantía del lunes 21 de marzo. Estos muestran distintas formas de mejorar el algoritmo de backtracking usando como ejemplo el conocido puzzle _Sudoku_, pero generalizandolo a grillas de n² x n², con n > 0. Llamaremos a estos puzzles **n-doku**.

Para ello se implementaron 4 programas distintos que intentan llenar aleatoriamente una grilla vacia de n-doku:

* `ndoku` : resuelve el problema usando backtracking recursivo y nada más.
* `pruneNdoku`: resuelve el problema usando backtracking iterativo y revisando si llega a un estado inresolvible.
* `heuristicNdoku` : lo mismo que el anterior, pero además priorizando la asignación de celdas segun una heurística.
* `fastNdoku` : lo mismo que el anterior, pero utilizando heaps para manejar la priorizacion de las celdas.

Los 4 programas reciben 2 parámetros:

* `n` : el orden n del n-doku.
* `f` : indica el nivel de seguimiento del algoritmo
  * `fast` : si quieres que el algoritmo resuelva el problema y luego imprima la solución
  * `step` : si quieres que el algoritmo imprima cada decisión que toma para llegar a la solución

Por ejemplo:

```sh
./heuristicNdoku 3 step
```

Ejecuta el solver con heurísticas para resolver un sudoku tradicional de 9x9 e imprime cada paso intermedio.

```sh
./ndoku 5 fast
```

Ejecuta el solver básico con un n-doku de 25x25 y espera a encontrar la solucion para imprimirla.

### Watcher

Los 4 programas pueden ser visualizados usando el **watcher** provisto, dándole como input el output del solver que escojas.

Una forma de hacerlo es con pipes:

```sh
./pruneNdoku 2 fast | ./watcher
```

Puedes regular el delay entre las actualizaciones de la ventana pasando un parámetro adicional al **watcher**, de la siguiente manera:

```sh
./fastNdoku 4 step | ./watcher t
```
Eso hará que el watcher actualize cada _t_ milisegundos. O por lo menos lo intentará.

Cualquier problema no dudes en preguntar!

