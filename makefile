CC = g++
CFLAGS = -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib
LINKS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = src/utils/utils.cpp src/core/game.cpp src/board/board.cpp src/gui/gui.cpp src/engine/engine.cpp
BIN = bin

all: build run

build:
# g++ -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system src/core/game.cpp src/gui/gui.cpp -o bin/minesweeper
	$(CC) $(CFLAGS) $(LINKS) $(SRC) -o $(BIN)/minesweeper

run: 
	./$(BIN)/minesweeper

debug:
	clang++ $(CFLAGS) $(LINKS) $(SRC) -o $(BIN)/minesweeper -g
	lldb ./$(BIN)/minesweeper

