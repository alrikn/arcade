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
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so" &&
                fileName.rfind("arcade_", 0) == 0) {
                _gameLibs.push_back(fileName);
            }
        }
        closedir(dir);
    }

    //we loop through all files in lib/graphical_lib and put them in _graphicalLibs
    if ((dir = opendir("./lib/graphical_lib")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string fileName = ent->d_name;
            if (fileName.size() > 3 && fileName.substr(fileName.size() - 3) == ".so" &&
                fileName.rfind("arcade_", 0) == 0) {
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
        _display->drawText("Please enter your name:", WHITE, 0, 0);
        _display->drawText(player_name + "_", WHITE, 0, 1); //we add an underscore to indicate the cursor position
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
    if (highscore_json.contains(game_name) && highscore_json[game_name].contains(player_name)) {
        if (highscore > highscore_json[game_name][player_name]) {
            highscore_json[game_name][player_name] = highscore;
        }
    } else {
        highscore_json[game_name][player_name] = highscore;
    }
}

//draw box looks terrible in sfml, bcause we are just drawing text, and in sfml, no all cjhars are same width
void MenuGame::drawBox(int startX, int startY, int width, int height)
{
    if (width > WIDTH)
        width = WIDTH; //60
    if (height > HEIGHT)
        height = HEIGHT; //40
    std::string horizontal = "+" + std::string(width - 2, '-') + "+";

    // Top border
    _display->drawText(horizontal, WHITE, startX, startY);

    // Sides
    for (int i = 1; i < height - 1; i++) {
        _display->drawText("|" + std::string(width - 2, ' ') + "|", WHITE, startX, startY + i);
    }

    // Bottom border
    _display->drawText(horizontal, WHITE, startX, startY + height - 1);
}

void MenuGame::displayHighscores(int startX, int startY)
{
    //int boxWidth = 40;
    //int boxHeight = 10;

    //drawBox(startX, startY, boxWidth, boxHeight);


    // Title
    _display->drawText(" Highscores: ", WHITE, startX + 2, startY + 1);

    if (highscore_json.empty()) {
        _display->drawText(" No scores yet", WHITE, startX + 2, startY + 3);
        return;
    }

    int y = startY + 3;
    int rank = 1;
    //now we loop through the high score json and display the high score of the current user one evry game

    for (auto& [game_name, scores] : highscore_json.items()) {
        if (scores.contains(player_name)) {
            unsigned int score = scores[player_name];
            _display->drawText(std::to_string(rank) + ". " + game_name + ": " + std::to_string(score), WHITE, startX + 2, y++);
            rank++;
        }

        if (rank > 5) break; // limit display
    }
}


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
    if (input == SPACE_KEY) {
        if (global_index < (int)_gameLibs.size()) {
            selected_game_index = global_index;
            _gameSelectionPending = true;
        } else {
            selected_graphical_index = global_index - _gameLibs.size();
            _graphSelectionPending = true;
        }
        selected_option = true;
    }

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

    _display->drawText("Welcome to the Arcade!", WHITE, 0, y++);
    _display->drawText("Use W/S to navigate", WHITE, 0, y++);
    _display->drawText("Press SPACE to select", WHITE, 0, y++);
    _display->drawText("Press Q to quit", WHITE, 0, y++);
    y++; //for readability

    _display->drawText("Available Game Libraries:", WHITE, 0, y++);
    for (const auto& lib : _gameLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, WHITE, 2, y++);
        index++;
    }

    y++;

    _display->drawText("Available Graphical Libraries:", WHITE, 0, y++);
    for (const auto& lib : _graphicalLibs) {
        std::string prefix = (global_index == index) ? "> " : "  ";
        _display->drawText(prefix + lib, WHITE, 2, y++);
        index++;
    }

    displayHighscores(0, y + 1);
}

std::tuple<std::string, std::string> MenuGame::get_path_chosen()
{
    if (!selected_option)
        return {"", ""};

    std::string gameLibPath = "";
    std::string graphLibPath = "";

    if (_gameSelectionPending)
        gameLibPath = "./lib/game_lib/" + _gameLibs[selected_game_index];
    if (_graphSelectionPending)
        graphLibPath = "./lib/graphical_lib/" + _graphicalLibs[selected_graphical_index];

    selected_option = false;
    _gameSelectionPending = false;
    _graphSelectionPending = false;
    return {gameLibPath, graphLibPath};
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
