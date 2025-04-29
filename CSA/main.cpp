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

	int player_def = 0;
	int enemy_def = 0;

    // Dynamically allocate a player (using 'new')
    int level = 1;
    Character* player = new Player("Erou");
    Character* enemy = new Enemy("Hot", level);

    sf::RenderWindow window(sf::VideoMode({ 1050, 700 }), "CSA");
    window.setFramerateLimit(60);

    sf::Texture texture;
	if (!texture.loadFromFile("arena.png")) {
		std::cerr << "Error loading background texture" << std::endl;
		return -1;
	}
	sf::Sprite background(texture);

    sf::Font font("arial.ttf");

    sf::Text playerStatus(font);
    playerStatus.setCharacterSize(24);
    playerStatus.setPosition({ 0, 700 - 24*6 });
    playerStatus.setString("Erou HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) +
        "Attack: " + std::to_string(player->getAttackPower()) + "Defence: " + std::to_string(player->getDefensePower()));

    sf::Text enemyStatus(font);
    enemyStatus.setCharacterSize(24);
    enemyStatus.setPosition({ 0, 700 - 24*5 });
    enemyStatus.setString("Hot HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()));

    sf::Text actionText(font);
    actionText.setCharacterSize(24);
    actionText.setPosition({ 0, 700-24 });
    actionText.setString("Press A to Attack, D to defend, H to Heal");

    sf::Text enemyAction(font);
    enemyAction.setCharacterSize(24);
    enemyAction.setPosition({ 0, 700-24*2 });


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            // Player input
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (!gameOver && playerTurn) {
                    char action = '\0';
                    if (keyPressed->scancode == sf::Keyboard::Scancode::A) action = 'A';
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::D) action = 'D';
                    else if (keyPressed->scancode == sf::Keyboard::Scancode::H) action = 'H';

                    if (action != '\0') {
                        try {
							player_def = player->getDefensePower();
                            dynamic_cast<Player*>(player)->takeTurn(*enemy, action);
							if (enemy->getDefensePower() != enemy_def && enemy_def!=0)
							{
								enemy->setDefensePower(enemy->getDefensePower() / 2);
							}
                            playerTurn = false;
                        }
                        catch (const std::invalid_argument& e) {
                            actionText.setString(e.what());
                        }
                    }
                }

                // Restart next level after victory
                if (!enemy->isAlive() && keyPressed->scancode == sf::Keyboard::Scancode::C) {
                    level++;
                    delete enemy;
                    enemy = new Enemy("Hot", level);
                    dynamic_cast<Player*>(player)->levelUp();
                    playerTurn = true;
                    enemyAction.setString("");
					actionText.setString("Press A to Attack, D to defend, H to Heal");
                }
            }
        }

        if (!gameOver && !playerTurn) {
            if (enemy->isAlive()) {
                int prevDef = enemy->getDefensePower();
                int prevHp = player->getHp();

				enemy_def = enemy->getDefensePower();
                dynamic_cast<Enemy*>(enemy)->takeTurn(*player);

				if (player->getDefensePower() != player_def)
				{
					player->setDefensePower(player->getDefensePower() / 2);
				}

                if (player->getHp() < prevHp) {
                    enemyAction.setString(enemy->getName() + " attacked you!");
                }
                else if (enemy->getDefensePower() > prevDef) {
                    enemyAction.setString(enemy->getName() + " defended!");
                }
                else {
                    enemyAction.setString(enemy->getName() + " healed!");
                }

                playerTurn = true;
            }
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
            bool keyPressed = false;


            // Game over check
            if (!player->isAlive()) {
                actionText.setString("You Died! Press ESC to Exit");
                gameOver = true;
            }
            else if (!enemy->isAlive()) {
                actionText.setString("You defeated the " + enemy->getName() + "! Press C to continue.");
            }

            // Update UI
            playerStatus.setString("Erou HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp()) +
                "  ATK: " + std::to_string(player->getAttackPower()) + "  DEF: " + std::to_string(player->getDefensePower()));
            enemyStatus.setString("Hot HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp()) +
                "  ATK: " + std::to_string(enemy->getAttackPower()) + "  DEF: " + std::to_string(enemy->getDefensePower()));

            window.clear();
			window.draw(background);
            window.draw(playerStatus);
            window.draw(enemyStatus);
            window.draw(actionText);
            window.draw(enemyAction);
            window.display();
        }
    }
        return 0;
}


//// Window closing (basic, not using event)
//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
//    window.close();
//}
//
//if (!gameOver) {
//    if (playerTurn) {
//        if (!keyPressed) {
//            char action = '\0';
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) action = 'A';
//            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) action = 'D';
//            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)) action = 'H';
//
//            if (action != '\0') {
//                try {
//                    dynamic_cast<Player*>(player)->takeTurn(*enemy, action);
//                    playerTurn = false;
//                    keyPressed = true;
//                }
//                catch (const std::invalid_argument& e) {
//                    actionText.setString(e.what());
//                }
//            }
//        }
//    }
//    else {
//        if (enemy->isAlive()) {
//            int def = enemy->getDefensePower();
//            int playerhp = player->getHp();
//            dynamic_cast<Enemy*>(enemy)->takeTurn(*player);
//            if (player->getHp() != playerhp)
//            {
//                enemyAction.setString(enemy->getName() + " attacked you!");
//                std::cout << enemy->getName() + " attacked you!" << std::endl;
//            }
//            else if (def != enemy->getDefensePower()) {
//                enemy->setDefensePower(enemy->getDefensePower() / 2);
//                enemyAction.setString(enemy->getName() + " defended!");
//                std::cout << enemy->getName() + " defended!" << std::endl;
//            }
//            else {
//                enemyAction.setString(enemy->getName() + " healed!");
//                std::cout << enemy->getName() + " healed!" << std::endl;
//            }
//            playerTurn = true;
//            keyPressed = false;
//            std::cout << "Rendering frame..." << std::endl;
//
//            // nu afiseaza mesajul ce va face inamicul + nu mai are loop jocul.
//        }
//    }
//
//    // Reset keyPressed flag if no keys are being pressed
//    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
//        !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
//        !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) &&
//        !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
//        bool keyPressed = false;
//    }
//}

//if (!player->isAlive()) {
//    actionText.setString("You Died! Press ESC to Exit");
//    gameOver = true;
//}
//else if (!enemy->isAlive()) {
//    //make it so that when the enemy is dead, it waits for a key press to continue
//    actionText.setString("You defeated the " + enemy->getName() + "! Press C to continue.");
//    // Wait for a key press
//    while (!keyPressed) {
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
//        {
//            keyPressed = true;
//            level++;
//            delete enemy;
//            enemy = new Enemy("Goblin", level);
//            dynamic_cast<Player*>(player)->levelUp();
//        }
//    }
//}
//
//// Update status texts
//playerStatus.setString("Player HP: " + std::to_string(player->getHp()) + "/" + std::to_string(player->getMaxHp())
//    + "  ATK: " + std::to_string(player->getAttackPower()) + "  DEF: " + std::to_string(player->getDefensePower()));
//
//enemyStatus.setString("Enemy HP: " + std::to_string(enemy->getHp()) + "/" + std::to_string(enemy->getMaxHp())
//    + "  ATK: " + std::to_string(enemy->getAttackPower()) + "  DEF: " + std::to_string(enemy->getDefensePower()));
//
//// Drawing
//window.clear(sf::Color::Black);
//window.draw(playerStatus);
//window.draw(enemyStatus);
//window.draw(actionText);
//window.draw(enemyAction);
//window.display();