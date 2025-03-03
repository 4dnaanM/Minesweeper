#include <vector>
#include <tuple>
#include <set>
#include <iostream>

class AbstractEngine{
protected:     

    int HIDDEN = -2; 
    int FLAGGED = -1;
    int CANTFLAG = -3; 

    int L; 
    int W;

    bool inBoard(int y, int x){
        return (x>=0)&&(x<L)&&(y>=0)&&(y<W);
    }

    std::vector<std::pair<int,int>> getAllHiddenNeighbors(int y, int x, const std::vector<std::vector<int>>& userBoard){

        std::vector<int> y_n_idxs = {-1,-1,-1,0,0,1,1,1};
        std::vector<int> x_n_idxs = {-1,0,1,-1,1,-1,0,1};
        
        std::vector<std::pair<int,int>> hiddenNeighbors;

        for(int n = 0; n<8; n++){
            if(inBoard(y+y_n_idxs[n],x+x_n_idxs[n])){
                if(userBoard[y+y_n_idxs[n]][x+x_n_idxs[n]]==HIDDEN){
                    hiddenNeighbors.push_back({y+y_n_idxs[n],x+x_n_idxs[n]});
                }
            }
        }
        return hiddenNeighbors;
    }

    std::tuple<int,int,int> countAround(int y, int x, const std::vector<std::vector<int>>& userBoard){
        std::vector<int> y_n_idxs = {-1,-1,-1,0,0,1,1,1};
        std::vector<int> x_n_idxs = {-1,0,1,-1,1,-1,0,1};
        int flaggedNeighbors = 0; 
        int hiddenNeighbors = 0;
        int cantFlagNeighbors = 0; 

        for(int n = 0; n<8; n++){
            if(inBoard(y+y_n_idxs[n],x+x_n_idxs[n])){
                if(userBoard[y+y_n_idxs[n]][x+x_n_idxs[n]]==FLAGGED)flaggedNeighbors++;
                if(userBoard[y+y_n_idxs[n]][x+x_n_idxs[n]]==HIDDEN)hiddenNeighbors++;
                if(userBoard[y+y_n_idxs[n]][x+x_n_idxs[n]]==CANTFLAG)cantFlagNeighbors++;
            }
        }
        return std::make_tuple(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors);
    }

    bool baseCheckFlagNeighbors(int y, int x, const std::vector<std::vector<int>>& userBoard){

        if(userBoard[y][x]==HIDDEN || userBoard[y][x]==FLAGGED|| userBoard[y][x]==CANTFLAG)return false;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);

        return hiddenNeighbors+flaggedNeighbors == userBoard[y][x];
    }

    bool baseCheckSatisfied(int y, int x, const std::vector<std::vector<int>>& userBoard){
        if(userBoard[y][x]==HIDDEN || userBoard[y][x]==FLAGGED|| userBoard[y][x]==CANTFLAG)return false;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);

        return flaggedNeighbors == userBoard[y][x];
    }

    bool consistentAround(int y, int x, const std::vector<std::vector<int>>& userBoard){
        if(userBoard[y][x]==HIDDEN || userBoard[y][x]==FLAGGED|| userBoard[y][x]==CANTFLAG)return true;
        
        int flaggedNeighbors;
        int hiddenNeighbors;
        int cantFlagNeighbors;
        std::tie(flaggedNeighbors, hiddenNeighbors, cantFlagNeighbors) = countAround(y,x,userBoard);
        
        return flaggedNeighbors<= userBoard[y][x] && flaggedNeighbors+hiddenNeighbors+cantFlagNeighbors >= userBoard[y][x];
    }

    virtual std::vector<std::tuple<int,int,int>> returnMoves(const std::vector<std::vector<int>>& userBoard) = 0;

public:
    AbstractEngine(int L, int W):L(L),W(W){}
};

class HelperEngine: public AbstractEngine{

public:    
    HelperEngine(int L, int W):AbstractEngine(L,W){}
    
    std::vector<std::tuple<int,int,int>> returnMoves(const std::vector<std::vector<int>>& userBoard){
        std::vector<std::vector<int>> ourBoard = userBoard; 
        std::set<std::tuple<int,int,int>> moves;
        int earlierSize = -1; 
        while(earlierSize<static_cast<int>(moves.size())){
            earlierSize = static_cast<int>(moves.size()); 
            for(int y = 0; y<W; y++){
                for(int x = 0; x<L; x++){
                    if(ourBoard[y][x]==HIDDEN || ourBoard[y][x]==FLAGGED || ourBoard[y][x]==CANTFLAG)continue;
                    if(baseCheckFlagNeighbors(y,x,ourBoard)){
                        std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                        for(auto neighbor: hiddenNeighbors){
                            int n_x = neighbor.second; 
                            int n_y = neighbor.first; 
                            ourBoard[n_y][n_x]=FLAGGED;
                            moves.insert(std::make_tuple(n_y, n_x, 1));
                        }
                    }

                    if(baseCheckSatisfied(y,x,ourBoard)){
                        std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                        for(auto neighbor: hiddenNeighbors){
                            int n_x = neighbor.second; 
                            int n_y = neighbor.first; 
                            ourBoard[n_y][n_x]=CANTFLAG; 
                            moves.insert(std::make_tuple(n_y, n_x, 0));
                        }
                        // moves.insert(std::make_tuple(y,x,1));
                    }
                }
            }
        }
        
        std::vector<std::tuple<int,int,int>> movesVector;
        for(auto move: moves){
            movesVector.push_back(move);
        }
        return movesVector; 
    }

};