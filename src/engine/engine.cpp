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

    virtual std::vector<std::tuple<int,int,int>> makeMove(const std::vector<std::vector<int>>& userBoard) = 0;

public:
    AbstractEngine(int L, int W):L(L),W(W){}
};

class HelperEngine: public AbstractEngine{

public:    
    HelperEngine(int L, int W):AbstractEngine(L,W){}
    
    std::vector<std::tuple<int,int,int>> makeMove(const std::vector<std::vector<int>>& userBoard){
        std::vector<std::vector<int>> ourBoard = userBoard; 
        std::set<std::tuple<int,int,int>> moves;
        for(int y = 0; y<W; y++){
            for(int x = 0; x<L; x++){
                if(ourBoard[y][x]==HIDDEN || ourBoard[y][x]==FLAGGED || ourBoard[y][x]==CANTFLAG)continue;
                if(baseCheckFlagNeighbors(y,x,ourBoard)){
                    std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                    for(auto neighbor: hiddenNeighbors){
                        int n_x = neighbor.second; 
                        int n_y = neighbor.first; 
                        ourBoard[n_y][n_x]=FLAGGED; 
                        if (moves.find(std::make_tuple(n_y, n_x, 1)) == moves.end()) {
                        }
                        moves.insert(std::make_tuple(n_y, n_x, 1));
                    }
                }
                if(baseCheckSatisfied(y,x,ourBoard)){
                    std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                    moves.insert(std::make_tuple(y,x,1));
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

class ProbabilisticEngine: public AbstractEngine{

    void backtrack(int cell, std::vector<std::vector<int>>& backTrackState, std::vector<std::vector<float>>& probabilities, int remainingMines, int& foundSolutions){
        
        if(remainingMines<0)return; 

        if(cell==L*W){
            if(remainingMines!=0)return; 
            foundSolutions++; 
            for(int i = 0; i<W; i++){
                for(int j = 0; j<L; j++){
                    if(backTrackState[i][j]==FLAGGED)probabilities[i][j]+=1;
                }
            }
        }

        int y = cell/L;
        int x = cell%L;

        if(backTrackState[y][x]==FLAGGED){
            if(remainingMines>0)backtrack(cell+1, backTrackState, probabilities, remainingMines-1,foundSolutions);
            return; 
        }
        if(backTrackState[y][x]==CANTFLAG){
            backtrack(cell+1, backTrackState, probabilities, remainingMines,foundSolutions);
            return; 
        }
        if(backTrackState[y][x]!=HIDDEN){
            // it is revealed
            backtrack(cell+1, backTrackState, probabilities, remainingMines,foundSolutions);
            return; 
        }
        
        // it is hidden
        // first see if there is some deterministic thing we can do: 
        if(baseCheckFlagNeighbors(y,x,backTrackState)){
            // try to flag all neighbors
            // if that breaks some constraint, exit
            std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,backTrackState);
            if(remainingMines<hiddenNeighbors.size())return;

            //flag all hidden neighbors
            for(auto neighbor: hiddenNeighbors){
                backTrackState[neighbor.first][neighbor.second] = FLAGGED;
            }

            //check consistent around all newlyflaggeds. 
            bool consistent = true;
            for(auto neighbor:hiddenNeighbors){
                consistent = consistent && consistentAround(neighbor.first, neighbor.second, backTrackState);
                if(!consistent) break;
            }
            //if none of the newly flagged neighbors breaks the constraint, continue
            if(consistent){
                int countedMines = 0; 
                for (auto neighbor: hiddenNeighbors){
                    if(neighbor.first<y || (neighbor.first==y && neighbor.second<=x)){
                        countedMines++;
                    }
                }
                
                backtrack(cell+1, backTrackState, probabilities, remainingMines-countedMines,foundSolutions);
            }
            // fix backTrackState
            for(auto neighbor: hiddenNeighbors){
                backTrackState[neighbor.first][neighbor.second] = HIDDEN;
            }
        }
        else if(baseCheckSatisfied(y,x,backTrackState)){
            // try to turn all neighbors into CANTFLAGs
            // if that breaks some constraint, exit
            std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,backTrackState);

            //cantflag all hiddens
            for(auto neighbor: hiddenNeighbors){
                backTrackState[neighbor.first][neighbor.second] = CANTFLAG;
            }

            //check consistent around all newlycantflaggeds. 
            bool consistent = true;
            for(auto neighbor:hiddenNeighbors){
                consistent = consistent && consistentAround(neighbor.first, neighbor.second, backTrackState);
                if(!consistent) break;
            }
            //if none of the newly cant-flagged neighbors breaks the constraint, continue
            if(consistent){
                backtrack(cell+1, backTrackState, probabilities, remainingMines,foundSolutions);
            }
            // fix backtrackstate
            for(auto neighbor: hiddenNeighbors){
                backTrackState[neighbor.first][neighbor.second] = HIDDEN;
            }
        }
        else{
            // do two things: place flag (if possible)
            backTrackState[y][x] = FLAGGED;
            if(consistentAround(y,x,backTrackState)){
                backtrack(cell+1, backTrackState, probabilities, remainingMines-1,foundSolutions);
            }
            // or place cantflag (if possible). 
            backTrackState[y][x] = CANTFLAG;
            if(consistentAround(y,x,backTrackState)){
                backtrack(cell+1, backTrackState, probabilities, remainingMines,foundSolutions);
            }
            backTrackState[y][x] = HIDDEN;
            return;
        }

    }

    std::vector<std::vector<float>> findProbabilities(const std::vector<std::vector<int>>& userBoard){
        std::vector<std::vector<float>> probabilities(W, std::vector<float>(L,0.0));
        std::vector<std::vector<int>> backTrackState = userBoard;
        int foundSolutions = 0;
        backtrack(0,backTrackState,probabilities,99,foundSolutions);
        std::cout<<foundSolutions<<" Solutions found!\n";
        for(int i = 0; i<W; i++){
            for(int j = 0; j<L; j++){
                probabilities[i][j]/=foundSolutions;
            }
        }
        return probabilities; 
    }

public: 
    ProbabilisticEngine(int L, int W):AbstractEngine(L,W){}
    
    std::vector<std::tuple<int,int,int>> makeMove(const std::vector<std::vector<int>>& userBoard){
        std::vector<std::vector<int>> ourBoard = userBoard; 
        std::set<std::tuple<int,int,int>> moves;
        for(int y = 0; y<W; y++){
            for(int x = 0; x<L; x++){
                if(ourBoard[y][x]==HIDDEN || ourBoard[y][x]==FLAGGED || ourBoard[y][x]==CANTFLAG)continue;
                if(baseCheckFlagNeighbors(y,x,ourBoard)){
                    std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                    for(auto neighbor: hiddenNeighbors){
                        int n_x = neighbor.second; 
                        int n_y = neighbor.first; 
                        ourBoard[n_y][n_x]=FLAGGED; 
                        if (moves.find(std::make_tuple(n_y, n_x, 1)) == moves.end()) {
                        }
                        moves.insert(std::make_tuple(n_y, n_x, 1));
                    }
                }
                if(baseCheckSatisfied(y,x,ourBoard)){
                    std::vector<std::pair<int,int>>hiddenNeighbors = getAllHiddenNeighbors(y,x,ourBoard);
                    moves.insert(std::make_tuple(y,x,1));
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