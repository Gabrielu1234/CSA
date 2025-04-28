#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>

#include <memory>

//salut

int main() {


    // Seed for random number generation (for enemy actions)
    srand(static_cast<unsigned int>(time(0)));

	bool gameOver = false;
	bool playerTurn = true;
    bool keyPressed = false;

    // Dynamically allocate a player (using 'new')
    int level = 1;
    Character* player = new Player("Hero");
    Character* enemy = new Enemy("Goblin", level);

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "CSA");
	window.setFramerateLimit(60);

	sf::Font font("arial.ttf");

    sf::Text playerStatus(font);
    playerStatus.setCharacterSize(24);
    playerStatus.setPosition({ 0, 300 });
	playerStatus.setString("Player HP: " + std::to_string(player->getHp()) + "/"+ std::to_string(player->getMaxHp()) + 
        "Attack: " + std::to_string(player->getAttackPower()) + "Defence: " + std::to_string(player->getDefensePower()));

    sf::Text enemyStatus(font);
    enemyStatus.setCharacterSize(24);
    enemyStatus.setPosition({ 50, 350});
    enemyStatus.setString("Enemy HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()));

    sf::Text actionText(font);
    actionText.setCharacterSize(24);
    actionText.setPosition({ 0, 600-24 });
    actionText.setString("Press A to Attack, D to defend, H to Heal");

    sf::Text enemyAction(font);
	enemyAction.setCharacterSize(24);
	enemyAction.setPosition({ 0, 600 - 24 * 2 });


    while (window.isOpen()) {
        // Window closing (basic, not using event)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }

        if (!gameOver) {
            if (playerTurn) {
                if (!keyPressed) {
                    try {
                        dynamic_cast<Player*>(player)->takeTurn(*enemy);

                        // After the action is performed, detect if valid key pressed
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
                            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
                            playerTurn = false;
                            keyPressed = true;
                        }
                    }
                    catch (const std::invalid_argument& e) {
                        actionText.setString(e.what());
                    }
                }
            }
            else {
                if (enemy->isAlive()) {
					int def = enemy->getDefensePower();
					int playerhp = player->getHp();
                    dynamic_cast<Enemy*>(enemy)->takeTurn(*player);
                    if (player->getHp() != playerhp)
                    {
						actionText.setString(enemy->getName() + " attacked you!");
                    }
                    else if (def != enemy->getDefensePower()) {
						enemy->setDefensePower(enemy->getDefensePower() / 2);
						actionText.setString(enemy->getName() + " defended!");
					}
					else {
						actionText.setString(enemy->getName() + " healed!");
					}
                    playerTurn = true;
					keyPressed = false;
                    std::cout << "Rendering frame..." << std::endl;

                    // nu afiseaza mesajul ce va face inamicul + nu mai are loop jocul.
                }
            }

            // Reset keyPressed flag if no keys are being pressed
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
                bool keyPressed = false;
            }
        }

        if (!player->isAlive()) {
            actionText.setString("You Died! Press ESC to Exit");
            gameOver = true;
        }
        else if (!enemy->isAlive()) {
            level++;
            delete enemy;
            enemy = new Enemy("Goblin", level);
            dynamic_cast<Player*>(player)->levelUp();
        }

        // Update status texts
        playerStatus.setString("Player HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp())
            + "  ATK: " + std::to_string(player->getAttackPower()) + "  DEF: " + std::to_string(player->getDefensePower()));

        enemyStatus.setString("Enemy HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp())
            + "  ATK: " + std::to_string(enemy->getAttackPower()) + "  DEF: " + std::to_string(enemy->getDefensePower()));

        // Drawing
        window.clear(sf::Color::Black);
        window.draw(playerStatus);
        window.draw(enemyStatus);
        window.draw(actionText);
        window.draw(enemyAction);
        window.display();
    }

    return 0;
}
//while (window.isOpen()) {
//    sf::Event event;
//    while (window.pollEvent(event)) {
//        if (event.type == sf::Event::Closed)
//            window.close();
//    }
//
//    if (!player->isAlive()) {
//        actionText.setString("Game Over! Press Escape to exit.");
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
//            window.close();
//        continue;
//    }
//
//    if (!enemy || !enemy->isAlive()) {
//        if (enemy) delete enemy;
//        enemy = new Enemy("Goblin", level);
//        actionText.setString("Level " + std::to_string(level) + " Battle!");
//    }
//
//    playerHpText.setString("Player HP: " + std::to_string(player->getHp()));
//    enemyHpText.setString("Enemy HP: " + std::to_string(enemy->getHp()));
//
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
//        dynamic_cast<Player*>(player)->attack(*enemy);
//        actionText.setString("You attacked!");
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) {
//        dynamic_cast<Player*>(player)->heal();
//        actionText.setString("You healed!");
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
//        dynamic_cast<Player*>(player)->defend();
//        actionText.setString("You defended!");
//    }
//
//    if (enemy->isAlive()) {
//        showEnemyAction(*dynamic_cast<Enemy*>(enemy), actionText);
//        dynamic_cast<Enemy*>(enemy)->takeTurn(*player);
//    }
//    else {
//        actionText.setString(enemy->getName() + " defeated! Leveling up!");
//        dynamic_cast<Player*>(player)->levelUp();
//        level++;
//    }



//    // Game loop
//int level = 1;
//while (player->isAlive()) {
//    // Dynamically allocate an enemy with the current level
//    Character* enemy = new Enemy("Goblin", level);
//
//    // Print the starting message for the round
//    std::cout << "\n====================\n";
//    std::cout << "Level " << level << " Battle!\n";
//    std::cout << "====================\n";
//
//    // Game loop for the current battle
//    while (player->isAlive() && enemy->isAlive()) {
//        // Print status of both characters
//        std::cout << "\n====================\n";
//        std::cout << "Player Status:\n";
//        player->printStatus();
//        std::cout << "====================\n";
//        std::cout << "Enemy Status:\n";
//        enemy->printStatus();
//        std::cout << "====================\n";
//
//        // Show the enemy's next action
//        showEnemyAction(*dynamic_cast<Enemy*>(enemy));  // Cast the base class pointer to Enemy
//
//        // Player's turn: Let the player choose an action
//        dynamic_cast<Player*>(player)->takeTurn(*enemy);  // Cast to Player and call takeTurn
//
//        // Check if enemy is still alive
//        if (!enemy->isAlive()) {
//            std::cout << "\n" << enemy->getName() << " has been defeated! You win this round!\n";
//            dynamic_cast<Player*>(player)->levelUp();  // Player levels up after defeating an enemy
//            delete enemy;  // Delete the enemy object after the battle
//            break;  // Exit the inner loop to start a new enemy encounter
//        }
//
//        // Enemy's turn (random action)
//        std::cout << "\n" << enemy->getName() << "'s turn:\n";
//        dynamic_cast<Enemy*>(enemy)->takeTurn(*player);  // Cast to Enemy and call takeTurn
//
//        // Check if player is still alive
//        if (!player->isAlive()) {
//            std::cout << "\n" << player->getName() << " has been defeated! You lose...\n";
//            delete enemy;  // Delete the enemy object after the battle
//            break;  // Exit the loop if the player is defeated
//        }
//    }
//
//    // If player is still alive, increase the level for the next round
//    if (player->isAlive()) {
//        level++;
//    }
//    else {
//        delete player;  // Delete the player object if the game ends
//        break;  // Exit the game if the player has been defeated
//    }
//}
//
//std::cout << "\nGame Over!\n";
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
//window.close();
//
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
//{
//    actionText.setString("You attacked!");
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H))
//{
//    actionText.setString("You healed!");
//}
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
//{
//    actionText.setString("You defended!");
//}


