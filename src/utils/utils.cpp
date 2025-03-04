#include <vector>

#ifndef UTILS
#define UTILS

#include <vector>
#include <SFML/Graphics.hpp>

struct GameParams{
    int L = 30; 
    int W = 16; 
    int totalMines = 99;
    bool gameOver = false; 
    int remainingMinesCount = totalMines;

    GameParams(int L, int W, int totalMines):L(L),W(W),totalMines(totalMines){}
    GameParams(){}
};

struct GUIParams{
    int CELL_SIZE=40;
    int L=30; 
    int W=16; 
    
    GUIParams(int L,int W,int CELL_SIZE):CELL_SIZE(CELL_SIZE),L(L),W(W){}
    GUIParams():CELL_SIZE(40),L(30),W(16){}
};

enum GameCellState{
    GAME_MINE,
    GAME_EMPTY,
    GAME_ONE,
    GAME_TWO,
    GAME_THREE,
    GAME_FOUR,
    GAME_FIVE,
    GAME_SIX,
    GAME_SEVEN,
    GAME_EIGHT
};

enum UserCellState {
    USER_HIDDEN,
    USER_FLAGGED,
    USER_REVEALED,
    USER_CANTFLAG, // to be used only by engines, GUI treats it as HIDDEN
    USER_MINE,
    USER_EMPTY,
    USER_ONE,
    USER_TWO,
    USER_THREE,
    USER_FOUR,
    USER_FIVE,
    USER_SIX,
    USER_SEVEN,
    USER_EIGHT
};

enum ClickType{
    LEFT_CLICK,
    RIGHT_CLICK
};

typedef std::vector<std::vector<GameCellState>> GameBoard;
typedef std::vector<std::vector<UserCellState>> UserBoard;

typedef std::tuple<int,int,ClickType> Move;

#endif