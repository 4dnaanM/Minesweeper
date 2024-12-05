#include <SFML/Graphics.hpp>
#include <iostream>

int main() {

    const int W = 16;
    const int L = 30;
    const int CELL_SIZE = 40;
    const int WIDTH = L * CELL_SIZE;
    const int HEIGHT = W * CELL_SIZE;

    std::vector<std::string> texturePaths = {
        "../assets/Hidden.svg.png",  // 0: Hidden cell
        "../assets/Flag.svg.png",    // 1: Flagged cell
        "../assets/Mine.svg.png",    // 2: Mine
        "../assets/0.svg.png",       // 3: Revealed empty cell
        "../assets/1.svg.png",       // 4: Number 1
        "../assets/2.svg.png",       // 5: Number 2
        "../assets/3.svg.png",       // 6: Number 3
        "../assets/4.svg.png",       // 7: Number 4
        "../assets/5.svg.png",       // 8: Number 5
        "../assets/6.svg.png",       // 9: Number 6
        "../assets/7.svg.png",       // 10: Number 7
        "../assets/8.svg.png"        // 11: Number 8
    };
    std::vector<sf::Texture> textures; 
    std::vector<sf::Sprite> sprites; 

    for (auto path : texturePaths) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textures.push_back(texture);
    }

    for (size_t i = 0; i < textures.size(); ++i) {
        sf::Sprite sprite(textures[i]);
        sprites.push_back(sprite);
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

    while (window.isOpen()) {
        sf::Event event;
        while (window.waitEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x;  // X coordinate of the click
                int y = event.mouseButton.y;  // Y coordinate of the click

                // Convert coordinates to grid indices
                int col = x / CELL_SIZE;
                int row = y / CELL_SIZE;
                
                if (event.mouseButton.button == sf::Mouse::Left) {
                    std::cout << "Left button clicked at cell (" << row << ", " << col << ")" << std::endl;
                }

                if (event.mouseButton.button == sf::Mouse::Right) {
                    std::cout << "Right button clicked at cell (" << row << ", " << col << ")" << std::endl;
                }
            }

            window.clear(sf::Color::White);

            for (int y = 0; y < W; y++) {
                for (int x = 0; x < L; x++) {
                    int state = 0;
                    
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
        }
    }

    return 0;
}
