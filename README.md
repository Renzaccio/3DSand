# 3DSand
![Sand accumulating into a pile](images/sandpile.png?raw=true "Title")

### Installation
- Clone the repo using `git clone https://github.com/Renzaccio/3DSand.git`.
  - Alternatively, you can download and extract the ZIP file.
- Go to the project root directory
  - It should be named `3DSand` and contains the `main.cpp` file.
- Compile the programe using `g++ -std=c++20 -I./include -L./lib  main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -O2`.
  - For this, you'll need an updated `g++` compiler.
- Launch the executable.
  - It should be named `a.exe` by default.

### Controls
They are all indicated on window when the program is running.
| ACTION                     | AZERTY        | QWERTY        |
| :-                         | :-:           | :-:           |
| Toggle Simulation          | `Space`       | `Space`       |
| Toggle Pouring             | `R`           | `R`           |
| Camera - Toggle Laid Back  | `F`           | `F`           |
| Camera - Lateral Moves     | `ZQSD`        | `WASD`        |
| Camera - Vertical Moves    | `E`/`A`       | `E`/`Q`       |
| Camera - Rotation          | Arrow Keys    | Arrow Keys    |
| Camera - Fast Mode         | Hold `Shift`  | Hold `Shift`  |
| Camera - Slow Mode         | Hold `Ctrl`   | Hold `Ctrl`   |
| Place Blocks               | `LMB`         | `LMB`         |
