all: compile link

compile:
	g++ -Isfml_lib/include -c src/main.cpp -o obj/main.o

link:
	g++ obj/main.o -o bin/main.exe -Lsfml_lib/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm obj/*.*
	rm bin/main.exe

run:
	bin/main.exe