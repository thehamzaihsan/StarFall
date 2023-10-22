#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>

enum class GameState {
    MainMenu,
    SinglePlayer,
    Multiplayer,
    GameOver,
    TipsScreen,
    Credits,
};

using namespace std;
using namespace sf;

void spawnEnemies(Sprite& player, RenderWindow& window, vector < Sprite >& enemies, Sprite& asteroidSprite) {
    //enemies spawning
    Sprite Asteroid_Sprite(asteroidSprite);

    // Define the range within which the asteroid can be positioned
    float range = 100.0f; // Adjust this value to change the range

    // Calculate the minimum and maximum x-coordinatesfor the asteroid's position
    float minX = player.getPosition().x - range;
    float maxX = player.getPosition().x + range;

    // Generate a random x-coordinate within the calculated range
    float randomX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (maxX - minX)) + minX;

    // Set the asteroid's position using the random x-coordinate and a y-coordinate of 0
    Asteroid_Sprite.setPosition(randomX, 0.0f);

    // Set the rotation of the sprite to a random value between 0 and 360 degrees
    Asteroid_Sprite.setRotation(static_cast <float> (rand() % 360));

    enemies.push_back(Asteroid_Sprite);

}



void spawnBullets(Vector2f playerpos, vector < Sprite >& bullets, Sprite bulletSprite) {

    bulletSprite.setScale(0.5f, 0.5f);
    bulletSprite.setPosition(playerpos);
    bulletSprite.setOrigin(bulletSprite.getLocalBounds().width / 2, bulletSprite.getLocalBounds().height / 2);
    bullets.push_back(bulletSprite);

}

void spawnBullets2(Vector2f playerpos, vector < Sprite >& bullets, Sprite bulletSprite) {

    bulletSprite.setScale(0.5f, 0.5f);
    bulletSprite.setPosition(playerpos);
    bulletSprite.setOrigin(bulletSprite.getLocalBounds().width / 2, bulletSprite.getLocalBounds().height / 2);
    bullets.push_back(bulletSprite);

}


void updateStars(RenderWindow& window, vector < CircleShape >& stars, vector <float >& speeds) {
    for (size_t i = 0; i < stars.size(); ++i) {
        stars[i].move(0, speeds[i]);

        if (stars[i].getPosition().y > window.getSize().y) {
            stars[i].setPosition(rand() % window.getSize().x, 0);
        }
    }
}

void drawStars(RenderWindow& window,
    const vector < CircleShape >& stars) {
    for (const auto& star : stars) {
        window.draw(star);
    }
}

