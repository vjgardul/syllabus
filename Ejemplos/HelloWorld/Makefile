all: helloworld
	@echo "Done compiling"

helloworld:
	@gcc -std=gnu11 src/main.c -o helloworld && \
	echo "Compilado helloworld"

clean:
	@(rm -f helloworld || del helloworld) && \
	echo "Eliminado helloworld"
