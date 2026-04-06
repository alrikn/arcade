# todo list for the arcade project


## Core

- [x] prompt the user for their name on initialisation (for the high score thing)
- [x] add a function that uploads high scores to a file (needs to have the game name, player name and score) (only upload it if its a higher score than current one, otherwise makes no sense)
- [x] add a function that retrieves high scores from a file (needs to have the game and returns the score) it won't care about the player name, i just wanna display high score.
- [ ] add a key map to restart the game
- [ ] add argument parsing to be able to specify graphical library.

## Menu
- [x] display the high score for each game in the menu
- [x] add a "high scores" section in the menu that displays the high scores for each game.
- [x] the menu should be able to switch games when 1 or 2 is being pressed.
- [x] the menu should be able to switch graphical libraries when 3 or 4 is being pressed.

## Games

### Snake
- [x] add a score system to snake (score increases by 1 for each food eaten)
- [x] add a high score system to snake (stores the highest score achieved in the game and displays it in the menu)

### Nibbler
- [ ] add a score system to nibbler

- [ ] whatever nico wants

### tetris
- [ ] add a score system to tetris
- [ ] whatever jad wants

## Graphical libraries

### Ncurses
- [x] implement the ncurses graphical library (for the menu and the games)

### SFML
- [ ] ensuring it works in the same way sfml does, by which i mean that inside the sfml window, there should be a background sprite, and then on that background sprite, we have the center as a black window (the game window that the game has acceess to).
  - [ ] when the game writes drawobject(x, y), we should adjust it so that it draws on the black game window, not the entire sfml window, so we need to adjust the coordinates accordingly.
  - [ ] make the background sprite look nice.
