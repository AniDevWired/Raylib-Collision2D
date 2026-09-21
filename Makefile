main: src/main.c
	gcc -o bin/main.o src/main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run:
	./bin/main.o