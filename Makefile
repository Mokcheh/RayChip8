CC = g++

emu: src/cpu.cpp src/cpu.h src/main.cpp
	$(CC) -o build/emu src/cpu.cpp src/cpu.h src/main.cpp

clean:
	rm build/*

