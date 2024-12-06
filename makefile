CC = g++
CFLAGS = -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib
LINKS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/core/game.cpp
BIN = bin

build:
# cd "/Users/Adnaan/Repos/Minesweeper/src/core/" && g++ -std=c++14 game.cpp -o game && "/Users/Adnaan/Repos/Minesweeper/src/core/"game
	$(CC) $(CFLAGS) $(LINKS) $(SRC) -o $(BIN)/minesweeper

run: 
	./$(BIN)/minesweeper