//while (window.isOpen())
//{
//    while (!player->isAlive())
//    {
//        // Print status of both characters
//        while (player->isAlive() && enemy->isAlive())
//        {
//            // Show the enemy's next action
//            int action = rand() % 3;  // Random number between 0 and 2
//
//            switch (action) {
//            case 0:
//                //std::cout << enemy.getName() << " is about to attack!\n";
//                enemyAction.setString(enemy->getName() + " is about to attack!");
//                break;
//            case 1:
//                //std::cout << enemy.getName() << " is about to defend!\n";
//                enemyAction.setString(enemy->getName() + " is about to defend!");
//                break;
//            case 2:
//                //std::cout << enemy.getName() << " is about to heal!\n";
//                enemyAction.setString(enemy->getName() + " is about to heal!");
//                break;
//            }
//
//            // Player's turn: Let the player choose an action
//            dynamic_cast<Player*>(player)->takeTurn(*enemy);  // Cast to Player and call takeTurn
//
//            // Check if enemy is still alive
//            if (!enemy->isAlive()) {
//                std::cout << "\n" << enemy->getName() << " has been defeated! You win this round!\n";
//                dynamic_cast<Player*>(player)->levelUp();  // Player levels up after defeating an enemy
//                delete enemy;  // Delete the enemy object after the battle
//                break;  // Exit the inner loop to start a new enemy encounter
//            }
//
//            // Enemy's turn (random action)
//            std::cout << "\n" << enemy->getName() << "'s turn:\n";
//            dynamic_cast<Enemy*>(enemy)->takeTurn(*player);  // Cast to Enemy and call takeTurn
//
//            // Check if player is still alive
//            if (!player->isAlive()) {
//                std::cout << "\n" << player->getName() << " has been defeated! You lose...\n";
//                delete enemy;  // Delete the enemy object after the battle
//                break;  // Exit the loop if the player is defeated
//            }
//        }
//
//        // If player is still alive, increase the level for the next round
//        if (player->isAlive()) {
//            level++;
//        }
//        else {
//            delete player;  // Delete the player object if the game ends
//            break;  // Exit the game if the player has been defeated
//        }
//
//        actionText.setString("Game Over! Press Escape to exit.");
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
//            window.close();
//        continue;
//    }
//    // Clear the window
//    window.clear(sf::Color::Black);
//    window.draw(playerStatus);
//    window.draw(enemyStatus);
//    window.draw(actionText);
//    window.display();
//}