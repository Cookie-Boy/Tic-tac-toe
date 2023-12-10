all: compile link

compile:
	g++ -Isfml_lib/include -Isrc -c src/interface.cpp -o obj/interface.o
	g++ -Isfml_lib/include -Isrc -c src/main.cpp -o obj/main.o

link:
	g++ obj/interface.o obj/main.o -o bin/main.exe -Lsfml_lib/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	del /Q /F obj/interface.o
	del /Q /F obj/main.o
	del /Q /F bin/main.exe

run:
	bin/main.exe