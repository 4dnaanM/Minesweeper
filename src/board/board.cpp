#ifndef BOARD
#define BOARD

#include "../utils/utils.cpp"


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

            if(gameBoard[random_y][random_x]!=CellState::MINE){
                gameBoard[random_y][random_x]=CellState::MINE;
                placedMines++;
            }
        }

        for(int x = 0; x < L; x++){
            for(int y = 0; y < W; y++){
                if(gameBoard[y][x]!=CellState::MINE)gameBoard[y][x] = countNeighbouringMines(y,x);
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
                neighbouring+=(userBoard[n_y][n_x]==CellState::FLAGGED);
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
                neighbouring += (gameBoard[n_y][n_x] == CellState::MINE);
            }
        }

        return neighbouring;
    }

    int chord(int y, int x, bool& gameOver){

        for (int neighbor = 0; neighbor<8; neighbor++){    
            int n_x = x+traverse_x[neighbor];
            int n_y = y+traverse_y[neighbor];
            if(inBoard(n_y,n_x)&&userBoard[n_y][n_x]==CellState::HIDDEN){
                // if it is hidden, click on it.
                if(gameBoard[n_y][n_x]==CellState::MINE){
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
        if(userBoard[y][x]!=CellState::HIDDEN)return 0;
        // not intended to be called on a mine
        assert(gameBoard[y][x] != CellState::MINE);
        
        userBoard[y][x] = gameBoard[y][x];

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

public:
    int L;
    int W;
    int totalMines;
    Board gameBoard;
    Board userBoard;
    Board board; 
    
    MinesweeperBoard(GameParams p){
        L = p.L;
        W = p.W;
        totalMines = p.totalMines;
        gameBoard = Board(W, std::vector<Cell>(L,CellState::EMPTY));
        userBoard = Board(W, std::vector<Cell>(L,CellState::HIDDEN));
        generateMines();
    }

    MinesweeperBoard(){
        GameParams p = GameParams();
        *this = MinesweeperBoard(p);
    }
    
    bool gameIsCorrect(){
        for (int x = 0; x < L; x++){
            for (int y = 0; y < W; y++){

                if(gameBoard[y][x]==CellState::MINE && userBoard[y][x]!=CellState::FLAGGED){
                    return false;
                }
                
                if(userBoard[y][x]==CellState::FLAGGED && gameBoard[y][x]!=CellState::MINE){
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
            return 0;
        }
        if(C == RIGHT_CLICK){
            if(userBoard[y][x]==CellState::HIDDEN){
                params.remainingMinesCount--;
                userBoard[y][x] = CellState::FLAGGED;
            }
            else if(userBoard[y][x]==CellState::FLAGGED){
                params.remainingMinesCount++;
                userBoard[y][x] = CellState::HIDDEN;
            }
            else{
                int nFlags = countNeighbouringFlags(y,x);
                if(nFlags==gameBoard[y][x])chord(y,x,params.gameOver);
            }
        }
        else {
            if(gameBoard[y][x]==CellState::MINE){
                params.gameOver = true;
            }
            else if(userBoard[y][x]==CellState::HIDDEN){
                updateUserBoard(y,x);
                
            }
            // revealed square left click: do nothing
        }
        return 0;
    }
        
};

#endif