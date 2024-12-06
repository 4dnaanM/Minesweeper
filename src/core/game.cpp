#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <thread>
#include <chrono>

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
    int remainingMinesCount = 99; 

    int MINE = -1; 
    int REVEALED = 1;
    int FLAGGED = 2;
    int HIDDEN = 0;

    std::vector<std::string> TEXTURE_PATHS = {
        "../assets/0.svg.png",       // Revealed empty cell
        "../assets/1.svg.png",       // Number 1
        "../assets/2.svg.png",       // Number 2
        "../assets/3.svg.png",       // Number 3
        "../assets/4.svg.png",       // Number 4
        "../assets/5.svg.png",       // Number 5
        "../assets/6.svg.png",       // Number 6
        "../assets/7.svg.png",       // Number 7
        "../assets/8.svg.png",       // Number 8
        "../assets/Hidden.svg.png",  // Hidden cell
        "../assets/Flag.svg.png",    // Flagged cell
        "../assets/Mine.svg.png"     // Mine
    };

    std::vector<sf::Texture> textures; 
    std::vector<sf::Sprite> sprites; 

    int CELL_SIZE = 40;

    std::vector<std::vector<int>> gameBoard                             // mine -> -1, else {0,1,2,3,4,5,6,7,8}
        = std::vector<std::vector<int>>(W, std::vector<int>(L,0));
    std::vector<std::vector<int>> userBoard                             // has values REVEALED, FLAGGED, HIDDEN
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

    int takeConsoleInput(int& y, int& x, bool& F){
        // if not engine:
        std::cout<<"(F,x,y)=?\n";
        char mark_flag; 
        std::cin>>mark_flag;
        F = (mark_flag=='F');
        std::cin>>x>>y;
        return 0;
    }

    int takeInput(int& y, int& x, bool& F, sf::RenderWindow& window){
        sf::Event event;
        while(window.waitEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::MouseButtonPressed){
                int click_x = event.mouseButton.x;
                int click_y = event.mouseButton.y;
                
                x = click_x/CELL_SIZE;
                y = click_y/CELL_SIZE;

                if(event.mouseButton.button == sf::Mouse::Left){
                    F = false;
                }
                else if(event.mouseButton.button == sf::Mouse::Right){
                    F = true;
                }
                return 0;
            }
        }
        return 0;
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
                // if it is hidden, cl
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
        
        userBoard[y][x]=REVEALED;

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

    int playInput(int& y, int& x, bool& F){
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
            // printUserBoard(); 
        }
        else {
            if(gameBoard[y][x]==MINE){
                gameOver = true;
            }
            else if(userBoard[y][x]==HIDDEN){
                updateUserBoard(y,x);
                // printUserBoard(); 
                
            }
            // revealed square left click: do nothing
        }
        return 0;
    }

    int displayBoard(sf::RenderWindow& window){
        window.clear(sf::Color::White);
        for (int y = 0; y < W; y++) {
            for (int x = 0; x < L; x++) {
                int state; 
                if(gameOver && gameBoard[y][x]==MINE)state = 11;
                else if(userBoard[y][x]==REVEALED)state = gameBoard[y][x];
                else if(userBoard[y][x]==FLAGGED)state = 10;
                else state = 9;
                
                sf::Sprite& sprite = sprites[state];
                sprite.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                sprite.setScale(
                    static_cast<float>(CELL_SIZE) / textures[state].getSize().x,
                    static_cast<float>(CELL_SIZE) / textures[state].getSize().y
                );
                window.draw(sprite);
                
            }
        }
        window.display();
        return 0; 
    }

public:
    Minesweeper(){

    
        for (auto path : TEXTURE_PATHS) {
            sf::Texture texture;

            if (!texture.loadFromFile(path)) {
                std::cerr << "Failed to load texture: " << path << "\n";
                continue; 
            }

            textures.push_back(texture);
            
        }

        for(int i = 0; i< textures.size(); i++){
            sf::Sprite sprite(textures[i]);
            sprites.push_back(sprite);
        }

        userBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,HIDDEN));
        gameBoard = std::vector<std::vector<int>>(W,std::vector<int>(L,0));
        generateMines();
        // printUserBoardWithAllMines();
    }

    int run(){
        int WIDTH = L * CELL_SIZE;
        int HEIGHT = W * CELL_SIZE;
        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

        displayBoard(window);
        while(window.isOpen()){
            while(!gameOver && remainingMinesCount>0){
                int play_x; 
                int play_y;
                bool play_F;
                takeInput(play_y, play_x, play_F,window);
                playInput(play_y,play_x,play_F);
                displayBoard(window);
            }
            if(remainingMinesCount==0 && gameIsCorrect()){
                // win
                displayBoard(window);
                break; 
                // printUserBoard();
            }
            else{;
                displayBoard(window);
                break; 
                // printUserBoardWithAllMines();
                // loss
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        window.close();
        return 0;
    }
        
};


int main(){
    Minesweeper Minesweeper; 
    Minesweeper.run();
    
}