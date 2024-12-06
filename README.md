# Minesweeper
## Interactive Minesweeper

A C++ implementation of the classic Minesweeper game, using [SFML](https://www.sfml-dev.org/) for rendering and user interaction. This project includes a fully functional game engine and an interactive graphical interface.

---

## Features

- **Fully Functional Minesweeper Logic**:
  - Automatic generation of a 16x30 game board with 99 mines placed randomly.
  - User board with states: Hidden, Flagged, and Revealed.
  - Supports flagging cells and revealing multiple cells.

- **Graphical User Interface**:
  - Renders the board using `SFML` sprites.
  - Mouse-based interaction to reveal or flag cells.

- **Game Dynamics**:
  - Win by correctly flagging all mines.
  - Lose by clicking on a mine.
  - Game updates dynamically based on user inputs.

## Installation and Setup

### Prerequisites
1. Install [SFML](https://www.sfml-dev.org/).
   - Follow the [SFML setup guide](https://www.sfml-dev.org/tutorials/) for your compiler or IDE.
   - For MacOS, homebrew installation is the easiest way.
2. Ensure C++14 is supported by your compiler.

### Project Structure
- **`game.cpp`**: Contains all the logic and rendering code.
- **`assets/`**: Directory for texture files used for the game board.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/4dnaanM/Minesweeper.git
   cd Minesweeper
   ```
2. If g++ is set up on your device, use the provided makefile to compile and run: 
   ```bash
   make build
   make run
   ```
   Else, follow step 3 and 4: 
2. Compile the program using your preferred C++ compiler. Example for g++:
    ```bash
    g++ -std=c++14 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system game.cpp -o game
    ```
    make sure to include and link the correct files.
3. Run the executable: 
    ```bash
    ./minesweeper
    ```

## Gameplay Instructions
1. Launch the game to see the Minesweeper board rendered in a window.
2. Controls:
- **Left-click**: Reveal a cell.
- **Right-click**: Flag or unflag a hidden cell, or reveal all hidden cells neighbouring a satisfied revealed cell. 

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
- Add an AI solver for the game.
- Implement a "No Guessing" mode to make the game solvable deterministically.
- Add a Game-Over Screen, Timer, Remaining Mines Counter, Re-start button, Hint button.
- tarball package instead of source code
### Known Issues:
- Relative paths to the assets/ directory may cause issues if the working directory is incorrect.
- Window scaling does not work. 
- Window sometimes refuses to close. (End the terminal process using Ctrl+C)
## Contributions
Contributions are welcome! Feel free to open issues or submit pull requests to improve the project.
## License
This project is open-source. You are free to use, modify, and distribute it under the terms of the [MIT License](https://opensource.org/license/mit).
## Acknowledgments
- SFML for its simplicity and power in handling graphics and input.
- Classic Minesweeper for the inspiration
- Wikipedia Commons, for the Assets
- ChatGPT for writing this Readme ;)  