#ifndef BOARD
#define BOARD

#include "../utils/utils.cpp"

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <ctime>
#include <cassert>

class MinesweeperBoard{


    std::vector<int> traverse_x = {-1,0,1,-1,1,-1,0,1};
    std::vector<int> traverse_y = {-1,-1,-1,0,0,1,1,1};

    int generateMines(){
        srand(time(0));
        int placedMines = 0; 
        
        while(placedMines<totalMines){

            int random_position = rand()%(W*L);
            int random_y = random_position/L;
            int random_x = random_position%L;

            if(gameBoard[random_y][random_x]!=GAME_MINE){
                gameBoard[random_y][random_x]=GAME_MINE;
                placedMines++;
            }
        }

        for(int x = 0; x < L; x++){
            for(int y = 0; y < W; y++){
                if(gameBoard[y][x]!=GAME_MINE)gameBoard[y][x] = convertToGameCellState(countNeighbouringMines(y,x));
            }
        }
        return 0; 
    }
    
    bool inBoard(int y, int x){
        return (x>=0)&&(x<L)&&(y>=0)&&(y<W);
    }

    int countNeighbouringFlags(int y, int x){
        
        int neighbouring = 0; 

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+traverse_x[neighbor];
            int n_y = y+traverse_y[neighbor];
            if(inBoard(n_y,n_x)){
                neighbouring+=(userBoard[n_y][n_x]==USER_FLAGGED);
            }
        }
        return neighbouring;
    }
    
    int countNeighbouringMines(int y, int x){
        
        int neighbouring = 0; 

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+traverse_x[neighbor];
            int n_y = y+traverse_y[neighbor];
            if(inBoard(n_y,n_x)){
                neighbouring += (gameBoard[n_y][n_x] == GAME_MINE);
            }
        }

        return neighbouring;
    }

    int chord(int y, int x, bool& gameOver){

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+traverse_x[neighbor];
            int n_y = y+traverse_x[neighbor];
            if(inBoard(n_y,n_x)&&userBoard[n_y][n_x]==USER_HIDDEN){
                // if it is hidden, click on it.
                if(gameBoard[n_y][n_x]==GAME_MINE){
                    gameOver = true;
                }
                else{
                    updateUserBoard(n_y,n_x);
                }
            }
        }
        return 0;
    }

    int updateUserBoard(int y,int x){
        if(!inBoard(y,x))return 0;
        if(userBoard[y][x]!=USER_HIDDEN)return 0;
        // not intended to be called on a mine
        assert(gameBoard[y][x] != GAME_MINE);
        
        userBoard[y][x] = convertToUserCellState(gameBoard[y][x]);

        int neighbouringMines = countNeighbouringMines(y,x); 

        if(neighbouringMines==0){
            for (int neighbor = 0; neighbor<8; neighbor++){
                int n_x = x+traverse_x[neighbor];
                int n_y = y+traverse_y[neighbor];
                updateUserBoard(n_y,n_x);
            }
        }
        return 0;
    }

    GameCellState convertToGameCellState(int k){
        switch(k) {
            case 0: return GAME_EMPTY;
            case 1: return GAME_ONE;
            case 2: return GAME_TWO;
            case 3: return GAME_THREE;
            case 4: return GAME_FOUR;
            case 5: return GAME_FIVE;
            case 6: return GAME_SIX;
            case 7: return GAME_SEVEN;
            case 8: return GAME_EIGHT;
            default: return GAME_EMPTY; // This should never be reached
        }
    }
    
    GameCellState convertToUserCellState(UserCellState k){
        switch(k) {
            case USER_MINE: return GAME_MINE;
            case USER_EMPTY: return GAME_EMPTY;
            case USER_ONE: return GAME_ONE;
            case USER_TWO: return GAME_TWO;
            case USER_THREE: return GAME_THREE;
            case USER_FOUR: return GAME_FOUR;
            case USER_FIVE: return GAME_FIVE;
            case USER_SIX: return GAME_SIX;
            case USER_SEVEN: return GAME_SEVEN;
            case USER_EIGHT: return GAME_EIGHT;
            default: return GAME_EMPTY; // This should never be reached
        }
    }
    
    UserCellState convertToUserCellState(int k){
        switch(k) {
            case -3: return USER_CANTFLAG;
            case -2: return USER_HIDDEN; 
            case -1: return USER_FLAGGED; 
            case 0: return USER_EMPTY;
            case 1: return USER_ONE;
            case 2: return USER_TWO;
            case 3: return USER_THREE;
            case 4: return USER_FOUR;
            case 5: return USER_FIVE;
            case 6: return USER_SIX;
            case 7: return USER_SEVEN;
            case 8: return USER_EIGHT;
            default: return USER_EMPTY; // This should never be reached
        }
    }

public:
    int L;
    int W;
    int totalMines;
    GameBoard gameBoard;
    UserBoard userBoard; 
    
    MinesweeperBoard(GameParams p){
        L = p.L;
        W = p.W;
        totalMines = p.totalMines;
        gameBoard = GameBoard(W, std::vector<GameCellState>(L,GAME_EMPTY));
        userBoard = UserBoard(W, std::vector<UserCellState>(L,USER_HIDDEN));
        generateMines();
    }

    MinesweeperBoard(){
        GameParams p = GameParams();
        *this = MinesweeperBoard(p);
    }
    
    bool gameIsCorrect(){
        for (int x = 0; x < L; x++){
            for (int y = 0; y < W; y++){

                if(gameBoard[y][x]==GAME_MINE && userBoard[y][x]!=USER_FLAGGED){
                    return false;
                }
                
                if(userBoard[y][x]==USER_FLAGGED && gameBoard[y][x]!=GAME_MINE){
                    return false;
                }

            }
        }
        return true;
    }
    
    int playInput(Move M, GameParams& params){
        int y = std::get<0>(M);
        int x = std::get<1>(M);
        ClickType C = std::get<2>(M);

        if(!inBoard(y,x)){
            // std::cout<<"Out of board!\n";
            return 0;
        }
        if(C == RIGHT_CLICK){
            if(userBoard[y][x]==USER_HIDDEN){
                params.remainingMinesCount--;
                userBoard[y][x] = USER_FLAGGED;
            }
            else if(userBoard[y][x]==USER_FLAGGED){
                params.remainingMinesCount++;
                userBoard[y][x] = USER_HIDDEN;
            }
            else{
                int nFlags = countNeighbouringFlags(y,x);
                if(nFlags==gameBoard[y][x])chord(y,x,params.gameOver);
            }
        }
        else {
            if(gameBoard[y][x]==GAME_MINE){
                params.gameOver = true;
            }
            else if(userBoard[y][x]==USER_HIDDEN){
                updateUserBoard(y,x);
                
            }
            // revealed square left click: do nothing
        }
        return 0;
    }
        
};

#endif