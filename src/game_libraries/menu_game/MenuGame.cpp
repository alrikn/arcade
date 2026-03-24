/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** MenuGame
*/

#include "MenuGame.hpp"
#include "IDisplayModule.hpp"
#include<dirent.h>

MenuGame::MenuGame()
{
    std::cout << "[" << _name << "] Constructor called" << std::endl;
    loadLibs();
}

const std::string &MenuGame::getName() const
{
    return _name;
}

void MenuGame::loadLibs()
{
    //we loop through all files in lib/game_lib and put them in _gameLibs
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./lib/game_lib")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so") {
                _gameLibs.push_back(fileName);
            }
        }
        closedir(dir);
    }

    //we loop through all files in lib/graphical_lib and put them in _graphicalLibs
    if ((dir = opendir("./lib/graphical_lib")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so") {
                _graphicalLibs.push_back(fileName);
            }
        }
        closedir(dir);
    }
}

void MenuGame::load_display(IDisplayModule* display)
{
    std::cout << "[" << _name << "] load_display called with display: " << display->getName() << std::endl;
    _display = display; //we store the display module so we can use it in the game logic
    _display->init();

    _height = _display->getHeight();
    _width = _display->getWidth();

    handle_name_input();

    //we should also check all of the .so files in the lib folder to be able to display them
}

void MenuGame::handle_name_input(void)
{
    while (!name_entered) {
        char ch = _display->getInputChar();
        if (ch == '\n') { //if the user presses enter, we consider the name input to be complete
            name_entered = true;
            break;
        }
        //if its not \0 we append
        if (ch != '\0') {
            player_name += ch;
        }

        //render the name input screen
        _display->clear();
        _display->drawText("Please enter your name:", 0, 0);
        _display->drawText(player_name + "_", 0, 1); //we add an underscore to indicate the cursor position
        _display->draw();
    }

    read_highscore_file();// read high score
}

//this file needs to be able to handle that there is no file
void MenuGame::read_highscore_file(void)
{
    std::ifstream file(highscore_file);

    if (!file.is_open()) {
        std::cout << "High score file not found, starting with empty high scores." << std::endl;
        return;
    }
    // Process the file
    try {
        file >> highscore_json;
    } catch (const std::exception& e) {
        std::cout << "Error reading high score file: " << e.what() << std::endl;
    }
}

void MenuGame::write_highscore_file(void)
{
    std::ofstream file(highscore_file);
    if (file.is_open()) {
        file << highscore_json.dump(4); //to make sure it prints nicely
        file.close();
    }
}

void MenuGame::update_highscore(std::string game_name, unsigned int highscore)
{
    if (highscore_json.contains(game_name)) {
        if (highscore > highscore_json[game_name][player_name]) {
            highscore_json[game_name][player_name] = highscore;
        }
    } else {
        highscore_json[game_name][player_name] = highscore;
    }
}

/*
** as a new feature, the menu needs to be able to display the high score of each game
** this also means that the high score needs to be stored in a file made by the core
** the high scores will be given initially by the core to the menu, and then on menu exit, we return them to menu so that it can write stuff
**
** OR (better solution)
** the menu can directly read and write the high score file,
** since it has access to the display module,
** it can also have access to the file system,
** and it can read and write the high score file directly,
** this way we don't need to pass the high scores back and forth between the core and the menu,
** and we can also update the high scores in real time while we're in the menu,
** so that if the user plays a game and gets a new high score,
** they can see it immediately in the menu without having to go back to the menu first.
*/
void MenuGame::tick(EventType input)
{

    if (!_display)
        return; //if we don't have a display module, we can't do anything, so we just return
    //input handle
    int maxIndex = _gameLibs.size() + _graphicalLibs.size() - 1;

    if (input == W_KEY && global_index > 0)
        global_index--;
    if (input == S_KEY && global_index < maxIndex)
        global_index++;
    if (input == SPACE_KEY)
        selected_option = true;

    //global index handle
    if (global_index < (int)_gameLibs.size()) {
        current_game_index = global_index;
    } else {
        current_graphical_index = global_index - _gameLibs.size();
    }

    //render handler
    _display->clear();

    int y = 0;
    int index = 0;

    _display->drawText("Welcome to the Arcade!", 0, y++);
    _display->drawText("Use W/S to navigate", 0, y++);
    _display->drawText("Press SPACE to select", 0, y++);
    _display->drawText("Press Q to quit", 0, y++);
    y++; //for readability

    _display->drawText("Available Game Libraries:", 0, y++);
    for (const auto& lib : _gameLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, 2, y++);
        index++;
    }

    y++;

    _display->drawText("Available Graphical Libraries:", 0, y++);
    for (const auto& lib : _graphicalLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, 2, y++);
        index++;
    }
}

std::tuple<std::string, std::string> MenuGame::get_path_chosen()
{
    if (selected_option) {
        std::string gameLibPath = "./lib/game_lib/" + _gameLibs[current_game_index];
        std::string graphLibPath = "./lib/graphical_lib/" + _graphicalLibs[current_graphical_index];
        selected_option = false; //reset for next time we go to the menu
        return {gameLibPath, graphLibPath};
    }
    return {"", ""}; //to signify that user has not selected shit
}

void MenuGame::exit()
{
    std::cout << "[" << _name << "] exit called" << std::endl;
}


extern "C" {

IGameModule* create()
{
    return new MenuGame();
}

void destroy(IGameModule* instance)
{
    delete instance;
}

}
