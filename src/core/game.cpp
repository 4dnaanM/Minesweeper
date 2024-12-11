#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <ctime>

#include "../gui/gui.cpp"
#include "../engine/engine.cpp"

// update: Refactor: base minesweeper, run can be engine or interactive

class Minesweeper{
    
    int L = 30; 
    int W = 16;
    
    //  _ _ _ _ _ _ _ _
    // |_|_|_|_|_|_|_|_| |
    // |_|_|_|_|_|_|_|_| W
    // |_|_|_|_|_|_|_|_| |
    // <-------L------->
    
    int totalMines = 99; 
    
    bool gameOver = false; 
    int remainingMinesCount = totalMines; 

    int MINE = -1; 
    int FLAGGED = -1;
    int HIDDEN = -2;

    std::vector<std::vector<int>> gameBoard                             // mine -> -1, else {0,1,2,3,4,5,6,7,8}
        = std::vector<std::vector<int>>(W, std::vector<int>(L,0));
    std::vector<std::vector<int>> userBoard                             // has values FLAGGED, HIDDEN or number
        = std::vector<std::vector<int>>(W, std::vector<int>(L,HIDDEN));

    int generateMines(){
        srand(time(0));
        int placedMines = 0; 
        
        while(placedMines<totalMines){

            int random_position = rand()%(W*L);
            int random_y = random_position/L;
            int random_x = random_position%L;

            if(gameBoard[random_y][random_x]!=MINE){
                gameBoard[random_y][random_x]=MINE;
                placedMines++;
            }
        }

        for(int x = 0; x < L; x++){
            for(int y = 0; y < W; y++){
                if(gameBoard[y][x]!=MINE)gameBoard[y][x] = countNeighbouringMines(y,x);
            }
        }
        return 0; 
    }

    bool gameIsCorrect(){
        for (int x = 0; x < L; x++){
            for (int y = 0; y < W; y++){

                if(gameBoard[y][x]==MINE && userBoard[y][x]!=FLAGGED){
                    return false;
                }
                
                if(userBoard[y][x]==FLAGGED && gameBoard[y][x]!=MINE){
                    return false;
                }

            }
        }
        return true;
    }
    
    bool inBoard(int y, int x){
        return (x>=0)&&(x<L)&&(y>=0)&&(y<W);
    }

    int countNeighbouringFlags(int y, int x){
        
        int neighbouring = 0; 

        int x_vals[] = {-1,0,1,-1,1,-1,0,1};
        int y_vals[] = {-1,-1,-1,0,0,1,1,1};

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+x_vals[neighbor];
            int n_y = y+y_vals[neighbor];
            if(inBoard(n_y,n_x)){
                neighbouring+=(userBoard[n_y][n_x]==FLAGGED);
            }
        }
        return neighbouring;
    }
    
    int countNeighbouringMines(int y, int x){
        
        int neighbouring = 0; 

        int x_vals[] = {-1,0,1,-1,1,-1,0,1};
        int y_vals[] = {-1,-1,-1,0,0,1,1,1};

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+x_vals[neighbor];
            int n_y = y+y_vals[neighbor];
            if(inBoard(n_y,n_x)){
                neighbouring+=(gameBoard[n_y][n_x]==MINE);
            }
        }
        return neighbouring;
    }

    int chord(int y, int x){

        int x_vals[] = {-1,0,1,-1,1,-1,0,1};
        int y_vals[] = {-1,-1,-1,0,0,1,1,1};

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+x_vals[neighbor];
            int n_y = y+y_vals[neighbor];
            if(inBoard(n_y,n_x)&&userBoard[n_y][n_x]==HIDDEN){
                // if it is hidden, click on it.
                if(gameBoard[n_y][n_x]==MINE){
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
        
        if(userBoard[y][x]!=HIDDEN)return 0;
        
        userBoard[y][x]=gameBoard[y][x];

        int neighbouringMines = countNeighbouringMines(y,x); 

        if(neighbouringMines==0){

            int x_vals[] = {-1,0,1,-1,1,-1,0,1};
            int y_vals[] = {-1,-1,-1,0,0,1,1,1};

            for (int neighbor = 0; neighbor<8; neighbor++){
                int n_x = x+x_vals[neighbor];
                int n_y = y+y_vals[neighbor];
                updateUserBoard(n_y,n_x);
            }
        }
        return 0;
    }

    int playInput(int y, int x, bool F){
        if(!inBoard(y,x)){
            // std::cout<<"Out of board!\n";
            return 0;
        }
        if(F){
            if(userBoard[y][x]==HIDDEN){
                remainingMinesCount--;
                userBoard[y][x] = FLAGGED;
            }
            else if(userBoard[y][x]==FLAGGED){
                remainingMinesCount++;
                userBoard[y][x] = HIDDEN;
            }
            else{
                // userBoard[y][x] is REVEALED
                // shows number of neighbors
                // chording

                int nFlags = countNeighbouringFlags(y,x);
                if(nFlags==gameBoard[y][x])chord(y,x);
            }
        }
        else {
            if(gameBoard[y][x]==MINE){
                gameOver = true;
            }
            else if(userBoard[y][x]==HIDDEN){
                updateUserBoard(y,x);
                
            }
            // revealed square left click: do nothing
        }
        return 0;
    }

public:
    Minesweeper(){

        userBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,HIDDEN));
        gameBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,0));
        generateMines();
    }

    int runGUI(){

        MinesweeperGUI GUI(L,W,MINE,HIDDEN,FLAGGED);

        GUI.displayBoard(gameOver, gameBoard, userBoard);

        while(GUI.windowIsOpen()){
            while(!gameOver && remainingMinesCount>0){
                int play_x; 
                int play_y;
                bool play_F;
                GUI.takeInteractiveInput(play_y, play_x, play_F);
                playInput(play_y,play_x,play_F);
                GUI.displayBoard(gameOver, gameBoard, userBoard);
            }
            if(remainingMinesCount==0 && gameIsCorrect()){
                // win
                GUI.displayBoard(gameOver, gameBoard, userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(gameOver, gameBoard, userBoard);
                break; 
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        GUI.closeWindow();
        return 0;
    }

    int runEngine(){
        MinesweeperGUI GUI(L,W,MINE,HIDDEN,FLAGGED);
        HelperEngine engine(L,W);

        GUI.displayBoard(gameOver, gameBoard, userBoard);

        while(GUI.windowIsOpen()){
            while(!gameOver && remainingMinesCount>0){
                int play_x; 
                int play_y;
                bool play_F;
                GUI.takeInteractiveInput(play_y, play_x, play_F);
                playInput(play_y,play_x,play_F);
                GUI.displayBoard(gameOver, gameBoard, userBoard);
                // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                std::vector<std::tuple<int,int,int>> moves = engine.makeMove(userBoard);

                for(auto move: moves){
                    // if(std::get<2>(move)==1)std::cout<<"Flagging ";
                    // else std::cout<<"Clicking ";
                    // std::cout<<"("<<std::get<1>(move)<<","<<std::get<0>(move)<<")\n";
                    playInput(std::get<0>(move),std::get<1>(move),std::get<2>(move)==1);
                    
                    GUI.displayBoard(gameOver, gameBoard, userBoard);
                    if(gameOver)break;
                    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
            if(remainingMinesCount==0 && gameIsCorrect()){
                // win
                GUI.displayBoard(gameOver, gameBoard, userBoard);
                break; 
            }
            else{
                // loss
                GUI.displayBoard(gameOver, gameBoard, userBoard);
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

