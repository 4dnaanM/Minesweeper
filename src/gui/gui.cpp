#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class MinesweeperGUI{
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

    int CELL_SIZE = 40;
    int L;
    int W;
    int MINE; 
    int HIDDEN; 
    int FLAGGED;

    sf::RenderWindow window;

    int getCellCoordinates(int& y, int& x, int click_y, int click_x){
        sf::View view = window.getView();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2u windowSize = window.getSize();

        float scale_x = viewSize.x / windowSize.x;
        float scale_y = viewSize.y / windowSize.y;
        
        x = click_x*scale_x/CELL_SIZE;
        y = click_y*scale_y/CELL_SIZE;
        return 0; 
    }

public: 
    int takeInteractiveInput(int& y, int& x, bool& F){
        sf::Event event;
        while(window.waitEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::MouseButtonPressed){
                int click_x = event.mouseButton.x;
                int click_y = event.mouseButton.y;

                getCellCoordinates(y,x,click_y,click_x);
                
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

    int displayBoard(int gameOver, std::vector<std::vector<int>>& gameBoard, std::vector<std::vector<int>>& userBoard){
        window.clear(sf::Color::White);
        for (int y = 0; y < W; y++) {
            for (int x = 0; x < L; x++) {
                int state; 
                if(gameOver && gameBoard[y][x]==MINE)state = 11;
                else if(userBoard[y][x]==FLAGGED)state = 10;
                else if(userBoard[y][x]==HIDDEN)state = 9;
                else state = gameBoard[y][x];

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

    MinesweeperGUI(int L, int W, int MINE, int HIDDEN, int FLAGGED): 
    L(L), W(W), 
    MINE(MINE), HIDDEN(HIDDEN), FLAGGED(FLAGGED), 
    window(sf::VideoMode(L * CELL_SIZE, W * CELL_SIZE), "Minesweeper")
    {
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
    }

    void closeWindow(){
        window.close();
        return; 
    }

    bool windowIsOpen(){
        return window.isOpen();
    }
};
