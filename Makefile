CC = g++
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
OUTPUT = -o bin/emu

emu: build/cpu.o build/main.o
	$(CC) build/main.o build/cpu.o $(OUTPUT) $(LIBS)

build/main.o: src/main.cpp
	$(CC) -c -o build/main.o src/main.cpp 

build/cpu.o: src/cpu.cpp
	$(CC) -c -o build/cpu.o src/cpu.cpp


