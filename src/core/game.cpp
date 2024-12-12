#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <ctime>

#include "../gui/gui.cpp"
#include "../engine/engine.cpp"
#include "../board/board.cpp"

class Minesweeper{
    
    bool gameOver;
    int remainingMinesCount; 

    MinesweeperBoard board;
    MinesweeperGUI GUI;
    HelperEngine engine; 

public:
    Minesweeper():
        board(),
        GUI(board.L, board.W, board.MINE, board.HIDDEN, board.FLAGGED),
        engine(board.L, board.W){
            gameOver = false;
            remainingMinesCount = board.totalMines;
        }

    int runGUI(){

        GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);

        while(GUI.windowIsOpen()){
            while(!gameOver && remainingMinesCount>0){
                int play_x; 
                int play_y;
                bool play_F;
                GUI.takeInteractiveInput(play_y, play_x, play_F);
                board.playInput(play_y,play_x,play_F, remainingMinesCount, gameOver);
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
            }
            if(remainingMinesCount==0 && board.gameIsCorrect()){
                // win
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                break; 
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        GUI.closeWindow();
        return 0;
    }

    int runEngine(){

        GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);

        while(GUI.windowIsOpen()){
            while(!gameOver && remainingMinesCount>0){
                int play_x; 
                int play_y;
                bool play_F;
                GUI.takeInteractiveInput(play_y, play_x, play_F);
                board.playInput(play_y,play_x,play_F, remainingMinesCount, gameOver);
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                std::vector<std::tuple<int,int,int>> moves = engine.returnMoves(board.userBoard);

                for(auto move: moves){
                    board.playInput(std::get<0>(move),std::get<1>(move),std::get<2>(move)==1,remainingMinesCount,gameOver);
                    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                    if(gameOver)break;
                }
            }
            if(remainingMinesCount==0 && board.gameIsCorrect()){
                // win
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(gameOver, board.gameBoard, board.userBoard);
                break; 
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        GUI.closeWindow();
        return 0;
    }
        
};


int main(){
    Minesweeper Minesweeper; 
    Minesweeper.runEngine();
}

