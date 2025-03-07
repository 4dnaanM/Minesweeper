#ifndef MINESWEEPER_GUI
#define MINESWEEPER_GUI

#include "../utils/utils.cpp"

class AbstractGUI{
public: 
    virtual int takeInteractiveInput(Move& Move) = 0;
    virtual int displayBoard(GameParams params, const Board& gameBoard, const Board& userBoard) = 0;
    virtual void closeWindow() = 0;
    virtual bool windowIsOpen() = 0;
};

class MinesweeperGUI{
    GUIParams params;
    sf::RenderWindow window;

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

    int displayBoard(GameParams p, const Board& gameBoard, const Board& userBoard){
        window.clear(sf::Color::White);
        for (int y = 0; y < params.W; y++) {
            for (int x = 0; x < params.L; x++) {
                int state; 
                if(p.gameOver && gameBoard[y][x]==CellState::MINE)state = 11;
                else if(userBoard[y][x]==CellState::FLAGGED)state = 10;
                else if(userBoard[y][x]==CellState::HIDDEN)state = 9;
                else state = gameBoard[y][x];

                sf::Sprite& sprite = sprites[state];

                sprite.setPosition(x * params.CELL_SIZE, y * params.CELL_SIZE);
                sprite.setScale(
                    static_cast<float>(params.CELL_SIZE) / textures[state].getSize().x,
                    static_cast<float>(params.CELL_SIZE) / textures[state].getSize().y
                );
                window.draw(sprite);
                
            }
        }
        window.display();
        return 0; 
    }

    MinesweeperGUI(GameParams p, int CELL_SIZE){
        params = GUIParams(p.L, p.W, CELL_SIZE);
        for (auto path : TEXTURE_PATHS) {
            sf::Texture texture;
            texture.loadFromFile(path);
            textures.push_back(texture);
        }
        for(int i = 0; i< textures.size(); i++){
            sf::Sprite sprite(textures[i]);
            sprites.push_back(sprite);
        }
        window.create(sf::VideoMode(p.L*CELL_SIZE, p.W*CELL_SIZE), "Minesweeper");
    }
    MinesweeperGUI(){
        GameParams p = GameParams();
        GUIParams params = GUIParams(p.L, p.W, 40);
        for (auto path : TEXTURE_PATHS) {
            sf::Texture texture;
            texture.loadFromFile(path);
            textures.push_back(texture);
        }
        for(int i = 0; i< textures.size(); i++){
            sf::Sprite sprite(textures[i]);
            sprites.push_back(sprite);
        }
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

#endif