CC = g++
CFLAGS = -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib
LINKS = -lsfml-graphics -lsfml-window -lsfml-system

build:
# g++ -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system main.cpp -o minesweeper
	$(CC) $(CFLAGS) $(LINKS) sfml.cpp -o sfml

exec: 
	./sfml

