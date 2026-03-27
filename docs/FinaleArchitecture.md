# here is the architecture we are sharing with the other group

- group_1: alrik nico jad
- group_2: jose, sofia, daniel


the code of the Igame

```C++
class IGameModule
{
    protected:
    public:
        virtual ~IGameModule() = default;
        virtual const std::string &getName() const = 0;

        //a game needs to get events from the display.
        virtual void load_display(IDisplayModule* display) = 0; //this is how the game gets the display module, it can then use this to get events and draw things

        //I don't yet know where the game exit logic will be stored, but if it needs to be handled by the game, we'll use loop, and if its handled by the core, we'll use tick.
        virtual void tick(EventType input) = 0; //this is where the game logic goes, it will be called every frame by the main loop
        //loop is actually handled by the core
        virtual void exit() = 0; //when the core needs the game to exit, it will call this function, and the game can do any cleanup it needs to do here before the core unloads the library

        virtual unsigned long get_elapsed() = 0; //get how much time is supposed to happen between ticks.
        virtual void set_elapsed(unsigned long elapsed) = 0; //set how much time is supposed to happen between ticks.

        virtual unsigned int get_score() = 0; //get the current score of the game
        virtual void set_score(unsigned int score) = 0; //set the current score of the game

        virtual unsigned int get_highscore() = 0; //get the current highscore of the game
        virtual void set_highscore(unsigned int highscore) = 0; //set the current

        virtual bool get_gameover() = 0; //get whether the game is over or not
};
```

code of the Idisplay

```C++
class IDisplayModule {
    public:
        virtual ~IDisplayModule() = default;
        virtual void init() = 0;
        virtual void stop() = 0;
        virtual const std::string &getName() const = 0;
        virtual void draw() = 0; //draws the current state of the game
        virtual void clear() = 0; //clears the screen (for the next draw)

        virtual EventType pollEvents() = 0; //polls for events (keyboard, mouse, etc.)
        //however, the pollEvents is not enough, we also need a way to get the events that were polled
        // we can do this by having a getEvents function that returns a vector of events that were polled.


        //drawing functions (these are just examples, we can add more as needed)
        virtual void drawText(const std::string& text, Color color, int x, int y) = 0; //draws text at the given position

        //we need to keep in mind that while sfml can take a sprite object, ncurses can only draw characters
        //we could have a simple sprite class that can be used by both libraries, and the drawSprite function can take care of the differences between the two libraries
        virtual void drawTile(ShapeType shape, Color color, int x, int y) = 0; //draws a space at the given position (for clearing)
        virtual void drawSprite(const Sprite &sprite, int x, int y) = 0;

        //get width and height of display
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;

        virtual char getInputChar() = 0; //get the last input character (for text input)

};
```
