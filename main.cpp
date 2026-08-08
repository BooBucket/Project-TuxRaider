#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


//Player's variables
int coins = 0;
int lives = 3;
bool isLevelComplete = false;
bool isWinTrackPlayed = false;

//Maze Configuration
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

int maze[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,3,0,0,0,3,0,0,0,2,3,0,0,0,1},
    {1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,3,0,1},
    {1,1,1,1,0,0,0,0,0,0,2,0,0,3,0,0,0,0,2,1},
    {1,0,0,1,0,0,0,0,0,0,3,0,0,0,2,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,2,0,0,0,3,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,3,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,1},
    {1,1,1,1,0,0,0,0,0,3,2,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,2,0,0,0,3,0,0,2,3,0,0,0,1},
    {1,0,0,0,0,0,3,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,0,3,2,0,0,0,0,1,1,0,1,1,0,0,0,3,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,4,0,1,0,0,2,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
//Map Legend:
//0 - Free Space
//1 - Wall
//2 - Coin
//3 - Trap
//4 - Flag

//Function that checks wall's collision with player
bool checkWallCollision(sf::Sprite& player, int maze[MAP_HEIGHT] [MAP_WIDTH], sf::Sprite& wallTemplate) {
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
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tux Raiders");
    window.setFramerateLimit(60);

    //Preload Assets
    sf::Font font;
    if (!font.loadFromFile("lato-latin.ttf")) {
        font.loadFromFile("lato-latin.ttf");
    }

    sf::SoundBuffer coinSBuffer;
    if (!coinSBuffer.loadFromFile("coin-collect.wav")) {
        return 0;
    }
    sf::Sound coinSound;
    coinSound.setBuffer(coinSBuffer);

    sf::SoundBuffer trapSBuffer;
    if (!trapSBuffer.loadFromFile("pop.wav")) {
        return 0;
    }
    sf::Sound trapSound;
    trapSound.setBuffer(trapSBuffer);

    sf::SoundBuffer victorySBuffer;
    if (!victorySBuffer.loadFromFile("victory.wav")) {
        return 0;
    }
    sf::Sound victorySound;
    victorySound.setBuffer(victorySBuffer);

    sf::Texture tuxRight, tuxLeft, tuxUp, tuxDown;
    tuxRight.loadFromFile("walk-right.png");
    tuxLeft.loadFromFile("walk-left.png");
    tuxUp.loadFromFile("walk-up.png");
    tuxDown.loadFromFile("walk-down.png");

    sf::Texture trapTexture;
    trapTexture.loadFromFile("mine.png");

    sf::Texture flagTexture;
    flagTexture.loadFromFile("flag.png");

    sf::Texture coinTexture;
    coinTexture.loadFromFile("coin.png");

    sf::Texture wallTexture;
    wallTexture.loadFromFile("wall.png");

    //Creates the player
    sf::Sprite tux;
    tux.setTexture(tuxDown);
    tux.setPosition(60.f, 270.f);

    //Animation Clock
    sf::Clock animationClock;
    int currentFrame = 0;
    //Creates the wall
    sf::Sprite wall;
    wall.setTexture(wallTexture);

    //Creates the coin
    sf::Sprite coin;
    coin.setTexture(coinTexture);

    //Creates the trap
    sf::Sprite trap;
    trap.setTexture(trapTexture);

    //Creates the flag
    sf::Sprite flag;
    flag.setTexture(flagTexture);


    //Creates the GUI
    sf::Text coinsText;
    coinsText.setFont(font);
    coinsText.setCharacterSize(24);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setPosition(20.f, 10.f);

    sf::Text livesText;
    livesText.setFont(font);

    sf::Text passText;
    passText.setFont(font);
    passText.setCharacterSize(50);
    passText.setString("Captured the Flag!");
    passText.setFillColor(sf::Color::Black);
    passText.setPosition(200.f, 250.f);


    //Main cycle
    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2 pos = tux.getPosition();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
            currentFrame = (currentFrame + 1) % 4;
            animationClock.restart();
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && pos.x < 780.f) {

                tux.setTexture(tuxRight);
                tux.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));

                tux.move(2.5f, 0.f);
                if (checkWallCollision(tux, maze, wall)) {
                    tux.move(-2.5f, 0.f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && pos.x > 0.f) {

                tux.setTexture(tuxLeft);
                tux.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));

                tux.move(-2.5f, 0.f);
                if (checkWallCollision(tux, maze, wall)) {
                    tux.move(2.5f, 0.f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && pos.y > 0.f) {

                tux.setTexture(tuxUp);
                tux.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));

                tux.move(0.f, -2.5f);
                if (checkWallCollision(tux, maze, wall)) {
                    tux.move(0.f, 2.5f);
                }
            }
        }

        if (!isLevelComplete) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && pos.y < 580.f) {

                tux.setTexture(tuxDown);
                tux.setTextureRect(sf::IntRect(currentFrame * 32, 0, 32, 32));

                tux.move(0.f, 2.5f);
                if (checkWallCollision(tux, maze, wall)) {
                    tux.move(0.f, -2.5f);
                }
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
        {
            tux.setTextureRect(sf::IntRect(0, 0, 32, 32));
        }


        window.clear(sf::Color(190, 225, 245));

        //Map drawing section

        window.draw(tux); //Draws tux

        //Draw Maze
        for (int i = 0; i<MAP_HEIGHT; ++i) {
            for (int j = 0; j<MAP_WIDTH; ++j) {
                if (maze[i][j] == 1) {
                    wall.setPosition(j * 40.f, i * 40.f);
                    window.draw(wall);
                }
                else if (maze[i][j] == 2) {
                    coin.setPosition(j * 40.f, i * 40.f);
                    if (tux.getGlobalBounds().intersects(coin.getGlobalBounds())) {
                        maze[i][j] = 0;
                        ++coins;
                        coinSound.play();
                    }
                    else {
                        window.draw(coin);
                    }
                }
                else if (maze[i][j] == 3) {
                    trap.setPosition(j * 40.f - 4.f, i * 40.f - 4.f);

                    sf::FloatRect trapHitbox (
                        j * 40.f + 12.f,
                        i * 40.f + 12.f,
                        16.f,
                        16.f
                    );

                    if (tux.getGlobalBounds().intersects(trapHitbox)) {
                        tux.setPosition(60.f, 270.f);
                        trapSound.play();
                    }
                    else {
                        window.draw(trap);
                    }
                }
                else if (maze[i][j] == 4) {
                    flag.setPosition(j * 40.f + 10.f, i * 40.f + 5.f);
                    if (tux.getGlobalBounds().intersects(flag.getGlobalBounds())) {
                        isLevelComplete = true;
                        if (!isWinTrackPlayed) {
                            victorySound.play();
                            isWinTrackPlayed = true;
                        }
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