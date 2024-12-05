// AI solver
// No Guessing Mode

#include <iostream>
#include <vector>
#include <random>

class Minesweeper{
    
    int L; 
    int W;
    
    //  _ _ _ _ _ _ _ _
    // |_|_|_|_|_|_|_|_| |
    // |_|_|_|_|_|_|_|_| W
    // |_|_|_|_|_|_|_|_| |
    // <-------L------->
    
    int totalMines; 
    
    bool gameOver; 
    int remainingMinesCount; 

    int MINE; 
    int REVEALED;
    int FLAGGED;
    int HIDDEN;

    std::vector<std::vector<int>> gameBoard; // mine -> -1, else {0,1,2,3,4,5,6,7,8}
    std::vector<std::vector<int>> userBoard; // has values REVEALED, FLAGGED, HIDDEN

    int generateMines(){
        // check mines can be placed
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

    bool gameIsComplete(){
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

    void printUserBoard(){
        std::cout<<" ------------------------------\n";
        for (int y = 0; y < W; y++){
            std::cout<<"|";
            for (int x = 0; x < L; x++){

                if(userBoard[y][x]==HIDDEN)std::cout<<" ";
                else if(userBoard[y][x]==FLAGGED)std::cout<<"F";

                else std::cout<<gameBoard[y][x];

            }

            std::cout<<"|\n";
        }
        std::cout<<" ------------------------------\n";
        return;
    }

    void printUserBoardWithAllMines(){
        std::cout<<" ------------------------------\n";
        for (int y = 0; y < W; y++){
            std::cout<<"|";
            for (int x = 0; x < L; x++){

                if(gameBoard[y][x]==MINE)std::cout<<"*";

                else if(userBoard[y][x]==HIDDEN)std::cout<<" ";
                else if(userBoard[y][x]==FLAGGED)std::cout<<"F";

                else std::cout<<gameBoard[y][x];

            }

            std::cout<<"|\n";
        }
        std::cout<<" ------------------------------\n";
        return;
    }

    int takeInput(int& y, int& x, bool& F){
        // if not engine:
        std::cout<<"(F,x,y)=?\n";
        char mark_flag; 
        std::cin>>mark_flag;
        F = (mark_flag=='F');
        std::cin>>x>>y;
        return 0;
    }

    bool inBoard(int y, int x){
        return (x>=0)&&(x<L)&&(y>=0)&&(y<W);
    }

    int countNeighbouringMines(int y, int x){
        
        int neighbouringMines = 0; 

        int x_vals[] = {-1,0,1,-1,1,-1,0,1};
        int y_vals[] = {-1,-1,-1,0,0,1,1,1};

        for (int neighbor = 0; neighbor<7; neighbor++){    
            int n_x = x+x_vals[neighbor];
            int n_y = y+y_vals[neighbor];
            if(inBoard(n_y,n_x)){
                neighbouringMines+=(gameBoard[n_y][n_x]==MINE);
            }
        }
        return neighbouringMines;
    }

    int updateUserBoard(int y,int x){
        if(!inBoard(y,x))return 0;
        
        if(userBoard[y][x]==REVEALED)return 0;
        
        userBoard[y][x]=REVEALED;

        int neighbouringMines = countNeighbouringMines(y,x); 

        if(neighbouringMines==0){

            int x_vals[] = {-1,0,1,-1,1,-1,0,1};
            int y_vals[] = {-1,-1,-1,0,0,1,1,1};

            for (int neighbor = 0; neighbor<7; neighbor++){
                int n_x = x+x_vals[neighbor];
                int n_y = y+y_vals[neighbor];
                updateUserBoard(n_y,n_x);
            }
        }
        return 0;
    }

    int playInput(int& y, int& x, bool& F){
        if(!inBoard(y,x)){
            std::cout<<"Out of board!\n";
            return 0;
        }
        if(F){
            if(userBoard[y][x]!=FLAGGED)remainingMinesCount--;
            userBoard[y][x] = FLAGGED;
            printUserBoard(); 
        }
        else if(gameBoard[y][x]==MINE){
            gameOver = true;
        }
        else if(userBoard[y][x]!=HIDDEN){
            if(userBoard[y][x]==REVEALED)std::cout<<"Square already revealed!\n";// chording later
            else std::cout<<"Square flagged!\n";
        }
        else{
            updateUserBoard(y,x);
            printUserBoard(); 
        }
        return 0;
    }

public:
    Minesweeper():
        L(30),
        W(12),
        totalMines(99),
        MINE(-1),
        HIDDEN(0),
        REVEALED(1),
        FLAGGED(2),
        gameOver(false),
        remainingMinesCount(totalMines)
    {
        userBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,HIDDEN));
        gameBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,0));
        generateMines();
        // printUserBoardWithAllMines();
    }

    int run(){
        while(!gameOver&&remainingMinesCount){
            int play_x; 
            int play_y;
            bool play_F;
            takeInput(play_y, play_x, play_F);
            // clear the screen here
            playInput(play_y,play_x,play_F);
        }
        if(remainingMinesCount==0 && gameIsComplete()){
            // win
            std::cout<<":)\n";
            printUserBoard();
        }
        else{
            std::cout<<":(\n";
            printUserBoardWithAllMines();
            // loss
        }
        return 0;
    }
};


int main(){
    Minesweeper Minesweeper; 
    Minesweeper.run(); 
}