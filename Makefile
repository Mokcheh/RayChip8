CC = g++
LIBS = -lraylib
OUTPUT = -o bin/emu

emu: build/cpu.o build/main.o build/window.o
	$(CC) build/main.o build/cpu.o build/window.o $(OUTPUT) $(LIBS)

build/main.o: src/main.cpp
	$(CC) -c -o build/main.o src/main.cpp 

build/cpu.o: src/cpu.cpp
	$(CC) -c -o build/cpu.o src/cpu.cpp


build/window.o: src/window.o
	$(CC) -c -o build/window.o src/window.cpp

clean:
	rm build/*


clean-bin:
	rm bin/emu
