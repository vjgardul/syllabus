# Programas vistos que resuelven Cindy

Los 3 "cindy" representan 3 formas de resolver el problema de cindy con backtracking.
  - *cindy* resuelve el problema con backtracking simple.
  - *mediumCindy* resuelve el problema con backtracking inteligente.
    Elige antes que nada los saltos largos, si es que puede hacerlos. Ésta es una de las ideas de "poda" que hablamos en la ayudantía. Verán que es siempre un 80% más rápida que "cindy".
  - *fastCindy* resuelve el problema podando siempre correctamente, por lo que NUNCA hace backtracking. Lo vimos en la ayudantía.

Para ejecutar los "cindy", por ejemplo *mediumCindy*, ejecuta:

```bat
 .\mediumCindy.exe
```

El problema correrá con 3 esferas por lado y usando recursión.

```bat
 .\mediumCindy.exe 5
```

El problema correrá con 5 esferas por lado y usando recursión.

```bat
 .\mediumCindy.exe 10 loop
```

El problema correrá con 10 esferas por lado y usando un loop con stack en lugar de recursión.
*fastCindy* no necesita hacer backtracking, por lo que el loop que tiene no usa stacks.

El *watcher* es un programa que permite visualizar el problema con esferas de color en lugar de la consola. Para usarlo es necesario entregarle el output de uno de los *cindy*. Para hacer esto, usamos *pipelines*:

  ```bat
   programaA | programaB
  ```

  Así el programaB recibirá como input de consola, el output del programaA.

En el caso del watcher:
  ```bat
    .\cindy.exe | .\watcher.exe
  ```
  Lo ejecuta con el input de *cindy* y con un delay de 100 milisegundos entre cada paso que muestra.
  ```bat
    .\cindy.exe | .\watcher.exe 550
  ```
  Lo ejecuta con el input de *cindy* y con un delay de 550 milisegundos entre cada paso que muestra.
