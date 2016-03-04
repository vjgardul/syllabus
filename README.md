# IIC2133 - Estructuras de Datos y Algoritmos

Bienvenido al sitio web del curso de Estructuras de Datos y Algoritmos. En esta página podrás encontrar la información administrativa del curso. En el repositorio podrás encontrar código ya preparado por tus ayudantes, junto con los eventuales enunciados de las tareas y las diapositivas de clases.

Asegurate de echar una mirada a la [Wiki](https://github.com/IIC2133-2016-1/syllabus/wiki) del curso, donde estáran todas las guias de como prepararte para poder trabajar en este ramo.

## Tabla de contenidos
 * [Equipo](#equipo)
     * [Profesor](#profesor)
     * [Ayudantes](#ayudantes)
 * [Acerca del curso](#acerca-del-curso) 
 * [Contenidos](#contenidos)
 * [Evaluación](#evaluación)
     * [Evaluaciones escritas](#evaluaciones-escritas)
     * [Tareas](#tareas)
 * [Política de Integridad Académica](#política-de-integridad-académica)

## Acerca del curso

Este curso enseña a analizar problemas, descomponerlos y elaborar algoritmos que los resuelvan en un tiempo óptimo. Esto se logra mediante la organización de la información en estructuras de datos y el uso de algoritmos capaces de aprovechar dichas estructuras. En particular se enseñan problemas de optimización en grafos, métodos de ordenación, y estructuración de la información en diccionarios.

El curso utiliza el lenguaje de programación C, ya que éste permite trabajar directa y limpiamente con los algoritmos aprendidos. Dado que este lenguaje trabaja en bajo nivel, se puede lograr escribir un programa que resuelva los problemas en tiempo óptimo no solo en la teoría, si no también en la práctica.
     
## Equipo

### Profesor

| Nombre               |  Email         |
|:-------------------- |:--------------|
| Yadran Eterovic Solano | yadran@ing.puc.cl |


### Ayudantes

| Nombre                | Github       | Email         |
|:--------------------- |:-------------|:--------------|
| Vicente Errázuriz Quiroga | [@Ghoeb](https://www.github.com/Ghoeb) | verrazuriz@uc.cl |
| Félix Fischer Marqués     | [@felix91gr](https://www.github.com/felix91gr) | ffischer@uc.cl |
| Belén Saldías Fuentes     | [@bcsaldias](https://www.github.com/bcsaldias) | bcsaldias@uc.cl |






## Contenidos

A continuación se presentan los contenidos del curso, no necesariamente en el orden en que serán impartidos.

* Estructuras elementales
 * Colas LIFO y FIFO.
 * Heaps binarios.
* Diccionarios
 * Tablas de hash
 * Árboles de búsqueda
* Algoritmos
 * Ordenación
      * Ingenua ( *O*(n<sup>2</sup>) )
      * Inteligente ( *O*(n log(n)) u *O*(n) )
 * Optimización en grafos
      * Orden Topológico
      * Cobertura
      * Búsqueda
      * Flujo
 * Técnicas Algorítmicas
      * Dividir y conquistar
      * Algoritmos Codiciosos
      * Programación Dinámica
      * Backtracking
* Strings

## Evaluación

El curso consta de una parte teórica, evaluada mediante evaluaciones escritas (también conocidas como interrogaciones), y una parte práctica, evaluada mediante tareas de programacion en C.

### 

Habrá 2 interrogaciones y un examen, donde se evaluarán los aspectos más teóricos del contenido.

| Evaluación | Fecha | 
|:----------|:----------|
| I1 | 14 de Abril a las 18:30 |
| I2 | 9 de Mayo a las 17:00 |
| Examen | 28 de Junio a las 9:00 |

### Tareas

Habrá 4 tareas de programación en C, donde deberán resolver un problema complejo y analizarlo en un informe escrito. Las fechas serán anunciadas durante el semestre.

---

La nota final del curso se calcula de la siguiente manera:

```c

double nota_final()
{
    /* La nota de cada tarea */
    double T1,T2,T3,T4;
    /* La nota de cada interrogación */
    double I1,I2,Ex;

    /* Promedio de tareas */
    double NT = (T1 + T2 + T3 + T4) / 4;
    /* Promedio de las interrogaciones */
    double NE = (I1 + I2 + 2 * Ex) / 4;
    
    /* Es necesario aprobar las pruebas por separado para aprobar el curso */
    if(NE < 4)
    {
       return NE;
    }
    else
    {
       return (NE + NT) / 2;
    }
}

```

## Política de integridad académica

Este curso se adscribe a la política de integridad académica de la Escuela de Ingeniería y el Departamento de Computación.

---

Los alumnos de la Escuela de Ingeniería de la Pontificia Universidad Católica de Chile deben mantener un comportamiento acorde a la Declaración de Principios de la Universidad.  En particular, se espera que **mantengan altos estándares de honestidad académica**.  Cualquier acto deshonesto o fraude académico está prohibido; los alumnos que incurran en este tipo de acciones se exponen a un Procedimiento Sumario. Es responsabilidad de cada alumno conocer y respetar el documento sobre Integridad Académica publicado por la Dirección de Docencia de la Escuela de Ingeniería (disponible en SIDING).

Específicamente, para los cursos del Departamento de Ciencia de la Computación, rige obligatoriamente la siguiente política de integridad académica. Todo trabajo presentado por un alumno para los efectos de la evaluación de un curso debe ser hecho individualmente por el alumno, sin apoyo en material de terceros.  Por “trabajo” se entiende en general las interrogaciones escritas, las tareas de programación u otras, los trabajos de laboratorio, los proyectos, el examen, entre otros.

**En particular, si un alumno copia un trabajo, o si a un alumno se le prueba que compró o intentó comprar un trabajo, obtendrá nota final 1.1 en el curso y se solicitará a la Dirección de Docencia de la Escuela de Ingeniería que no le permita retirar el curso de la carga académica semestral.**

Por “copia” se entiende incluir en el trabajo presentado como propio, partes hechas por otra persona.  **En caso que corresponda a “copia” a otros alumnos, la sanción anterior se aplicará a todos los involucrados**.  En todos los casos, se informará a la Dirección de Docencia de la Escuela de Ingeniería para que tome sanciones adicionales si lo estima conveniente. Obviamente, está permitido usar material disponible públicamente, por ejemplo, libros o contenidos tomados de Internet, siempre y cuando se incluya la referencia correspondiente y sea autorizado por los ayudantes.

Lo anterior se entiende como complemento al Reglamento del Alumno de la Pontificia Universidad Católica de 
Chile<sup><a name="pucCLBack">[1](#pucCL)</a></sup>.  Por ello, es posible pedir a la Universidad la aplicación de sanciones adicionales especificadas en dicho reglamento.

<sub>**<a name="pucCL">[1](#pucCL)</a>**: Reglamento del Alumno de la Pontificia Universidad Católica de Chile disponible en: http://admisionyregistros.uc.cl/alumnos/informacion-academica/reglamentos-estudiantiles [&#8593;](#pucCLBack)</sub>
