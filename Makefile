all: compile link

compile:
	g++ -Isfml_lib/include -Isrc -c src/environment.cpp -o obj/environment.o
	g++ -Isfml_lib/include -Isrc -c src/main.cpp -o obj/main.o

link:
	g++ obj/environment.o obj/main.o -o bin/main.exe -Lsfml_lib/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/*.*
	rm bin/main.exe

run:
	bin/main.exe