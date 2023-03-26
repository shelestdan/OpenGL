CC= g++
LIB= -Iinclude -Llibs -lglfw3 -lgdi32

run: all open

all: src/main.cpp src/glad.c
	$(CC) src/main.cpp src/glad.c $(LIB)

clean: 
	rm -rf *.exe

open:
	.\a.exe