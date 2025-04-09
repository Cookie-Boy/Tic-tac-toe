all: compile link

compile:
	g++ -Isrc -c src/interface.cpp -o obj/interface.o
	g++ -Isrc -c src/main.cpp -o obj/main.o

link:
	g++ obj/interface.o obj/main.o -o bin/main.exe -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/interface.o
	rm obj/main.o
	rm bin/main.exe

run:
	bin/main.exe