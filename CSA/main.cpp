#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Character.h"
#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>

#include <memory>

//salut
// 2/3 alocari dinamice, posibil sa adaug sunete dinamic, boss, animati etc.
//ierarhii de clase (charater in 2 (player si enemy), boss mosteneste enemy
//supradefinire a 3 operatori dar nefolositi, urmeaza :)
//polimorfism de mostenire avem, ad-hoc avem, parametric nu (poate pun)
//tablou polimorfm am, Character* characters[2]
//supradefinirea a cel putin o metoda am, attack def si heal
//metoda statica de resetare a jocului am, GameManager::resetGame, mai sunt optiuni de altele, sunet animatii etc.
//exceptii am, in player la takeTurn, in enemy la heal si cele de la sfml
//functii friend nu am
// clasa abstracta am, Character e abstracta
//lucru cu fisiere am, arial si arena dar nu cred ca se pun
//clase template nu am

//de lucru la frontend

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

    Character* characters[2];
    characters[0] = new Player("Erou");
    characters[1] = new Enemy("Hot", level);

    sf::RenderWindow window(sf::VideoMode({ 1050, 700 }), "CSA");
    window.setFramerateLimit(60);

    sf::Texture texture;
	if (!texture.loadFromFile("arena.png")) {
		std::cerr << "Error loading background texture" << std::endl;
		return -1;
	}
	sf::Sprite background(texture);

    sf::Font font("arial.ttf");

	sf::Text nivel(font);
	nivel.setCharacterSize(24);
	nivel.setPosition({ 0, 0 });
	nivel.setString("Nivel: " + std::to_string(level));

    sf::Text playerStatus(font);
    playerStatus.setCharacterSize(24);
    playerStatus.setPosition({ 0, 700 - 24*6 });
    playerStatus.setString("Erou HP: " + std::to_string(characters[0]->getHp()) + "/" + std::to_string(characters[0]->getMaxHp()) +
        "Attack: " + std::to_string(characters[0]->getAttackPower()) + "Defence: " + std::to_string(characters[0]->getDefensePower()));

    sf::Text enemyStatus(font);
    enemyStatus.setCharacterSize(24);
    enemyStatus.setPosition({ 0, 700 - 24*5 });
    enemyStatus.setString("Hot HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1] ->getMaxHp()));

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
							player_def = characters[0]->getDefensePower();
                            dynamic_cast<Player*>(characters[0])->takeTurn(*characters[1], action);
							if (characters[1]->getDefensePower() != enemy_def && enemy_def != 0)
							{
								characters[1]->setDefensePower(characters[1]->getDefensePower() / 2);
							}
                            playerTurn = false;
                        }
                        catch (const std::invalid_argument& e) {
                            actionText.setString(e.what());
                        }
                    }
                }

                // Restart next level after victory
                if (!characters[1]->isAlive() && keyPressed->scancode == sf::Keyboard::Scancode::C) {
                    level++;
                    delete characters[1];
                    if (level % 5 == 0) { // La fiecare 5 niveluri, creează un Boss
                        characters[1] = new Boss("Sef", level);
                    }
                    else {
                        characters[1] = new Enemy("Hot", level);
                    }
                    if(level % 5 == 1)
					{
						characters[0]->setMaxHp(characters[0]->getMaxHp() + 30);
						characters[0]->setHp(characters[0]->getMaxHp());
						characters[0]->setAttackPower(characters[0]->getAttackPower() + 10);
						characters[0]->setDefensePower(characters[0]->getDefensePower() + 5);
					}
					else
					{
                        dynamic_cast<Player*>(characters[0])->levelUp();
					}
                    playerTurn = true;
                    enemyAction.setString("");
					actionText.setString("Press A to Attack, D to defend, H to Heal");
                }
            }
        }

        if (!gameOver && !playerTurn) {
            if (characters[1]->isAlive()) {
                int prevDef = characters[1]->getDefensePower();
                int prevHp = characters[0]->getHp();

				enemy_def = characters[1]->getDefensePower();
                dynamic_cast<Enemy*>(characters[1])->takeTurn(*characters[0]);

				if (characters[0]->getDefensePower() != player_def)
				{
					characters[0]->setDefensePower(characters[0]->getDefensePower() / 2);
				}

                if (characters[0]->getHp() < prevHp) {
                    enemyAction.setString(characters[1]->getName() + " attacked you!");
                }
                else if (characters[1]->getDefensePower() > prevDef) {
                    enemyAction.setString(characters[1]->getName() + " defended!");
                }
                else {
                    enemyAction.setString(characters[1]->getName() + " healed!");
                }

                playerTurn = true;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            GameManager::resetGame(characters, level);
            gameOver = false; // Asigură-te că jocul nu este în starea de Game Over
            playerTurn = true; // Jucătorul începe din nou
            actionText.setString("Game Reset! Press A to Attack, D to Defend, H to Heal");
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            bool keyPressed = false;


            // Game over check
            if (!characters[0]->isAlive()) {
                actionText.setString("You Died! Press ESC to Exit");
                gameOver = true;
            }
            else if (!characters[1]->isAlive()) {
                actionText.setString("You defeated the " + characters[1]->getName() + "! Press C to continue.");
            }

            // Update UI
            playerStatus.setString("Erou HP: " + std::to_string(characters[0]->getHp()) + "/" + std::to_string(characters[0]->getMaxHp()) +
                "  ATK: " + std::to_string(characters[0]->getAttackPower()) + "  DEF: " + std::to_string(characters[0]->getDefensePower()));
            enemyStatus.setString("Hot HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1]->getMaxHp()) +
                "  ATK: " + std::to_string(characters[1]->getAttackPower()) + "  DEF: " + std::to_string(characters[1]->getDefensePower()));
			nivel.setString("Nivel: " + std::to_string(level));

            window.clear();
			window.draw(background);
			window.draw(nivel);
            window.draw(playerStatus);
            window.draw(enemyStatus);
            window.draw(actionText);
            window.draw(enemyAction);
            window.display();
        }
    }
    delete characters;
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