#include <vector>

#ifndef UTILS
#define UTILS

#include <vector>
#include <SFML/Graphics.hpp>

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
    int CELL_SIZE;
    int L; 
    int W; 
    std::vector<std::string> TEXTURE_PATHS = {
        "assets/0.svg.png",       // Revealed empty cell
        "assets/1.svg.png",       // Number 1
        "assets/2.svg.png",       // Number 2
        "assets/3.svg.png",       // Number 3
        "assets/4.svg.png",       // Number 4
        "assets/5.svg.png",       // Number 5
        "assets/6.svg.png",       // Number 6
        "assets/7.svg.png",       // Number 7
        "assets/8.svg.png",       // Number 8
        "assets/Hidden.svg.png",  // Hidden cell
        "assets/Flag.svg.png",    // Flagged cell
        "assets/Mine.svg.png"     // Mine
    };
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    
    GUIParams(int L,int W,int CELL_SIZE){
        CELL_SIZE = CELL_SIZE;
        L = L; 
        W = W;
        for (auto path : TEXTURE_PATHS) {
            sf::Texture texture;
            textures.push_back(texture);
        }
        for(int i = 0; i< textures.size(); i++){
            sf::Sprite sprite(textures[i]);
            sprites.push_back(sprite);
        }
    }

    GUIParams(){
        GUIParams(30,16,40);
    }
};

enum GameCellState{
    GAME_MINE,
    GAME_EMPTY,
    GAME_ONE,
    GAME_TWO,
    GAME_THREE,
    GAME_FOUR,
    GAME_FIVE,
    GAME_SIX,
    GAME_SEVEN,
    GAME_EIGHT
};

enum UserCellState {
    USER_HIDDEN,
    USER_FLAGGED,
    USER_REVEALED,
    USER_CANTFLAG, // to be used only by engines, GUI treats it as HIDDEN
    USER_MINE,
    USER_EMPTY,
    USER_ONE,
    USER_TWO,
    USER_THREE,
    USER_FOUR,
    USER_FIVE,
    USER_SIX,
    USER_SEVEN,
    USER_EIGHT
};

enum ClickType{
    LEFT_CLICK,
    RIGHT_CLICK
};

typedef std::vector<std::vector<GameCellState>> GameBoard;
typedef std::vector<std::vector<UserCellState>> UserBoard;

typedef std::tuple<int,int,ClickType> Move;
#endif