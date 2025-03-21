#ifndef MINESWEEPER
#define MINESWEEPER

#include "../gui/gui.cpp"
#include "../board/board.cpp"
#include "../engine/engine.cpp"

class Minesweeper{
    
    GameParams params;

    MinesweeperBoard board;
    MinesweeperGUI GUI;
    HelperEngine engine; 

public:
    Minesweeper():
        params(),
        board(params),
        GUI(params, 40),
        engine(params)
        {}

    int runGUI(){

        GUI.displayBoard(params, board.gameBoard, board.userBoard);

        while(GUI.windowIsOpen()){
            while(!params.gameOver && params.remainingMinesCount>0){
                Move M;
                GUI.takeInteractiveInput(M);
                board.playInput(M, params);
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
            }
            if(params.remainingMinesCount==0 && board.gameIsCorrect()){
                // win
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
                break; 
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        GUI.closeWindow();
        return 0;
    }

    int runEngine(){

        GUI.displayBoard(params, board.gameBoard, board.userBoard);

        while(GUI.windowIsOpen()){
            while(!params.gameOver && params.remainingMinesCount>0){
                Move M;
                GUI.takeInteractiveInput(M);
                board.playInput(M, params);
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
                std::vector<Move> moves = engine.returnMoves(board.userBoard);

                for(auto move: moves){
                    board.playInput(move,params);
                    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    GUI.displayBoard(params, board.gameBoard, board.userBoard);
                    if(params.gameOver)break;
                }
            }
            if(params.remainingMinesCount==0 && board.gameIsCorrect()){
                // win
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(params, board.gameBoard, board.userBoard);
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

#endif