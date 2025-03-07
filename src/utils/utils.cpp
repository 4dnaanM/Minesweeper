#ifndef UTILS
#define UTILS

#include "../lib/lib.h"

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

#include <iostream>
#include <stdexcept>

enum class CellState : int8_t {
    MINE = -1,
    HIDDEN = -2,
    CANTFLAG = -3, 
    FLAGGED = -1,  
    EMPTY = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8
};

struct Cell {
    CellState state;

    Cell(CellState s = CellState::EMPTY) : state(s) {}

    Cell& operator=(int value) {
        state = toCellState(value);
        return *this;
    }

    bool operator==(const CellState& s) const {
        return this->state == s;
    }

    bool operator!=(const CellState& s) const {
        return this->state != s;
    }

    operator int() const {
        return static_cast<int>(state);
    }

    static CellState toCellState(int value) {
        switch (value) {
            case -3: return CellState::CANTFLAG;
            case -2: return CellState::HIDDEN;
            case -1: return CellState::MINE;  
            case  0: return CellState::EMPTY;
            case  1: return CellState::ONE;
            case  2: return CellState::TWO;
            case  3: return CellState::THREE;
            case  4: return CellState::FOUR;
            case  5: return CellState::FIVE;
            case  6: return CellState::SIX;
            case  7: return CellState::SEVEN;
            case  8: return CellState::EIGHT;
            default: throw std::invalid_argument("Invalid integer for CellState");
        }
    }
};


enum ClickType{
    LEFT_CLICK,
    RIGHT_CLICK
};

typedef std::vector<std::vector<Cell>> Board;

typedef std::tuple<int,int,ClickType> Move;

#endif