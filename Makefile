INPUT = entrada.txt

build: main.cpp veb.cpp hashtable.cpp
	g++ -std=c++17 -O2 -o programa.exe main.cpp veb.cpp hashtable.cpp

run: programa.exe
	./programa.exe $(INPUT)
