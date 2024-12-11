CC = g++
CFLAGS = -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib
LINKS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/core/game.cpp src/gui/gui.cpp
BIN = bin

build:
# g++ -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system src/core/game.cpp src/gui/gui.cpp -o bin/minesweeper
	$(CC) $(CFLAGS) $(LINKS) $(SRC) -o $(BIN)/minesweeper

run: 
	./$(BIN)/minesweeper

debug:
	clang++ $(CFLAGS) $(LINKS) $(SRC) -o $(BIN)/minesweeper -g
	lldb ./$(BIN)/minesweeper

