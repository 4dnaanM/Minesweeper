#include "../utils/utils.cpp"

class AbstractEngine{
protected:     

    GameParams params;
    std::vector<int> traverse_y = {-1,-1,-1,0,0,1,1,1};
    std::vector<int> traverse_x = {-1,0,1,-1,1,-1,0,1};

    bool inBoard(int y, int x){
        return (x>=0)&&(x<params.L)&&(y>=0)&&(y<params.W);
    }

    std::vector<std::pair<int,int>> getAllHiddenNeighbors(int y, int x, const Board& userBoard){
        
        std::vector<std::pair<int,int>> hiddenNeighbors;

        for(int n = 0; n<8; n++){
            if(inBoard(y+traverse_y[n],x+traverse_x[n])){
                if(userBoard[y+traverse_y[n]][x+traverse_x[n]]==CellState::HIDDEN){
                    hiddenNeighbors.push_back({y+traverse_y[n],x+traverse_x[n]});
                }
            }
        }
        return hiddenNeighbors;
    }

    std::tuple<int,int,int> countAround(int y, int x, const Board& userBoard){
        int flaggedNeighbors = 0; 
        int hiddenNeighbors = 0;
        int cantFlagNeighbors = 0; 

        for(int n = 0; n<8; n++){
            if(inBoard(y+traverse_y[n],x+traverse_x[n])){
                if(userBoard[y+traverse_y[n]][x+traverse_x[n]]==CellState::FLAGGED)flaggedNeighbors++;
                if(userBoard[y+traverse_y[n]][x+traverse_x[n]]==CellState::HIDDEN)hiddenNeighbors++;
                if(userBoard[y+traverse_y[n]][x+traverse_x[n]]==CellState::CANTFLAG)cantFlagNeighbors++;
            }
        }
        return std::make_tuple(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors);
    }

    bool baseCheckFlagNeighbors(int y, int x, const Board& userBoard){

        if(userBoard[y][x]==CellState::HIDDEN || userBoard[y][x]==CellState::FLAGGED|| userBoard[y][x]==CellState::CANTFLAG)return false;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);

        return userBoard[y][x] == hiddenNeighbors+flaggedNeighbors;
    }

    bool baseCheckSatisfied(int y, int x, const Board& userBoard){
        if(userBoard[y][x]==CellState::HIDDEN || userBoard[y][x]==CellState::FLAGGED|| userBoard[y][x]==CellState::CANTFLAG)return false;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);

        return userBoard[y][x] == flaggedNeighbors;
    }

    bool consistentAround(int y, int x, const Board& userBoard){
        if(userBoard[y][x]==CellState::HIDDEN || userBoard[y][x]==CellState::FLAGGED|| userBoard[y][x]==CellState::CANTFLAG)return true;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);
        
        return flaggedNeighbors <= userBoard[y][x] && flaggedNeighbors+hiddenNeighbors+cantFlagNeighbors >= userBoard[y][x];
    }

    virtual std::vector<Move> returnMoves(const Board& userBoard) = 0;

public:
    AbstractEngine(GameParams p):params(p){}
};

class HelperEngine: public AbstractEngine{

public:    
    HelperEngine(GameParams p):AbstractEngine(p){}
    
    std::vector<Move> returnMoves(const Board& userBoard){
        Board engineBoard = userBoard; 
        std::vector<Move> moves;
        int earlierSize = -1; 
        while(earlierSize<static_cast<int>(moves.size())){
            earlierSize = static_cast<int>(moves.size()); 
            for(int y = 0; y<params.W; y++){
                for(int x = 0; x<params.L; x++){
                    if(engineBoard[y][x]==CellState::HIDDEN || engineBoard[y][x]==CellState::FLAGGED || engineBoard[y][x]==CellState::CANTFLAG)continue;
                    
                    if(baseCheckFlagNeighbors(y,x,engineBoard)){
                        std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,engineBoard);
                        for(auto neighbor: hiddenNeighbors){
                            int n_x = neighbor.second; 
                            int n_y = neighbor.first; 
                            if(engineBoard[n_y][n_x]!=CellState::FLAGGED){
                                engineBoard[n_y][n_x]=CellState::FLAGGED;
                                moves.push_back(std::make_tuple(n_y, n_x, RIGHT_CLICK));
                            }
                        }
                    }

                    if(baseCheckSatisfied(y,x,engineBoard)){
                        std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,engineBoard);
                        for(auto neighbor: hiddenNeighbors){
                            int n_x = neighbor.second; 
                            int n_y = neighbor.first; 
                            if(engineBoard[n_y][n_x]!=CellState::CANTFLAG){
                                engineBoard[n_y][n_x]=CellState::CANTFLAG; 
                                moves.push_back(std::make_tuple(n_y, n_x, LEFT_CLICK));
                            }
                        }
                        // can't just chord: How will I check duplicate moves?
                    }
                }
            }
        }
        return moves; 
    }

};