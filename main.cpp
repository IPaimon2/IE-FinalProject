#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Constants
const int BLOCK_SIZE = 20;
const int WIDTH = 800;
const int HEIGHT = 600;
//const float MONSTER_SPEED = 0.1f;
const float SPAWN_DELAY = 1.0f; // Monster spawn speed
int buttonNumber;
#define Max_main_menu 2


class Main_menu
{
public:
    Main_menu(float width, float hight);


    void draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();

    int mainMenuPressed()
    {
        return mainMenuSelected;
    }

    ~Main_menu();


private:
    int mainMenuSelected;
    //int score; i didnt use it
    sf::Font font;//
    sf::Text mainMenu[Max_main_menu];// array of characters
};

Main_menu::Main_menu(float width, float height)
{
    if (!font.loadFromFile("arial.ttf")){}
    // Play
    mainMenu[0].setFont(font);
    mainMenu[0].setColor(sf::Color::Blue);
    mainMenu[0].setString("Play");
    mainMenu[0].setCharacterSize(50);
    mainMenu[0].setPosition(sf::Vector2f(width / 2 , height / (Max_main_menu + 1)));
    // EXIT
    mainMenu[1].setFont(font);
    mainMenu[1].setColor(sf::Color::White);
    mainMenu[1].setString("Exit");
    mainMenu[1].setCharacterSize(50);
    mainMenu[1].setPosition(sf::Vector2f(width / 2 , height / (Max_main_menu + 1) * 2));

    //  method for displying the SCORE


//    mainMenu[2].setFont(font);
//    mainMenu[2].setColor(sf::Color::Black);
//    mainMenu[2].setCharacterSize(30);
//    mainMenu[2].setPosition(sf::Vector2f(width / 2 , height / (Max_main_menu + 1) * 5));





    mainMenuSelected = 0;  // reset the  index from zero

}




Main_menu::~Main_menu()
{
}


// Function of drawing main menu
void Main_menu::draw(sf::RenderWindow &window)
{
    for (int i = 0; i <= 1; i++)
    {
        window.draw(mainMenu[i]);
    }
}

// Function of moving up
void Main_menu::moveUp()
{
    if (mainMenuSelected >= 1) // check if not on the first item (play)

    {

        mainMenu[mainMenuSelected].setColor(sf::Color::White);  // change the pervious item's color

        mainMenuSelected--;  // changes the counter to move to "play" option

        mainMenu[mainMenuSelected].setColor(sf::Color::Blue); // change the new item's color
    }
}

// Function of moving down
void Main_menu::moveDown()
{
    if (mainMenuSelected <= Max_main_menu) // check if not on the last item (exit)

    {
        mainMenu[mainMenuSelected].setColor(sf::Color::White);  // change the pervious item's color

        mainMenuSelected++;  // changes the counter to move to "exit" option

        mainMenu[mainMenuSelected].setColor(sf::Color::Blue);           // change the new item's color

    }
}

// Snake class
class Snake {
public:
    Snake(int startX, int startY) {
        head.setPosition(startX, startY);
        head.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        head.setFillColor(sf::Color::Blue );
        body.push_back(head);
    }

    void move(int dx, int dy) {
        sf::Vector2f position = head.getPosition();
        head.setPosition(position.x + dx * BLOCK_SIZE, position.y + dy * BLOCK_SIZE);

        // Move the body parts simntiously
        for (int i = body.size() - 1; i > 0; --i) {
            body[i].setPosition(body[i - 1].getPosition());
        }
        body[0].setPosition(head.getPosition());
    }

    void grow() {
        sf::RectangleShape bodyPart(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        bodyPart.setFillColor(sf::Color::Yellow );
        body.push_back(bodyPart);
    }

    void draw(sf::RenderWindow& window) {
        for (sf::RectangleShape& bodyPart : body) {
            window.draw(bodyPart);
        }
    }

    sf::FloatRect getBounds() {
        return head.getGlobalBounds();
    }

    void checkCollision(sf::FloatRect otherBounds) {
        if (head.getGlobalBounds().intersects(otherBounds)) {
            gameOver = true;
        }
    }

    bool isGameOver() {
        return gameOver; // getter check if the game is over if so then returns
    }

private:
    sf::RectangleShape head;
    std::vector<sf::RectangleShape> body;
    bool gameOver = false;
};

// Monster class
class Monster {
public:
    Monster(float startX, float startY) {
        head.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        head.setFillColor(sf::Color::Blue); // Head color is blue
        head.setPosition(startX, startY);

        body1.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        body1.setFillColor(sf::Color::Red); // Body color is red

        body2.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        body2.setFillColor(sf::Color::Red); // Body color is red

        // Generate a random direction for the monster
        int direction = rand() % 4; // 0: Up, 1: Down, 2: Left, 3: Right
        switch (direction) {
            case 0: // Up
                dx = 0;
                dy = -1;
                break;
            case 1: // Down
                dx = 0;
                dy = 1;
                break;
            case 2: // Left
                dx = -1;
                dy = 0;
                break;
            case 3: // Right
                dx = 1;
                dy = 0;
                break;
        }
    }

