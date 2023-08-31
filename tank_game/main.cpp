
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main() {
    // Initialize window and other constants
    const int windowWidth = 800;
    const int windowHeight = 600;
    int lives = 5;
    const float tankSpeed = 5.0f;
    const float cannonRotationSpeed = 1.0f;
    const float projectileSpeed = 10.0f;
    const int numFruits = 8;
    const float fruitRadius = 10.0f;

    bool isProjectileActive = false; // Flag to indicate if a projectile is currently active
    int fruitsShot = 0;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tank Game");




    // Define shapes for game elements
    sf::RectangleShape barrier(sf::Vector2f(130, 20));
    barrier.setFillColor(sf::Color::Blue);
    barrier.setPosition(0, 200);

    sf::RectangleShape barrier2(barrier);
    barrier2.setPosition(330, 200);

    sf::RectangleShape barrier3(barrier);
    barrier3.setPosition(680, 200);

    sf::RectangleShape tank(sf::Vector2f(100, 50));
    tank.setFillColor(sf::Color::Green);
    tank.setPosition(350, 500);

    sf::RectangleShape cannon(sf::Vector2f(15, 30));
    cannon.setFillColor(sf::Color::Red);

    sf::RectangleShape projectile(sf::Vector2f(10, 20));
    projectile.setFillColor(sf::Color::Yellow);

    // the fruit vector
    std::vector<sf::CircleShape> fruits;
    for (int i = 0; i < numFruits; ++i) {
        sf::CircleShape fruit(fruitRadius);
        fruit.setFillColor(sf::Color::Yellow);

        //  fruit randomly above the barriers
        float x = static_cast<float>(rand() % (window.getSize().x - 2 * static_cast<int>(fruitRadius)));
        float y = static_cast<float>(rand() % 190);
        fruit.setPosition(x, y);

        fruits.push_back(fruit);
    }

    // Load font
    sf::Font font1;
    if (!font1.loadFromFile("Gameplay.ttf")) {
        return EXIT_FAILURE;
    }
    // Create and display the game over and congrats messages
    sf::Text gameOverText;
    gameOverText.setFont(font1);
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over");
    // Calculate the position to center the text on the screen
    float gameOverTextX = (windowWidth - gameOverText.getLocalBounds().width) / 2;
    float gameOverTextY = (windowHeight - gameOverText.getLocalBounds().height) / 2;
    gameOverText.setPosition(gameOverTextX, gameOverTextY);
    // Create the "Congratulations" text
    sf::Text congratsText;
    congratsText.setFont(font1);
    congratsText.setCharacterSize(60);
    congratsText.setFillColor(sf::Color::Green);
    congratsText.setString("Congratulations !");

    // Calculate the position to center the text on the screen
    float congratsTextX = (windowWidth - congratsText.getLocalBounds().width) / 2;
    float congratsTextY = (windowHeight - congratsText.getLocalBounds().height) / 2;
    congratsText.setPosition(congratsTextX, congratsTextY);

    // Create a text object for displaying lives
    sf::Text livesText;
    livesText.setFont(font1);
    livesText.setCharacterSize(14);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(10.0f, 10.0f); // Adjust the position as needed
    // Create a text object for displaying score
    sf::Text scoreText;
    scoreText.setFont(font1);
    scoreText.setCharacterSize(14);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(700.0f, 10.0f); // Adjust the position as needed


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle window close event
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle spacebar press to shoot projectile
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!isProjectileActive) {
                    isProjectileActive = true;
                    projectile.setPosition(cannon.getPosition().x + (cannon.getSize().x - projectile.getSize().x) / 2,
                                           cannon.getPosition().y);
                }
            }
        }

        // Tank movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            tank.move(-tankSpeed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            tank.move(tankSpeed, 0);
        }

        // Cannon inclination
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cannon.rotate(-cannonRotationSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cannon.rotate(cannonRotationSpeed);
        }

        // Update cannon position based on tank position
        // Calculate the position of the cannon's base (center of tank)
        sf::Vector2f tankCenter = sf::Vector2f(tank.getPosition().x + tank.getSize().x / 2,
                                               tank.getPosition().y + tank.getSize().y / 2);

        // Calculate the position of the cannon's base based on cannon's rotation
        float angle = cannon.getRotation();
        float angleRad = angle * (3.14159265359 / 180.0);
        sf::Vector2f cannonBasePos = tankCenter + sf::Vector2f(0, -tank.getSize().y / 2) + sf::Vector2f(-std::sin(angleRad) * cannon.getSize().y,
                                                                                                        std::cos(angleRad) * cannon.getSize().y);

        cannon.setPosition(cannonBasePos.x - cannon.getSize().x / 2,
                           cannonBasePos.y - cannon.getSize().y / 2);

        // Update projectile position
        if (isProjectileActive) {
            float angle = cannon.getRotation(); // Get the current rotation angle of the cannon in degrees
            float angleRad = angle * (3.14159265359 / 180.0); // Convert angle to radians
            float speedX = std::sin(angleRad) * projectileSpeed; // Calculate the X component of speed
            float speedY = -std::cos(angleRad) * projectileSpeed; // Calculate the Y component of speed
            projectile.move(speedX, speedY);

            // Check if projectile is out of the screen
            sf::Vector2f projectilePosition = projectile.getPosition();
            if (projectilePosition.x < 0 || projectilePosition.x > window.getSize().x ||
                projectilePosition.y < 0 || projectilePosition.y > window.getSize().y) {
                isProjectileActive = false; // Reset projectile when it goes off-screen
            }
        }

        // Handle projectile collisions with barriers and window edges
        if (isProjectileActive) {
            projectile.move(0, -projectileSpeed);

            // Collision with top of the window
            if (projectile.getPosition().y < 0) {
                isProjectileActive = false;
                lives--;


            }

            // Collision with barriers
            if (projectile.getGlobalBounds().intersects(barrier.getGlobalBounds()) ||
                projectile.getGlobalBounds().intersects(barrier2.getGlobalBounds()) ||
                projectile.getGlobalBounds().intersects(barrier3.getGlobalBounds())) {
                isProjectileActive = false;
                lives--;


            }
        }

        // Handle projectile collisions with fruits
        for (auto& fruit : fruits) {
            if (fruit.getGlobalBounds().intersects(projectile.getGlobalBounds())) {
                // Fruit collided with projectile
                isProjectileActive = false;
                fruit.setPosition(static_cast<float>(rand() % (window.getSize().x - 2 * static_cast<int>(fruitRadius))) + fruitRadius, 0);
                fruitsShot++; // Increment the fruits shot counter
            }
        }
        livesText.setString("Lives: " + std::to_string(lives));
        scoreText.setString("Score: " + std::to_string(fruitsShot));


        // Draw elements
        window.clear();
        window.draw(barrier);
        window.draw(barrier2);
        window.draw(barrier3);
        window.draw(tank); // Draw the tank sprite
        window.draw(cannon);
        for (auto& fruit : fruits) {
            window.draw(fruit);
        }

        if (isProjectileActive) {
            window.draw(projectile);
        }

        // Draw lives text and scores
        window.draw(livesText);
        window.draw(scoreText);

        // Inside the game loop
        if (lives <= 0) {
            window.draw(gameOverText);
            window.display();
            sf::sleep(sf::seconds(2)); // Display "Game Over" for 2 seconds before closing
            window.close();
        } else if (fruitsShot >= 5) {
            window.draw(congratsText); // Display congrats message
            window.display();
            sf::sleep(sf::seconds(2)); // Display "Congratulations" for 2 seconds before closing
            window.close();
        }

        window.display();
    }

    return 0;
}

