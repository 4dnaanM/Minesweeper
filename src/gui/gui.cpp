#include "../utils/utils.cpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class AbstractGUI{
public: 
    virtual int takeInteractiveInput(Move& Move) = 0;
    virtual int displayBoard(GameParams params, const GameBoard& gameBoard, const UserBoard& userBoard) = 0;
    virtual void closeWindow() = 0;
    virtual bool windowIsOpen() = 0;
};

class MinesweeperGUI{
    GUIParams params;
    sf::RenderWindow window;

    int getCellCoordinates(int& y, int& x, int click_y, int click_x){
        sf::View view = window.getView();
        sf::Vector2f viewSize = view.getSize();
        sf::Vector2u windowSize = window.getSize();

        float scale_x = viewSize.x / windowSize.x;
        float scale_y = viewSize.y / windowSize.y;
        
        x = click_x*scale_x/params.CELL_SIZE;
        y = click_y*scale_y/params.CELL_SIZE;
        return 0; 
    }

public: 
    int takeInteractiveInput(Move& move){
        sf::Event event;
        while(window.waitEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::MouseButtonPressed){
                int click_x = event.mouseButton.x;
                int click_y = event.mouseButton.y;
                
                int y;
                int x;
                ClickType F;
                
                getCellCoordinates(y,x,click_y,click_x);

                if(event.mouseButton.button == sf::Mouse::Left){
                    F = LEFT_CLICK;
                }
                else if(event.mouseButton.button == sf::Mouse::Right){
                    F = RIGHT_CLICK;
                }
                move = std::make_tuple(y,x,F);
                return 0;
            }
        }
        return 0;
    }

    int displayBoard(GameParams p, const GameBoard& gameBoard, const UserBoard& userBoard){
        window.clear(sf::Color::White);
        for (int y = 0; y < params.W; y++) {
            for (int x = 0; x < params.L; x++) {
                int state; 
                if(p.gameOver && gameBoard[y][x]==GAME_MINE)state = 11;
                else if(userBoard[y][x]==USER_FLAGGED)state = 10;
                else if(userBoard[y][x]==USER_HIDDEN)state = 9;
                else state = gameBoard[y][x];

                sf::Sprite& sprite = params.sprites[state];

                sprite.setPosition(x * params.CELL_SIZE, y * params.CELL_SIZE);
                sprite.setScale(
                    static_cast<float>(params.CELL_SIZE) / params.textures[state].getSize().x,
                    static_cast<float>(params.CELL_SIZE) / params.textures[state].getSize().y
                );
                window.draw(sprite);
                
            }
        }
        window.display();
        return 0; 
    }

    MinesweeperGUI(GameParams p, int CELL_SIZE){
        params = GUIParams(p.L, p.W, CELL_SIZE);
        window.create(sf::VideoMode(p.L*CELL_SIZE, p.W*CELL_SIZE), "Minesweeper");
    }
    MinesweeperGUI(){
        GameParams p = GameParams();
        GUIParams params = GUIParams(p.L, p.W, 40);
        window.create(sf::VideoMode(params.L*40, params.W*40), "Minesweeper");
    }

    void closeWindow(){
        window.close();
        return; 
    }

    bool windowIsOpen(){
        return window.isOpen();
    }
};