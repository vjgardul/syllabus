###############################################################################
#                              Makefile Simple                                #
#                                                                             #
#                                                                             #
# Por Vicente Errázuriz                                                       #
# Para el curso de Estructuras de Datos y Algoritmos, 2016 - 1, PUC           #
###############################################################################

# El compilador a usar: Gnu C Compiler standard 2011 with GNU extensions
CC=gcc -std=gnu11

# Parametros para el compilador
# -Wunused = Que avise de las variables que no están siendo usadas
# -Wall    = Que avise de todos los errores
# -O3      = Optimizaciones nivel 3
# -I.      = Que luego de compilar conecte los archivos de objeto
CFLAGS=-Wunused -Wall -O3 -I.

# Parametros para poder compilar aplicaciones con interfaz gráfica GTK+
GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`

# Módulos de los cuales depende el programa.
DEPS=$(wildcard src/*.h)

# Archivos de objeto, un estado intermedio de compilación
OBJ=$(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))

# Nombre deseado para el archivo compilado final
PROGRAMS=helloanimation

# Al llamar make a secas se ejecutará esta regla:
# Limpiar compilación anterior, compilar, borrar los .o remanentes
# Las otras reglas se pueden llamar con make <regla>. Ej: make clean
default: mkobj $(PROGRAMS)
	@echo "Done compiling"

# Regla que especifíca como compilar los archivos de objeto
obj/%.o: src/%.c $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS) $(GTKFLAGS) && \
	echo "Compilado $@"

# Regla que conecta todas las partes del programa a partir de los objetos
$(PROGRAMS): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) $(GTKFLAGS) && \
	echo "Compilado $@"

# Regla que elimina todo registro de compilación que se haya hecho
clean: cleanobj cleanexe
	@echo "Done cleaning"

# Regla que elimina la versión anterior compilada
# "rm" para unix, "del" para windows. Eres libre de borrar el que no te sirva
cleanexe:
	@for i in $(PROGRAMS); do 		# Para cada uno de los programas          \
		if [ -e $$i ]; then 		# Si es que existe              		  \
			rm $$i || del $$i.exe; 	# Lo elimina 							  \
			echo "eliminado $$i"; 	# Y lo notifica 						  \
		fi; 																  \
	done

# Regla que elimina los archivos de objeto .o
# "rm" para unix, "del" para windows. Eres libre de borrar el que no te sirva
cleanobj:
	@for i in $(OBJ); do	  	  # Para cada uno de los archivos de objeto   \
		if [ -e $$i ]; then		  # Si es que existe 						  \
			rm $$i || del $$i; 	  # Lo elimina 								  \
			echo "eliminado $$i"; # Y lo notifica 							  \
		fi; 																  \
	done

# Regla encargada de crear los directorios para guardar los archivos de objeto
mkobj:
	@(mkdir obj 2>/dev/null || true)      # Crea primero la carpeta obj

###############################################################################
#                   Cualquier duda no temas en preguntar!                     #
###############################################################################
