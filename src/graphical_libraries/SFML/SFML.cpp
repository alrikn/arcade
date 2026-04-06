/*
** EPITECH PROJECT, 2026
** bootstrap_arcade
** File description:
** SFML
*/

#include "IDisplayModule.hpp"
#include "SFML_lib.hpp"

static sf::Color toSfColor(Color color)
{
    switch (color) {
        case RED:     return sf::Color::Red;
        case GREEN:   return sf::Color::Green;
        case BLUE:    return sf::Color::Blue;
        case YELLOW:  return sf::Color::Yellow;
        case MAGENTA: return sf::Color::Magenta;
        case CYAN:    return sf::Color::Cyan;
        case WHITE:   return sf::Color::White;
        case BLACK:
        default:      return sf::Color::Black;
    }
}

bool SFML_lib::isInsideGameArea(int x, int y) const
{
    return x >= 0 && y >= 0
        && x < static_cast<int>(_width)
        && y < static_cast<int>(_height);
}

sf::Vector2f SFML_lib::tileToPixel(int x, int y) const
{
    return {
        static_cast<float>(_originX + x * static_cast<int>(_tileSize)),
        static_cast<float>(_originY + y * static_cast<int>(_tileSize))
    };
}

void SFML_lib::updateLayout()
{
    const sf::Vector2u winSize = _window.getSize();
    const int boxW = static_cast<int>(_width * _tileSize);
    const int boxH = static_cast<int>(_height * _tileSize);

    _originX = (static_cast<int>(winSize.x) - boxW) / 2;
    _originY = (static_cast<int>(winSize.y) - boxH) / 2;

    _frameBorder.setPosition(static_cast<float>(_originX), static_cast<float>(_originY));
    _frameBorder.setSize(sf::Vector2f(static_cast<float>(boxW), static_cast<float>(boxH)));

}

SFML_lib::SFML_lib()
{
    //the window must only be initialised in init
    //we might howvere still use this func to load some resources


}

void SFML_lib::init()
{
    //we will create the window and set up any necessary resources
    const unsigned int winW = _width * _tileSize + 2 * _tileSize;
    const unsigned int winH = _height * _tileSize + 2 * _tileSize;

    _window.create(sf::VideoMode(winW, winH), "Arcade - SFML");
    _window.setFramerateLimit(60);

    if (!_font.loadFromFile("assets/Xolonium-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }


    _frameBorder.setFillColor(sf::Color::Transparent);
    _frameBorder.setOutlineThickness(3.f);
    _frameBorder.setOutlineColor(sf::Color(255, 255, 255, 180));

    updateLayout(); //we only do it once at the start, because SFML automatically maintains the aspect ratio of the window
    // so we don't need to update the layout on resize
}

void SFML_lib::draw()
{
    if (!_window.isOpen())
        return;
    _window.draw(_frameBorder);
    _window.display();
}

EventType SFML_lib::pollEvents()
{
   sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            return QUIT;

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    return W_KEY;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    return A_KEY;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    return S_KEY;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    return D_KEY;
                case sf::Keyboard::Space:
                    return SPACE_KEY;
                case sf::Keyboard::Escape:
                case sf::Keyboard::Q:
                    return QUIT;
                case sf::Keyboard::M:
                    return MENU;
                case sf::Keyboard::Enter:
                    return ENTER;
                case sf::Keyboard::Num1:
                    return NUM_1;
                case sf::Keyboard::Num2:
                    return NUM_2;
                case sf::Keyboard::Num3:
                    return NUM_3;
                case sf::Keyboard::Num4:
                    return NUM_4;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left)
                return MOUSE_L;
            if (event.mouseButton.button == sf::Mouse::Right)
                return MOUSE_R;
        }
    }

    return OTHER;
}

void SFML_lib::stop()
{
    //we will close the window and free any resources
    if (_window.isOpen())
        _window.close();

}

void SFML_lib::clear()
{
    _window.clear(sf::Color::Black);
}

void SFML_lib::drawTile(ShapeType shape, Color color, int x, int y)
{
    if (!_window.isOpen())
        return;
    const sf::Color sfColor = toSfColor(color);
    const int px = _originX + x * static_cast<int>(_tileSize);
    const int py = _originY + y * static_cast<int>(_tileSize);

    switch (shape) {
        case CIRCLE: {
            sf::CircleShape circle(_tileSize / 2.f);
            circle.setFillColor(sfColor);
            circle.setPosition(px, py);
            _window.draw(circle);
            break;
        }
        case SQUARE: {
            sf::RectangleShape rect(sf::Vector2f(_tileSize, _tileSize));
            rect.setFillColor(sfColor);
            rect.setPosition(px, py);
            _window.draw(rect);
            break;
        }
        case EMPTY:
        default:
            //we won't draw anything for empty tiles
            break;
    }
}

void SFML_lib::drawText(const std::string &text, Color color, int x, int y)
{
    if (!_window.isOpen())
        return;

    sf::Text sfText;
    sfText.setFont(_font);
    sfText.setString(text);
    sfText.setCharacterSize(18);
    sfText.setFillColor(toSfColor(color));
    sfText.setPosition(
        static_cast<float>(_originX + x * static_cast<int>(_tileSize)),
        static_cast<float>(_originY + y * static_cast<int>(_tileSize))
    );

    _window.draw(sfText);
}

void SFML_lib::drawSprite(const Sprite &sprite, int x, int y)
{
    if (!_window.isOpen()) {
        return;
    }

    const std::string fullPath = "assets/" + sprite.path;

    if (_failedTextures.count(fullPath)) {
        drawTile(sprite.fallback, sprite.fallbackColor, x, y);
        return;
    }

    if (_textures.find(fullPath) == _textures.end()) {
        sf::Texture &stored = _textures[fullPath];
        if (!stored.loadFromFile(fullPath)) {
            std::cerr << "Failed to load sprite: " << fullPath << std::endl;
            _textures.erase(fullPath);
            _failedTextures.insert(fullPath);
            drawTile(sprite.fallback, sprite.fallbackColor, x, y);
            return;
        }
    }

    const sf::Texture &tex = _textures.at(fullPath);
    sf::Sprite sfSprite(tex);

    // If src is 0 it loads the full image
    const float srcW = sprite.srcW > 0 ? sprite.srcW : static_cast<float>(tex.getSize().x);
    const float srcH = sprite.srcH > 0 ? sprite.srcH : static_cast<float>(tex.getSize().y);

    if (sprite.srcW > 0 && sprite.srcH > 0) {
        sfSprite.setTextureRect(sf::IntRect(sprite.srcX, sprite.srcY, sprite.srcW, sprite.srcH));
    }

    sfSprite.setScale(
        static_cast<float>(_tileSize) / srcW,
        static_cast<float>(_tileSize) / srcH
    );
    sfSprite.setPosition(
        static_cast<float>(_originX + x * static_cast<int>(_tileSize)),
        static_cast<float>(_originY + y * static_cast<int>(_tileSize))
    );
    _window.draw(sfSprite);
}

char SFML_lib::getInputChar()
{
    //we'll use sf::Event::TextEntered
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128 && event.text.unicode >= 32) {
                return static_cast<char>(event.text.unicode);
            }
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter ||
                event.key.code == sf::Keyboard::Escape) {
                return '\n';
            }
        }
    }
    return '\0';
}

//C interface (THIS is what dlopen/dlsym uses)
extern "C" {

IDisplayModule* create()
{
    return new SFML_lib();
}

void destroy(IDisplayModule* instance)
{
    instance->stop(); //we call stop before deleting the instance to make sure we end ncurses mode properly
    delete instance;
}

}
