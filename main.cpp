#include <SFML/Graphics.hpp>

//Player's variables
int coins = 0;
bool isLevelComplete = false;

//Maze Configuration
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

int maze[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,3,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,3,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,0,0,3,0,0,1,0,0,0,0,0,0,0,1,0,0,0,4,1},
    {1,3,0,2,0,0,1,0,0,0,0,0,0,0,1,3,0,1,1,1},
    {1,0,0,0,0,3,1,1,1,1,0,0,0,0,1,0,2,1,0,1},
    {1,0,3,2,0,0,0,2,2,1,0,0,0,0,1,3,0,1,0,1},
    {1,0,0,0,0,3,0,2,2,1,0,0,0,0,1,0,0,1,0,1},
    {1,3,0,3,0,0,0,2,2,1,0,0,0,0,1,3,0,1,0,1},
    {1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1},
    {1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
//Map Legend:
//0 - Free Space
//1 - Wall
//2 - Coin
//3 - Trap
//4 - Flag

//Function that checks wall's collision with player
bool checkWallCollision(sf::CircleShape& player, int maze[MAP_HEIGHT] [MAP_WIDTH], sf::RectangleShape& wallTemplate) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            if (maze[i][j] == 1) {
                wallTemplate.setPosition(j * 40.f, i * 40.f);

                if (player.getGlobalBounds().intersects(wallTemplate.getGlobalBounds())) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    // Creates Window with 800 by 600 size
    sf::RenderWindow window(sf::VideoMode(800, 600), "Adventure Creator");
    window.setFramerateLimit(60);

    //Preload Fonts and Stuff
    sf::Font font;
    if (!font.loadFromFile("lato-latin.ttf")) {
        font.loadFromFile("lato-latin.ttf");
    }

    //Creates the player
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Blue);
    ball.setPosition(60.f, 60.f);

    //Creates the wall
    sf::RectangleShape wall(sf::Vector2f(40.f, 40.f));
    wall.setFillColor(sf::Color::Blue);

    //Creates the coin
    sf::CircleShape coin(7.f);
    coin.setFillColor(sf::Color::Yellow);

    //Creates the trap
    sf::RectangleShape trap(sf::Vector2f(25.f, 25.f));
    trap.setFillColor(sf::Color::Red);

    //Creates the flag
    sf::RectangleShape flag(sf::Vector2f(20.f, 30.f));
    flag.setFillColor(sf::Color::Magenta);


    //Creates the GUI
    sf::Text coinsText;
    coinsText.setFont(font);
    coinsText.setCharacterSize(24);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setPosition(20.f, 10.f);

    sf::Text passText;
    passText.setFont(font);
    passText.setCharacterSize(50);
    passText.setString("Captured the Flag!");
    passText.setFillColor(sf::Color::White);
    passText.setPosition(200.f, 250.f);


    //Main cycle
    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2 pos = ball.getPosition();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && pos.x < 780.f) {
                ball.move(2.5f, 0.f);
                if (checkWallCollision(ball, maze, wall)) {
                    ball.move(-2.5f, 0.f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && pos.x > 0.f) {
                ball.move(-2.5f, 0.f);
                if (checkWallCollision(ball, maze, wall)) {
                    ball.move(2.5f, 0.f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && pos.y > 0.f) {
                ball.move(0.f, -2.5f);
                if (checkWallCollision(ball, maze, wall)) {
                    ball.move(0.f, 2.5f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && pos.y < 580.f) {
                ball.move(0.f, 2.5f);
                if (checkWallCollision(ball, maze, wall)) {
                    ball.move(0.f, -2.5f);
                }
            }
        }


        window.clear(sf::Color::Black);

        //Add the code between clear and display if needed

        window.draw(ball); //Draws Ball

        //Draw Maze
        for (int i = 0; i<MAP_HEIGHT; ++i) {
            for (int j = 0; j<MAP_WIDTH; ++j) {
                if (maze[i][j] == 1) {
                    wall.setPosition(j * 40.f, i * 40.f);
                    window.draw(wall);
                }
                else if (maze[i][j] == 2) {
                    coin.setPosition(j * 40.f, i * 40.f);
                    if (ball.getGlobalBounds().intersects(coin.getGlobalBounds())) {
                        maze[i][j] = 0;
                        ++coins;
                    }
                    else {
                        window.draw(coin);
                    }
                }
                else if (maze[i][j] == 3) {
                    trap.setPosition(j * 40.f, i * 40.f);
                    if (ball.getGlobalBounds().intersects(trap.getGlobalBounds())) {
                        ball.setPosition(60.f, 60.f);
                    }
                    else {
                        window.draw(trap);
                    }
                }
                else if (maze[i][j] == 4) {
                    flag.setPosition(j * 40.f + 10.f, i * 40.f + 5.f);
                    if (ball.getGlobalBounds().intersects(flag.getGlobalBounds())) {
                        isLevelComplete = true;
                    }
                    else {
                        window.draw(flag);
                    }
                }
            }
        }

        //Draw GUI
        coinsText.setString("Coins: " + std::to_string(coins));
        window.draw(coinsText);

        if (isLevelComplete) {
            window.draw(passText);
        }

        window.display();

    }
    return 0;
}