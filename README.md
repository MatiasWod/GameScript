# GameScript - PyGame Engine

Un compilador construido con Flex y Bison, en el marco de la materia Autómatas, Teoría de Lenguajes y Compiladores del Instituto Tecnológico de Buenos Aires.

| Alumno            | Legajo | Mail                  |
|-------------------|--------|-----------------------|
| Baumgart Bruno    | 62161  | bbaumgart@itba.edu.ar |
| Canevaro Bautista | 62179  | bcanevaro@itba.edu.ar |   
| Hirsch Santiago   | 62169  | shirsch@itba.edu.ar   |
| Wodtke Matías     | 62098  | mwodtke@itba.edu.ar   |
|-------------------|--------|-----------------------|

<hr>

* [Introducción](#introducción)
* [Utilización del compilador](#utilización-del-compilador)
    * [Requerimientos](#requerimientos)
    * [Construcción](#construcción)
    * [Ejecución](#ejecución)
    * [Testing](#testing)
* [Desarrollo del proyecto](#desarrollo-del-proyecto)
* [Ejemplos de código](#ejemplos-de-código)
* [Limitaciones y dificultades encontradas](#limitaciones-y-dificultades-encontradas)
* [Futuras extensiones y modificaciones](#futuras-extensiones-y-modificaciones)
* [Conclusiones](#conclusiones)

<hr>

## Introducción

GameScript es un lenguaje que permite a cualquier persona con un conocimiento básico de programación, poder programar videojuegos usando una sintaxis sencilla y amigable para el usuario. Para realizar esto con poca complejidad, este lenguaje hace uso de la librería de Python, Pygame. Pygame es un conjunto de módulos del lenguaje Python que permiten la creación de videojuegos en dos dimensiones de una manera sencilla. De esta manera, logramos tener un lenguaje eficiente, simple y potente para la generación de videojuegos, en la que el usuario puede demostrar toda su creatividad en el diseño y jugabilidad.



La idea de este lenguaje nace con la idea de centralizar y versionar el proceso de generación de sprites para videojuegos, permitiendo aplicar las mismas técnicas de desarrollo que a una aplicación; a saber, revisión, testing y análisis estático.

## Utilización del compilador

### Requerimientos

Para construir el compilador, se requiere tener instalado [Docker](https://docs.docker.com/engine/install/), junto con la imagen de compilación.

Se debe crear la imagen necesaria utilizando el archivo `Dockerfile`:

```
$ docker build -t compiler .
```

### Construcción

Para construir el proyecto por completo, ejecute en la raíz del repositorio los siguientes comandos:

```
$ chmod u+x --recursive script
$ docker run -v "${PWD}:/root" -ti --rm alejofl/tla-compiler
$ ./script/build.sh
```

### Ejecución

Para compilar, primero cree un archivo con el contenido del programa. A modo de ejemplo, el path al archivo (relativo a la raíz del proyecto) será `program`.

Luego, dentro del contenedor de Docker, ejecute el compilador desde el directorio raíz del proyecto, pasando por parámetro el path hacia el programa a compilar:

```
$ ./script/start.sh program
```


### Testing

Dentro del contenedor de Docker,

```
$ ./script/test.sh
```

Si desea agregar nuevos casos de uso, deberá crear un archivo por cada uno, que contenga el programa a testear dentro de las carpetas `test/accept` o `test/reject` según corresponda (es decir, si el mismo debe ser aceptado o rechazado por el compilador).

## Desarrollo del proyecto

El desarrollo del compilador tuvo tres iteraciones bastante marcadas. En primer lugar, se plantearon las funcionalidades que el lenguaje iba a proveer. Esto resultó un desafío debido a la necesidad de condensar lo máximo posible los requerimientos externos. Sin embargo, se acordó usar un buen número de funciones que permiten que nuestro lenguaje sea sencillo y, por sobretodo, entendible de usar.

En segundo lugar, se debió pensar la sintaxis del lenguaje, para luego convertirla en una gramática propiamente dicha, que pueda ser programada. En pos de lograr esto, nos basamos principalmente en Java y Python para usar una sintaxis familiar y amable para el usuario. Luego de eso, se programó la gramática de GameScript, utilizando Lex (Flex) y Yacc (Bison).

Por último, se debió implementar el análisis semántico del lenguaje, donde se agregaron la mayoría de las limitaciones del mismo. Inicialmente, se concentraron los esfuerzos en crear el árbol de sintaxis abstracta del programa. Luego, se programó la parte de generación de código. Dentro de este proceso, gran parte del esfuerzo se concentró en encontrar las maneras de lograr las funcionalidades que habíamos propuesto en Python. Finalmente, se crearon las estructuras necesarias para soportar una tabla de símbolos, así permitiendo detectar errores de tipos de datos y de variables no definidas.

## Ejemplos de código

1. El siguiente fragmento de código crea el famoso juego Pong pero con la modificacion de poder jugarlo 2vs2:

    ```
    // Creamos a los 4 jugadores:
    gobject Player()
    {
        block hitbox[1][8]; // 3 posiciones de ancho, 1 posición de altura
        return this;
    }
    
    // p2,p3,p4 serian iguales pero modificando las teclas de movimiento
    
    gobject Ball()
    {
        block hitbox[1][1]; // 1 posición de ancho, 1 posición de altura
        return this;
    }
    // Creamos escena (en este caso única y suponiendo que es 10x10 en tamaño)
    // UP,DOWN y NOKEY serían macros proporcionadas por el lenguaje
    
    mainScene()
    {
        int team1Score = 0;
        int team2Score = 0;
        gobject p1 = Player();
        
        p1.setMovement(UP, DOWN, NOKEY, NOKEY);
        p1.setPosition(0, 0);
        p1.setSpeed(6);
        gobject p2 = Player();
        p2.setSpeed(6);
        p2.setMovement("w", "s", NOKEY, NOKEY);
        p2.setPosition(0, 440);
        gobject p3 = Player();
        p3.setSpeed(6);
        p3.setMovement("t", "g", NOKEY, NOKEY);
        p3.setPosition(780, 0);
        gobject p4 = Player();
        p4.setSpeed(6);
        p4.setMovement("i", "k", NOKEY, NOKEY);
        p4.setPosition(780, 440);
        gobject ball = Ball();
        ball.setSpeed(5);
        ball.randomMovement(1); // Movimiento aleatorio
        ball.setPosition(300, 400);
        when(ball in p1)
        {
            ball.bounceX();
        }
        when(ball in p2)
        {
            ball.bounceX();
        }
        when(ball in p3)
        {
            ball.bounceX();
        }
        when(ball in p4)
        {
            ball.bounceX();
        }
    
        when(ball in RIGHT_BORDER)
        {
            team1Score++;
            ball.setPosition(400, 300);
        }
        when(ball in LEFT_BORDER)
        {
            team2Score++;
            ball.setPosition(300, 400);
        }
        when(ball in UP_BORDER or ball in DOWN_BORDER)
        {
            ball.bounceY();
        }
        
        if(team1Score == 10)
        {
            finish("Team 1 wins!");
        }
	}
    ```

2. El siguiente fragmento de código toma ciertas imágenes y las modifica para que sirvan para fondos de pantalla para el celular:

    ```
    filter blur = filter("BLUR");
    filter other_filter = filter()
                              .parameter(shadows=0.6)
                              .parameter(brightness=0.5);
    
    for wallpaper in set(image("playa.jpg");, image("mar.jpg"), image("cielo.png")) {
        wallpaper.apply(blur, other_filter);
        wallpaper.trim(width=1080.0, height=1920.0, position=center-center);
        wallpaper.save();
    }
    ```


## Limitaciones y dificultades encontradas

Nos encontramos con un gran número de dificultades a lo largo del desarrollo del proyecto. En primer lugar, cubrir todos los casos que se habían propuesto, pese a los grandes esfuerzos del grupo por desarrollar y entender los conceptos de la mejor manera, en un principio no resultó posible debido a la poca disponibilidad de tiempo con la que contamos para esta tarea.

En segundo lugar, al poder crear funciones con nuestro lenguaje abrio un gran abanico de posibilidades al usuario que resultaron en complicaciones para nosotros quien desarrollamos el lenguaje, por lo que decidimos acotar las mismas a unicamente funciones Gobject, las cuales tienen un comportamiento en especifico, y luego algunos metodos extra para que el usuario pueda lograr distintas combinaciones en su juego.

Ademas lograr que dos objetos colisionen a traves de la clausula "when" fue mas dificil de lo que imaginamos, llevandonos mas tiempo del que hubiesemos imaginado, pero finalmente con un resultado que nos dejo muy conformes.

El optimismo del grupo a la hora de desarrollar las reglas del lenguaje en primera instancia nos llevo a cometer algunos errores de diseño que si tuviesemos que empezar nuevamente en este momento lograriamos evadir para lograr un mejor rendimiento.

El principal error fue plantear al lenguaje desde 0, como si tuviesemos que programar el juego en un inicio, cuando realmente deberiamos haber pensado de forma mas generica, aprovechando que podemos insertar codigo de python a nuestro gusto para los casos que aceptaramos, y lograr asi resultados de mejor calidad, como por ejemplo algo del estilo "createEnemy();" y que automaticamente esto genere cierto enemigo con un comportamiento especifico en la pantalla o poder setear de antemano niveles e ir navegando en ellos a medida que llegues a la parte derecha de la pantalla

## Futuras extensiones y modificaciones

A continuación se listan una serie de modificaciones que harían de GameScript un lenguaje más potente:

- Agregar la funcionalidad de "Scenes" para permitir al usuario navegar en diferentes escenas y lograr incluso una especie de juego arcade con "niveles"
- Agregar metodos que permitan configurar inteligencia artificial a "bots" y distintos comportamientos
- Agregar botones y texto personalizado a las pantallas
- Pulir reglas de gramatica para un mejor funcionamiento
- Finalizar las reglas que no pudieron hacerse hasta este momento
## Conclusiones

En conclusion este trabajo practico no resulto exactamente como esperabamos debido a la falta de experiencia en este tipo de ambitos, asi y todo disfrutamos mucho de la experiencia y todos los participantes del grupo coincidimos en que la idea es realmente interesante y divertida, pero que elegimos mal el enfoque de la gramatica desde un inicio. Consideramos realmente volver a rehacer este lenguaje mas alla de la materia con un poco mas de experiencia y mas tiempo libre, y asi lograr el resultado que tanto deseabamos.

A pesar de todo, consideramos que el resultado obtenido esta a la altura de lo que queriamos generar, pudiendo crear nuestro tan ansiedo "pong 2vs2" y algunos otros juegos que se encuentran en el directorio "test"