int main() {
    GameState gameState = GameState::MainMenu;
    //VARIABLES
    int STAR_COUNT = 100;
    float STAR_SPEED = 3.0f;
    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;
    int speed = 5;
    int lives = 3;
    int score = 0;
    int score2 = 0;
    float gravity = 5.0f;
    int enemySpawnTimerMax = 75.f;
    int enemySpawnTimer = enemySpawnTimerMax;
    int maxEnemies = 10;
    int reloadTimerMax = 5.0f;
    int reloadTimer = reloadTimerMax;
    int bulletspeed = 30.0f;
    int noBullets = 10;
    int level = 1;
    Font font;
    Text scoreText;
    Text score2Text;

    Text livesText;
    Text BulletText;
    vector < Sprite > bullets;
    vector < CircleShape > stars;
    vector <float > speeds;
    Texture bulletTexture;
    Music backgroundMusic;
    Sprite bulletSprite;
    Texture jetTexture;
    vector < Sprite > enemies;
    Texture asteroidTexture;
    vector < Sprite > bullets2;
    int reloadTimerMax2 = 5.0f;
    int reloadTimer2 = reloadTimerMax2;
    bool livesIncremented = false;
    bool doubleShooting = false;
    if (!backgroundMusic.openFromFile("./Sounds/wandering-6394.wav")) {
        std::cout << "Failed to load background music!" << std::endl;
        return 1;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(70);
    backgroundMusic.play();
    if (gameState == GameState::Multiplayer) {
        lives = 6;
    }

    //Opening a window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Starfall");
    window.setFramerateLimit(60);

    // Loadfont
    if (!font.loadFromFile("Fonts/SpaceMono-Bold.ttf")) {
        cerr << "Failed to loadfont" << endl;
        return -1;
    }


    // define menu options
    const int NUM_MENU_ITEMS = 5;
    string menuItems[NUM_MENU_ITEMS] = {
      "SinglePlayer",
      "Multiplayer",
      "How to play",
      "Credits",
      "Exit"
    };


    // Load textures
    if (!bulletTexture.loadFromFile("./Assets/bullet.png")) {
        cerr << "Failed to load bullet texture" << endl;
        return -1;
    }

    if (!jetTexture.loadFromFile("./Assets/pngwingcom.png")) {
        cerr << "Failed to load jet texture" << endl;
        return -1;
    }

    if (!asteroidTexture.loadFromFile("./Assets/Asteroid Brown.png")) {
        cerr << "Failed to load asteroid texture" << endl;
        return -1;
    }

    Sprite jetSprite(jetTexture);
    jetSprite.setScale(0.2f, 0.2f); // Adjust the scalefactor as needed
    jetSprite.setOrigin(jetSprite.getLocalBounds().width / 2, jetSprite.getLocalBounds().height / 2);

    // Create the asteroid sprite and set its texture
    Sprite asteroidSprite;
    asteroidSprite.setTexture(asteroidTexture);
    asteroidSprite.setScale(0.5f, 0.5f);

    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setScale(0.5f, 0.5f);

    //for multiplayer
    //jet 2
    Sprite jetSprite2(jetTexture);
    jetSprite2.setScale(0.2f, 0.2f);
    jetSprite2.setOrigin(jetSprite2.getLocalBounds().width / 2, jetSprite2.getLocalBounds().height / 2);
    if (gameState == GameState::Multiplayer) {
        jetSprite2.setPosition(window.getSize().x / 2.0f, window.getSize().y - 50);
    }

    //Setting Playing Screen Texts
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setString("Score 1");
    scoreText.setPosition(Vector2f(10.0f, 10.0f));

    score2Text.setFont(font);
    score2Text.setCharacterSize(24);
    score2Text.setFillColor(Color::White);
    score2Text.setString("Score 2");
    score2Text.setPosition(Vector2f(10.0f, 40.0f));

    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::White);
    livesText.setString("Lives");


    BulletText.setFont(font);
    BulletText.setCharacterSize(24);
    BulletText.setFillColor(Color::White);
    BulletText.setString("Bullets");
    BulletText.setPosition(Vector2f(window.getSize().y - 150, 10.0f));

    Text tipsTitleText1;
    tipsTitleText1.setFont(font);
    tipsTitleText1.setString("Starfall");
    tipsTitleText1.setCharacterSize(48);
    tipsTitleText1.setFillColor(Color::White);
    tipsTitleText1.setPosition(WINDOW_WIDTH / 2 - tipsTitleText1.getGlobalBounds().width / 2, 50);

    //INITIALIZING STAR BACKGROUND
    srand(static_cast <unsigned int> (time(NULL)));
    stars.reserve(STAR_COUNT);
    speeds.reserve(STAR_COUNT);
    for (int i = 0; i < STAR_COUNT; ++i) {
        float x = rand() % WINDOW_WIDTH;
        float y = rand() % WINDOW_HEIGHT;
        float speed = STAR_SPEED + (rand() % 5) / 10.0f;

        CircleShape star(1.0f);
        star.setFillColor(Color(255, 255, 255, rand() % 128 + 128));
        star.setPosition(x, y);

        stars.push_back(star);
        speeds.push_back(speed);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                // Create a view with the new size
                sf::View view(sf::FloatRect(0, 0, event.size.width, event.size.height));
                window.setView(view);

                WINDOW_HEIGHT = event.size.height;
                WINDOW_WIDTH = event.size.width;
            }
        }


        //Setting Text Position
        livesText.setPosition(Vector2f(WINDOW_WIDTH - 150, 10.0f));


        //Leveling system
        level = score / 7 + 1;

        if (score2 > 0)
        {
            level = ((score + score2) / 2) / 10 + 1;
        }

        if (level == 2) {
            maxEnemies = 12;
            gravity = 7.0f;
            enemySpawnTimerMax = 60.f;
        }
        else if (level == 3) {
            maxEnemies = 14;
            gravity = 7.5f;
            enemySpawnTimerMax = 55.f;
        }
        else if (level == 4) {
            maxEnemies = 16;
            gravity = 7.75f;
            enemySpawnTimerMax = 50.f;
        }
        else if (level == 5) {
            maxEnemies = 18;
            gravity = 8.0f;
            enemySpawnTimerMax = 50.f;
        }
        else if (level == 6) {
            maxEnemies = 20;
            gravity = 8.0f;
            enemySpawnTimerMax = 50.f;
        }
        else if (level == 7) {
            maxEnemies = 20;
            gravity = 7.0f;
            enemySpawnTimerMax = 45.f;
        }

        if (level % 3 == 0 && gameState == GameState::SinglePlayer && level !=0) {
            doubleShooting = true;
            scoreText.setFillColor(Color::Green);

        }
        else {
            doubleShooting = false;
            scoreText.setFillColor(Color::White);
        }

        




        //updating happens here

        if (gameState == GameState::SinglePlayer) {
            if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && jetSprite.getPosition().y > 0)
                jetSprite.move(0, -speed);
            if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && jetSprite.getPosition().y < window.getSize().y)
                jetSprite.move(0, speed);
            if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && jetSprite.getPosition().x > 0)
                jetSprite.move(-speed, 0);
            if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && jetSprite.getPosition().x < window.getSize().x)
                jetSprite.move(speed, 0);
        }

        

        //Enemy generation
        if (enemies.size() < maxEnemies) {
            if (enemySpawnTimer >= enemySpawnTimerMax) {

                if (gameState == GameState::SinglePlayer || gameState == GameState::Multiplayer)
                {
                    spawnEnemies(jetSprite, window, enemies, asteroidSprite);
                }

                if (gameState == GameState::Multiplayer) {
                    spawnEnemies(jetSprite2, window, enemies, asteroidSprite);
                }

                enemySpawnTimer = 0.f;

            }
            else {
                enemySpawnTimer += 1.f;
            }
        }

        //updating enemies
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].move(0.0f, gravity);

            if (enemies[i].getGlobalBounds().intersects(jetSprite.getGlobalBounds())) {
                if (gameState == GameState::SinglePlayer || gameState == GameState::Multiplayer) {
                    enemies.erase(enemies.begin() + i);
                    lives--;
                }
            }
            else if (enemies[i].getGlobalBounds().intersects(jetSprite2.getGlobalBounds())) {
                if (gameState == GameState::SinglePlayer || gameState == GameState::Multiplayer) {
                    enemies.erase(enemies.begin() + i);
                    lives--;
                }
            }
            else if (enemies[i].getPosition().y > window.getSize().y) {
                enemies.erase(enemies.begin() + i);
                /* score++;*/ //This code used to update score every time an asteroid goes outside the screen
            }

        }



        //Displaying scores and levels
        stringstream scr;
        scr << "Score: " << score;
        scoreText.setString(scr.str());

        stringstream scr2;
        scr2 << "Score: " << score2;
        score2Text.setString(scr2.str());

        stringstream livesstring;
        livesstring << "Lives: " << lives;
        livesText.setString(livesstring.str());

        if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::LShift)) {
            if (reloadTimer >= reloadTimerMax) {

                if (!doubleShooting) {
                    spawnBullets(jetSprite.getPosition(), bullets, bulletSprite);
                }
                else if (doubleShooting) {
                    Vector2f newPos1(jetSprite.getPosition().x + 10.0f, jetSprite.getPosition().y);
                    Vector2f newPos2(jetSprite.getPosition().x - 10.0f, jetSprite.getPosition().y);

                    spawnBullets(newPos1, bullets, bulletSprite);
                    spawnBullets(newPos2, bullets, bulletSprite);

                }
                reloadTimer = 0.f;
            }
        }
        else {
            reloadTimer += 1.f;
        }

        for (int i = 0; i < bullets.size(); i++) {
            bullets[i].move(0.0f, -bulletspeed);

            if (bullets[i].getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                for (int j = 0; j < enemies.size(); j++) {
                    if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
                        score++;
                        bullets.erase(bullets.begin() + i);
                        enemies.erase(enemies.begin() + j);
                        break;
                    }
                }
            }

        }

        // create text objectsfor each menu item
        Text menuTexts[NUM_MENU_ITEMS];
        for (int i = 0; i < NUM_MENU_ITEMS; i++) {
            menuTexts[i].setFont(font);
            menuTexts[i].setString(menuItems[i]);
            menuTexts[i].setCharacterSize(30);
            menuTexts[i].setFillColor(Color::White);

            // center text origin
            FloatRect textRect = menuTexts[i].getLocalBounds();
            menuTexts[i].setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height + 40.0f);

            // position text
            float x = WINDOW_WIDTH / 2.0f;
            float y = WINDOW_HEIGHT / 2.0f + i * 75;
            menuTexts[i].setPosition(Vector2f(x, y));
        }


        if (gameState == GameState::Multiplayer) {
            if (Keyboard::isKeyPressed(Keyboard::Up) && jetSprite2.getPosition().y > 0)
                jetSprite2.move(0, -speed);
            if (Keyboard::isKeyPressed(Keyboard::Down) && jetSprite2.getPosition().y < window.getSize().y)
                jetSprite2.move(0, speed);
            if (Keyboard::isKeyPressed(Keyboard::Left) && jetSprite2.getPosition().x > 0)
                jetSprite2.move(-speed, 0);
            if (Keyboard::isKeyPressed(Keyboard::Right) && jetSprite2.getPosition().x < window.getSize().x)
                jetSprite2.move(speed, 0);

            if (Keyboard::isKeyPressed(Keyboard::W) && jetSprite.getPosition().y > 0)
                jetSprite.move(0, -speed);
            if (Keyboard::isKeyPressed(Keyboard::S) && jetSprite.getPosition().y < window.getSize().y)
                jetSprite.move(0, speed);
            if (Keyboard::isKeyPressed(Keyboard::A) && jetSprite.getPosition().x > 0)
                jetSprite.move(-speed, 0);
            if (Keyboard::isKeyPressed(Keyboard::D) && jetSprite.getPosition().x < window.getSize().x)
                jetSprite.move(speed, 0);

            // Spawning bulletsfor jet2
            if (Mouse::isButtonPressed(Mouse::Left) || Keyboard::isKeyPressed(Keyboard::RShift)) {
                if (reloadTimer2 >= reloadTimerMax2) {

                    spawnBullets2(jetSprite2.getPosition(), bullets2, bulletSprite);
                    reloadTimer2 = 0.f;
                }
            }
            else {
                reloadTimer2 += 1.f;
            }

            for (int i = 0; i < bullets2.size(); i++) {
                bullets2[i].move(0.0f, -bulletspeed);

                if (bullets2[i].getPosition().y < 0) {
                    bullets2.erase(bullets2.begin() + i);
                }
                else {
                    for (int j = 0; j < enemies.size(); j++) {
                        if (bullets2[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
                            score2++;
                            bullets2.erase(bullets2.begin() + i);
                            enemies.erase(enemies.begin() + j);
                            break;
                        }
                    }
                }

            }
        }

        //RENDERING STARTS FROM HERE
        window.clear();

        updateStars(window, stars, speeds);
        drawStars(window, stars);

        if (gameState == GameState::GameOver) {
            level = 1;

            Text startText("GameOver (Escape)", font, 40);
            startText.setFillColor(Color::Red);

            // Get the local bounds of the text
            FloatRect textRect = startText.getLocalBounds();

            // Set the origin of the text to its center
            startText.setOrigin(textRect.left + textRect.width / 2.0f,
                textRect.top + textRect.height / 2.0f);

            // Set the position of the text to the center of the window
            startText.setPosition(Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

            window.draw(startText);

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                level = 1;
                lives = 3;
                score = 0;
                jetSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100);
                jetSprite2.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100);
                gameState = GameState::MainMenu;
            }
        }
        if (gameState == GameState::MainMenu) {


            window.draw(tipsTitleText1);
           
            // draw menu texts
            for (int i = 0; i < NUM_MENU_ITEMS; i++) {
                window.draw(menuTexts[i]);
            }

            tipsTitleText1.setPosition(WINDOW_WIDTH / 2 - tipsTitleText1.getGlobalBounds().width / 2, 50);

            // handle mouse click
            if (Mouse::isButtonPressed(Mouse::Left)) {
                // get mouse position
                Vector2i mousePos = Mouse::getPosition(window);

                // check if mouse is within bounds of a menu item
                for (int i = 0; i < NUM_MENU_ITEMS; i++) {
                    if (menuTexts[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        if (i == 0) {
                            gameState = GameState::SinglePlayer;
                        }
                        if (i == 1) {
                            gameState = GameState::Multiplayer;

                        }
                        if (i == 2) {
                            gameState = GameState::TipsScreen;
                        }
                        if (i == 3) {
                            gameState = GameState::Credits;
                        }
                        if (i == 4) {
                            window.close();
                        }
                    }
                }
            }

            level = 1;
            lives = 3;
            score = 0;
            jetSprite.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100);
            jetSprite2.setPosition(window.getSize().x / 2.0f, window.getSize().y - 100);

            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                gameState = GameState::SinglePlayer;
            }
        }
        if (gameState == GameState::SinglePlayer) {
            //rendering bulets
            for (auto& e : bullets) {
                window.draw(e);
            }

            //rendering enemies
            for (auto& e : enemies) {
                window.draw(e);

            }

            window.draw(scoreText);
            window.draw(livesText);
            window.draw(jetSprite);

            if (lives <= 0) {
                gameState = GameState::GameOver;
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                gameState = GameState::MainMenu;
            }

        }
        if (gameState == GameState::TipsScreen) {

            Text tipsTitleText;
            tipsTitleText.setFont(font);
            tipsTitleText.setString("Tips");
            tipsTitleText.setCharacterSize(48);
            tipsTitleText.setFillColor(Color::White);
            tipsTitleText.setPosition(WINDOW_WIDTH / 2 - tipsTitleText.getGlobalBounds().width / 2, 50);

            Text tipsContentText;
            tipsContentText.setFont(font);
            tipsContentText.setString("Avoid the asteroids and shoot them down!\n\nUse the arrow keys to move.\nPress Space to shoot.\n\nPress Esc to go back.");
            tipsContentText.setCharacterSize(24);
            tipsContentText.setFillColor(Color::White);
            tipsContentText.setPosition(WINDOW_WIDTH / 2 - tipsContentText.getGlobalBounds().width / 2, 150);

            window.draw(tipsTitleText);
            window.draw(tipsContentText);

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                gameState = GameState::MainMenu;
            }
        }
        if (gameState == GameState::Credits) {

            Text tipsTitleText;
            tipsTitleText.setFont(font);
            tipsTitleText.setString("Developed By");
            tipsTitleText.setCharacterSize(48);
            tipsTitleText.setFillColor(Color::White);
            tipsTitleText.setPosition(WINDOW_WIDTH / 2 - tipsTitleText.getGlobalBounds().width / 2, 50);

            Text tipsContentText;
            tipsContentText.setFont(font);
            tipsContentText.setString("Usama Saeed\n\nHamza Ihsan\n\nMuhammad Haseeb");
            tipsContentText.setCharacterSize(24);
            tipsContentText.setFillColor(Color::White);
            tipsContentText.setPosition(WINDOW_WIDTH / 2 - tipsContentText.getGlobalBounds().width / 2, 150);

            window.draw(tipsTitleText);
            window.draw(tipsContentText);

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                gameState = GameState::MainMenu;
            }
        }
        if (gameState == GameState::Multiplayer) {

            for (auto& e : bullets) {
                window.draw(e);
            }

            //rendering enemies/
            for (auto& e : enemies) {
                window.draw(e);

            }

            //rendering bulletsfor jet 2
            for (auto& e : bullets2) {
                window.draw(e);
            }

            window.draw(scoreText);
            window.draw(livesText);
            window.draw(jetSprite);
            window.draw(jetSprite2);
            window.draw(score2Text);

            if (lives <= 0) {
                gameState = GameState::GameOver;
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                gameState = GameState::MainMenu;
            }
        }

        window.display();

    }

    return 0;
}