    void move() {
        sf::Vector2f headPosition = head.getPosition();
        sf::Vector2f newPosition(headPosition.x + dx *BLOCK_SIZE , headPosition.y + dy *BLOCK_SIZE);

        // Check if the new position is out of bounds
        if (newPosition.x < 0 || newPosition.x >= WIDTH || newPosition.y < 0 || newPosition.y >= HEIGHT) {
            // Wrap the monster to the opposite side of the window
            if (newPosition.x < 0)
                newPosition.x = WIDTH - BLOCK_SIZE;
            else if (newPosition.x >= WIDTH)
                newPosition.x = 0;
            else if (newPosition.y < 0)
                newPosition.y = HEIGHT - BLOCK_SIZE;
            else if (newPosition.y >= HEIGHT)
                newPosition.y = 0;
        }

        // Move the body parts
        body2.setPosition(body1.getPosition());
        body1.setPosition(headPosition);
        head.setPosition(newPosition);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(head);
        window.draw(body1);
        window.draw(body2);
    }

    sf::FloatRect getBounds() {
        return head.getGlobalBounds();
    }

    sf::Vector2f getPosition() {
        return head.getPosition();
    }

private:
//    sf::RectangleShape2 head;

    sf::RectangleShape head;
    sf::RectangleShape body1;
    sf::RectangleShape body2;
    int dx;
    int dy;
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Main window
    sf::RenderWindow mainWindow(sf::VideoMode(sf::VideoMode(WIDTH, HEIGHT)), "SNAKE GAME - Main Menu", sf::Style::Close);
    sf::Texture menuBackground;
    menuBackground.loadFromFile("menu.jpg");
    menuBackground.setSmooth(true);
    sf::Sprite menubg(menuBackground);
    menubg.setTextureRect({0, 0, WIDTH, HEIGHT});
    menubg.setScale(1,1);
    Main_menu menu(50, 370); // Placing it on the left of the screen idk why but it looks better on left.

    while (mainWindow.isOpen())
            {
                sf::Event event;
                while (mainWindow.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        mainWindow.close();
                    if (event.type == sf::Event::KeyReleased)
                    {
                        if (event.key.code == sf::Keyboard::Up)
                        {
                            menu.moveUp();
                            break;
                        }
                        if (event.key.code == sf::Keyboard::Down)
                        {
                            menu.moveDown();
                            break;
                        }

                        // choose button
                        if (event.key.code == sf::Keyboard::Return) // enter
                        {

                            if (menu.mainMenuPressed() == 0)
                            {
                                mainWindow.close();
                                buttonNumber = 0;
                            }
                            if (menu.mainMenuPressed() == 1)
                            {
                                mainWindow.close();
                                buttonNumber = 1;
                            }

                        }


                    }

                }
                mainWindow.clear();
                mainWindow.draw(menubg);
                menu.draw(mainWindow);
                mainWindow.display();


            }


    if (buttonNumber == 0)  // buttonNumber 0 - PLAY
            {

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");
    window.setFramerateLimit(10);

    Snake player(0, 0);

    std::vector<Monster> monsters;
    for (int i = 0; i < 5; ++i) {
        float startX = rand() % (WIDTH - BLOCK_SIZE * 3);
        float startY = rand() % (HEIGHT - BLOCK_SIZE);
        monsters.push_back(Monster(startX, startY));
    }

    sf::Clock clock;
    float elapsedTime = 0.0f;
    float growthDelay = 1.0f; // Snake growth delay in seconds
    float spawnTimer = 1.0f;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Player Controls
        int dx = 0, dy = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            dy = -1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            dy = 1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            dx = -1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            dx = 1;
        player.move(dx, dy);

        // Snake Growth
        elapsedTime += clock.restart().asSeconds();
        if (elapsedTime >= growthDelay) {
            player.grow();
            elapsedTime = 0.0f;
        }

        // Spawn Monsters
           spawnTimer += clock.restart().asSeconds();
           if (spawnTimer >= SPAWN_DELAY) {
               Monster newMonster(rand() % (WIDTH / BLOCK_SIZE) * BLOCK_SIZE, rand() % (HEIGHT / BLOCK_SIZE) * BLOCK_SIZE);// it spawns the snakes randomly
               monsters.push_back(newMonster);
               spawnTimer = 0.0f; // Reset the spawn timer
           }

        // Monster Movement
        for (Monster& monster : monsters) {
            monster.move();
            sf::Vector2f position = monster.getPosition();

            // Check if the monster hits the window boundaries if so then they will spawn in the oppsite side of the screen.
            if (position.x < 0 || position.x >= WIDTH || position.y < 0 || position.y >= HEIGHT) {
                // Wrap the monster to the opposite side of the window
                if (position.x < 0)
                    position.x = WIDTH - BLOCK_SIZE;
                else if (position.x >= WIDTH)
                    position.x = 0;
                else if (position.y < 0)
                    position.y = HEIGHT - BLOCK_SIZE;
                else if (position.y >= HEIGHT)
                    position.y = 0;
                monster.move();
            }

            player.checkCollision(monster.getBounds());
        }


        if (player.isGameOver()) {
            window.close();
            break;
        }

        // Render
        window.clear(sf::Color::Black);
        player.draw(window);
        for (Monster& monster : monsters) {
            monster.draw(window);
        }
        window.display();
         }
    }
    else
        mainWindow.close();

    return 0;
}
