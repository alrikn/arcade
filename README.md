# Arcade

A modular arcade framework that lets you mix and match graphical backends and games at runtime through dynamic library loading. The core engine, graphical renderers, and games are completely decoupled — you can swap any component without recompiling anything else.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building & Compiling](#building--compiling)
- [Running the Project](#running-the-project)
- [Architecture](#architecture)
  - [Directory Layout](#directory-layout)
  - [Component Overview](#component-overview)
  - [Plugin Loading](#plugin-loading)
  - [Interfaces](#interfaces)
  - [Core Game Loop](#core-game-loop)
- [Adding a New Graphical Library](#adding-a-new-graphical-library)
- [Adding a New Game Library](#adding-a-new-game-library)

---

## Prerequisites

- **C++ compiler:** `clang++` (C++17 or later)
- **GNU Make**
- **ncurses** development headers — needed to build the bundled Ncurses display library:
  - Ubuntu/Debian: `sudo apt install libncurses-dev`
  - Arch: `sudo pacman -S ncurses`
  - macOS (Homebrew): `brew install ncurses`

---

## Building & Compiling

All build targets are handled by the root `Makefile`.

```bash
# Build the core executable and all libraries
make

# Build only the shared libraries (graphical + game)
make libs

# Remove compiled objects
make clean

# Full clean — removes the core binary too
make fclean

# Clean rebuild
make re
```

After a successful build you will have:

```
core                            # The main executable
lib/
  graphical_lib/
    libNcurses.so               # Ncurses terminal renderer
  game_lib/
    arcade_snake.so             # Snake game
```

Each library has its own `Makefile` inside its source directory and can be compiled independently:

```bash
# Build only the Ncurses display library
make -C src/graphical_libraries/Ncurses

# Build only the Snake game library
make -C src/game_libraries/snake_game
```

---

## Running the Project

```bash
./core
```

The program opens with the built-in menu. Use the following keys throughout the application:

| Key | Action |
|-----|--------|
| `W` / `↑` | Move selection / snake up |
| `S` / `↓` | Move selection / snake down |
| `A` / `←` | Move snake left |
| `D` / `→` | Move snake right |
| `Space` | Confirm selection in menu |
| `Q` / `Esc` | Quit / return to menu |

**Menu flow:**

1. The menu scans `./lib/game_lib/` and `./lib/graphical_lib/` at startup and lists all `.so` files it finds.
2. Use `W`/`S` to navigate, `Space` to select a game and a graphical library.
3. The selected game and renderer are loaded dynamically and the game starts immediately.
4. Press `Q` or `Esc` at any time to return to the menu, where you can pick a different combination.

---

## Architecture

### Directory Layout

```
arcade/
├── include/                  # Public headers — interfaces & abstract base classes
│   ├── IDisplayModule.hpp    # Display interface (all graphical libs implement this)
│   ├── IGameModule.hpp       # Game interface (all games implement this)
│   ├── ADisplayModule.hpp    # Abstract base: default getWidth / getHeight
│   ├── AGameModule.hpp       # Abstract base: score, elapsed time, gameover helpers
│   ├── Core.hpp              # Core engine declaration
│   ├── DLLoader.hpp          # Generic dynamic-library loader (template)
│   ├── Ncurses.hpp           # Ncurses display library header
│   ├── LibFoo.hpp            # Stub display library header
│   ├── MenuGame.hpp          # Built-in menu game header
│   └── SnakeGame.hpp         # Snake game header
├── src/
│   ├── Core.cpp              # Core engine implementation
│   ├── graphical_libraries/
│   │   ├── Ncurses/          # Terminal renderer (ncurses)
│   │   └── LibFoo/           # Minimal stub renderer
│   └── game_libraries/
│       ├── menu_game/        # Built-in menu (compiled into core)
│       └── snake_game/       # Snake (compiled as shared library)
├── lib/
│   ├── graphical_lib/        # Output directory for graphical .so files
│   └── game_lib/             # Output directory for game .so files
├── main.cpp                  # Entry point
└── Makefile
```

### Component Overview

```
┌─────────────────────────────────────────────┐
│                 Core Engine                  │
│  - Owns DLLoader<IDisplayModule>            │
│  - Owns DLLoader<IGameModule>               │
│  - Drives the main game loop                │
│  - Handles library hot-swapping             │
└────────────────┬────────────────────────────┘
                 │ injects display into game
        ┌────────┴────────┐
        ▼                 ▼
 ┌─────────────┐   ┌─────────────┐
 │  Graphical  │   │    Game     │
 │  Libraries  │   │  Libraries  │
 │ (*.so)      │   │ (*.so)      │
 ├─────────────┤   ├─────────────┤
 │  Ncurses    │   │   Snake     │
 │  LibFoo     │   │   Menu      │
 └─────────────┘   └─────────────┘
```

- **Core** — the only permanent binary. It loads everything else at runtime.
- **Graphical libraries** — each implements `IDisplayModule`. They handle all rendering and raw input.
- **Game libraries** — each implements `IGameModule`. They contain game logic and call the display module to draw.
- **Menu game** — a special game module compiled directly into the core binary (not a `.so`). It scans the `lib/` directories and lets the user pick a combination.

### Plugin Loading

Shared libraries are loaded and unloaded at runtime by the `DLLoader<T>` template (`include/DLLoader.hpp`).

```
dlopen(path)  →  dlsym("create")  →  create()  →  T* instance
                 dlsym("destroy")
```

When the user switches libraries, the core:

1. Calls `exit()` on the current game module (if any).
2. Calls `reset()` on the relevant `DLLoader`, which calls `destroy(instance)` then `dlclose()`.
3. Calls `setHandle(newPath)` to open the new `.so`.
4. Calls `getInstance()` which resolves `create` / `destroy` symbols and instantiates the new module.
5. Calls `load_display()` on the new game module to inject the current display module.

### Interfaces

#### `IDisplayModule` — graphical library contract

```cpp
// Lifecycle
virtual void init() = 0;
virtual void stop() = 0;
virtual const std::string &getName() const = 0;

// Rendering
virtual void draw()  = 0;   // Flush the current frame
virtual void clear() = 0;   // Erase the back buffer

// Drawing primitives
virtual void drawText(const std::string& text, int x, int y) = 0;
virtual void drawTile(ShapeType shape, Color color, int x, int y) = 0;

// Display dimensions
virtual int getWidth()  = 0;  // Always WIDTH  (60)
virtual int getHeight() = 0;  // Always HEIGHT (40)

// Input
virtual EventType pollEvents() = 0;
```

Shared enums defined in `IDisplayModule.hpp`:

| Enum | Values |
|------|--------|
| `ShapeType` | `EMPTY`, `CIRCLE`, `SQUARE` |
| `EventType` | `OTHER`, `W_KEY`, `A_KEY`, `S_KEY`, `D_KEY`, `SPACE_KEY`, `MOUSE_L`, `MOUSE_R`, `MENU`, `QUIT` |
| `Color` | `BLACK`, `RED`, `GREEN`, `BLUE`, `YELLOW`, `MAGENTA`, `CYAN`, `WHITE` |

Constants: `WIDTH = 60`, `HEIGHT = 40`.

#### `IGameModule` — game library contract

```cpp
virtual const std::string &getName() const = 0;

// Called by core to give the game access to the display
virtual void load_display(IDisplayModule* display) = 0;

// Called every tick with the latest input event
virtual void tick(EventType input) = 0;

// Called before the library is unloaded
virtual void exit() = 0;

// Tick interval control (milliseconds)
virtual unsigned long get_elapsed() = 0;
virtual void set_elapsed(unsigned long elapsed) = 0;

// Score
virtual unsigned int get_score() = 0;
virtual void set_score(unsigned int score) = 0;

// Game-over flag (core returns to menu when true)
virtual bool get_gameover() = 0;
```

#### Abstract base classes

- **`ADisplayModule`** — inherits `IDisplayModule`, provides `getWidth()` and `getHeight()` backed by `_width` / `_height` constants. Recommended starting point for new graphical libraries.
- **`AGameModule`** — inherits `IGameModule`, provides default implementations for score, elapsed time, gameover flag, and `load_display()` (which also calls `display->init()`). Recommended starting point for new games.

### Core Game Loop

```
Core::run()
│
├── load graphical module (DLLoader)
├── load game module (DLLoader)
├── inject display into game (load_display)
│
└── loop (while _running)
    ├── update_event()          — pollEvents() from display
    ├── if QUIT  → stop loop
    ├── if MENU  → show built-in menu
    ├── else     → game_module->tick(event)
    ├── if game_module->get_gameover() → return to menu
    ├── sleep until next tick   — based on get_elapsed()
    └── display->draw()         — flush frame
```

---

## Adding a New Graphical Library

1. **Create a source directory:**

   ```
   src/graphical_libraries/MyLib/
   ├── MyLib.cpp
   └── Makefile
   ```

2. **Add a header** in `include/MyLib.hpp`:

   ```cpp
   #pragma once
   #include "ADisplayModule.hpp"

   class MyLib : public ADisplayModule {
   public:
       void init()   override;
       void stop()   override;
       const std::string &getName() const override;
       void draw()   override;
       void clear()  override;
       EventType pollEvents() override;
       void drawText(const std::string& text, int x, int y) override;
       void drawTile(ShapeType shape, Color color, int x, int y) override;
   private:
       const std::string _name = "MyLib";
   };
   ```

   > Inheriting from `ADisplayModule` gives you `getWidth()` and `getHeight()` for free. You can also inherit directly from `IDisplayModule` if you need custom dimensions.

3. **Implement the interface** in `MyLib.cpp`, then **export the C interface** — this is mandatory so that `DLLoader` can instantiate your library:

   ```cpp
   #include "MyLib.hpp"

   // ... method implementations ...

   extern "C" {
       IDisplayModule* create()                  { return new MyLib(); }
       void destroy(IDisplayModule* instance)    { instance->stop(); delete instance; }
   }
   ```

4. **Write a `Makefile`** following the same pattern as `src/graphical_libraries/Ncurses/Makefile`:

   ```makefile
   NAME = libMyLib.so
   SRC  = MyLib.cpp

   CXX      = clang++
   CXXFLAGS = -Wall -Wextra -g -fPIC -I../../../include
   LDFLAGS  = -shared   # add -lYourLib if you have extra link dependencies

   OUTPUT_DIR = ../../../lib/graphical_lib

   all: $(OUTPUT_DIR)/$(NAME)

   $(OUTPUT_DIR)/$(NAME): $(SRC) | $(OUTPUT_DIR)
   	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

   $(OUTPUT_DIR):
   	mkdir -p $(OUTPUT_DIR)

   clean:  ; rm -f $(OUTPUT_DIR)/$(NAME)
   fclean: clean
   re:     fclean all
   .PHONY: all clean fclean re
   ```

5. **Register the directory** in the root `Makefile` so that `make all` / `make libs` picks it up:

   ```makefile
   GRAPHICAL_DIRS = \
       src/graphical_libraries/Ncurses \
       src/graphical_libraries/MyLib   # ← add this line
   ```

6. **Build and test:**

   ```bash
   make
   ./core
   # Your library will appear in the graphical library list in the menu
   ```

---

## Adding a New Game Library

1. **Create a source directory:**

   ```
   src/game_libraries/my_game/
   ├── MyGame.cpp
   └── Makefile
   ```

2. **Add a header** in `include/MyGame.hpp`:

   ```cpp
   #pragma once
   #include "AGameModule.hpp"

   class MyGame : public AGameModule {
   public:
       MyGame();
       const std::string &getName() const override;
       void tick(EventType input)  override;
       void exit()                 override;
   private:
       const std::string _name = "MyGame";
   };
   ```

   > Inheriting from `AGameModule` provides `load_display()` (which stores the display pointer and calls `init()`), as well as default score, elapsed, and gameover accessors.

3. **Implement the interface** in `MyGame.cpp`, then **export the C interface**:

   ```cpp
   #include "MyGame.hpp"

   MyGame::MyGame() { _elapsed = 50; /* tick every 50 ms */ }

   const std::string &MyGame::getName() const { return _name; }

   void MyGame::tick(EventType input)
   {
       _display->clear();
       // game logic here — call _display->drawTile() / _display->drawText()
   }

   void MyGame::exit() { /* cleanup */ }

   extern "C" {
       IGameModule* create()                { return new MyGame(); }
       void destroy(IGameModule* instance)  { instance->exit(); delete instance; }
   }
   ```

   Key points for `tick()`:
   - `_display` is already set before `tick()` is first called (done by `load_display()`).
   - Call `_display->clear()` at the start of each tick, then redraw everything.
   - Set `_gameover = true` when the game should end; the core will return to the menu automatically.
   - The coordinate origin `(0, 0)` is the top-left corner of the game area. Valid range: `x ∈ [0, WIDTH)`, `y ∈ [0, HEIGHT)`.

4. **Write a `Makefile`** following the same pattern as `src/game_libraries/snake_game/Makefile`:

   ```makefile
   NAME = arcade_my_game.so
   SRC  = MyGame.cpp

   CXX      = clang++
   CXXFLAGS = -Wall -Wextra -g -fPIC -I../../../include
   LDFLAGS  = -shared

   OUTPUT_DIR = ../../../lib/game_lib

   all: $(OUTPUT_DIR)/$(NAME)

   $(OUTPUT_DIR)/$(NAME): $(SRC) | $(OUTPUT_DIR)
   	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

   $(OUTPUT_DIR):
   	mkdir -p $(OUTPUT_DIR)

   clean:  ; rm -f $(OUTPUT_DIR)/$(NAME)
   fclean: clean
   re:     fclean all
   .PHONY: all clean fclean re
   ```

5. **Register the directory** in the root `Makefile`:

   ```makefile
   GAME_DIRS = \
       src/game_libraries/snake_game \
       src/game_libraries/my_game    # ← add this line
   ```

6. **Build and test:**

   ```bash
   make
   ./core
   # Your game will appear in the game list in the menu
   ```
