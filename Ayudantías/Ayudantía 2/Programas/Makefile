###############################################################################
#                             Makefile Múltiple                               #
#                                                                             #
# Por Vicente Errázuriz                                                       #
# Para el curso de Estructuras de Datos y Algoritmos, 2016 - 1, PUC           #
# Makefile diseñada para el trabajo de varios programas con código común      #
###############################################################################

# Indica que se debe parsear el archivo dos veces
.SECONDEXPANSION:

# El compilador a usar: Gnu C Compiler, Standard 2011 with GNU extensions
CC=gcc -std=gnu11

# Parametros para el compilador
# -Wunused = (Warn Unused) Da aviso de las variables que no se estan usando
# -Wall    = (Warn All) Da aviso de todos los posibles errores de compilación
# -O3      = Optimizaciones nivel 3
# -I.      = (Include .) Que busque los headers en el directorio actual
CFLAGS=-Wunused -Wall -O0 -I.

# Parametros para poder compilar aplicaciones con interfaz gráfica GTK+
GTKFLAGS=`pkg-config --cflags --libs gtk+-3.0`

# Librerias que deben ser linkeadas al proyecto para poder compilar
# m = C Math library
LIB=-lm

# Los programas de los que esta makefile se hace cargo
PROGRAMS=ndoku watcher pruneNdoku heuristicNdoku fastNdoku

# Todos los .h del proyecto
DEPS=$(foreach i, common $(PROGRAMS), $(wildcard src/$(i)/*.h))

# Regla de substitucion para encontrar los archivos .c
SUBST=$(patsubst src/%.c, obj/%.o, $(wildcard src/$(i)/*.c))

# Archivos de objeto, un estado intermedio de compilación
# Por cada .c dentro de src/X, se crea un .o dentro de obj/X
OBJ=$(foreach i, common $(PROGRAMS), $(SUBST))

# Los directorios para los archivos de objeto
OBJDIR=obj $(foreach i, common $(PROGRAMS), obj/$(i))

# Al llamar make a secas se ejecutará esta regla, ya que es la primera
# Llamar a las reglas respectivas para cada uno de los elementos en OBJDIR
# para que creen las carpetas para guardar los archivos de objeto
# Llamar a las reglas respectivas con cada uno de los elementos en PROGRAMS
# para que compilen cada uno de los programas
# Las otras reglas se pueden llamar con make <regla>. Ej: make clean
all: $(OBJDIR) $(PROGRAMS)
	@echo "done compiling"

# Regla que especifíca como compilar los archivos de objeto
# %.o se compila a partir de %.c guardado en src/ (% incluye la carpeta)
# y todos los .h presentes
# TODO Idealmente esto solo debería usar los .h en la carpeta especificada
obj/%.o: src/%.c $(DEPS)
	@$(CC) $(CFLAGS) $< -c -o $@ $(GTKFLAGS) $(LIB) 						  \
	&& echo "compilado $@"

# Regla que conecta todas las partes del programa a partir de los objetos
# Conecta tambien los demás archivos que hayan en la carpeta del programa
# Esta regla se llamará con cualquiera de los elementos que está en PROGRAMS
# Esta regla necesita SECONDEXPANSION para poder armar los prerrequisitos
$(PROGRAMS): $$(filter obj/$$@/% obj/common/%, $(OBJ))
	@$(CC) $(CFLAGS) $^ -o $@ $(GTKFLAGS) $(LIB) 		  \
	&& echo "compilado $@"

# Regla que elimina todo registro de compilación que se haya hecho
clean: cleanobj cleanexe
	@echo "done cleaning"

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
$(OBJDIR):
	@mkdir $@
	
###############################################################################
#                   Cualquier duda no temas en preguntar!                     #
###############################################################################
