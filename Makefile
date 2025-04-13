all: compile link

compile:
	g++ -g -Wall -Isrc -c src/interface.cpp -o obj/interface.o
	g++ -g -Wall -Isrc -c src/main.cpp -o obj/main.o

link:
	g++ -g -Wall obj/interface.o obj/main.o -o bin/main.exe -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/interface.o
	rm obj/main.o
	rm bin/main.exe

run:
	bin/main.exe