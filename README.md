# Minesweeper
## Interactive Minesweeper

A C++ implementation of the classic [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)) game, using [SFML](https://www.sfml-dev.org/) for rendering and user interaction. This project includes a fully functional game engine and an interactive graphical interface.\
My motivation to build this was that I was curious to try out the process of building an engine to solve Minesweeper optimally. 

---

## Features

- **Fully Functional Minesweeper Logic**: 16x30 game board with 99 mines
- **Game Dynamics**: Dynamic updates of game board when you flag or reveals cells. Win by correctly flagging all mines.
- **Graphical User Interface**: To visualize the game board and flag or reveal cells through Mouse Clicks. 
- **Helper Engine**: Option to set engine to play deterministic moves quickly. Developing more functionality

## Installation and Setup

### Prerequisites
1. Install [SFML](https://www.sfml-dev.org/).
   - Follow the [SFML setup guide](https://www.sfml-dev.org/tutorials/) for your compiler or IDE.
   - For MacOS, homebrew installation is the easiest way.
2. Ensure C++14 is supported by your compiler.

### Project Structure
- **`assets/`**: Directory for texture files used for the game board.
- **`src/core/game.cpp`**: Contains all the logic and rendering code.
- **`src/gui/gui.cpp`**: Code that implements interactive GUI.
- **`src/engine/engine.cpp`**:
  - Basic Engine Implementation that makes deterministic moves. Will add hint option to GUI later.
  - In process: Adding more deterministic moves from modeling revealed board as set of constraints on cells.
  - In process: (Exponential-time) solver that calculates probabilities for each cell. 
  - Future: Compare and test various heuristic-based AI Solvers.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/4dnaanM/Minesweeper.git
   cd Minesweeper
   ```
2. If g++ is your preferred C++ compiler, use the provided makefile to compile and run: 
   ```bash
   make build
   make run
   ```
   Else, follow step 3 and 4: 
3. Compile the program using your preferred C++ compiler. Example for g++:
    ```bash
    g++ -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system src/core/game.cpp src/gui/gui.cpp -o bin/minesweeper
    ```
    make sure to include and link the correct files as shown above. If all goes well, you should have an executable in the bin folder.
4. Run the executable: This will allow you to use the GUI and play the game. Engine is not enabled by default.
    ```bash
    ./bin/minesweeper
    ```

## Gameplay Instructions
1. Launch the game to see the Minesweeper board rendered in a window.
2. Controls:
- **Left-click**: Reveal a cell.
- **Right-click**: Flag or unflag a hidden cell, or reveal all hidden cells neighbouring a satisfied revealed cell (see: [chording](https://minesweeper.fandom.com/wiki/Chording)). 

The game ends if:
- All mines are flagged correctly (win).
- A mine is revealed or 99 flags are placed, atleast one of which is wrong (loss).

## Asset Details
- The game uses sprite textures for visual representation:
    - Hidden cells
    - Flagged cells
    - Revealed cells with numbers (1-8)
    - Mines
- Place all required files in the ```assets``` folder.
```bash
assets/
├── 0.svg.png       # Empty revealed cell
├── 1.svg.png       # Cell with number 1
├── 2.svg.png       # Cell with number 2
├── ...
├── Flag.svg.png    # Flagged cell
├── Mine.svg.png    # Mine
└── Hidden.svg.png  # Hidden cell
```
## Known Issues and Future Improvements
### To-Do:
- Ensure the first click is never on a mine.
- Implement "No Guessing" mode.
- Add a Game-Over Screen, Timer, Remaining Mines Counter, Re-start button, Hint button.
- tarball package instead of source code distribution
### Known Issues:
- Window refuses to close. (End the terminal process using Ctrl+C)
## Contributions
Contributions are welcome! Feel free to open issues or submit pull requests to improve the project.
## License
This project is open-source. You are free to use, modify, and distribute it under the terms of the [MIT License](https://opensource.org/license/mit).
## Acknowledgments
- SFML for its simplicity and power in handling graphics and input.
- Classic Minesweeper for the inspiration
- Wikipedia Commons, for the Assets
