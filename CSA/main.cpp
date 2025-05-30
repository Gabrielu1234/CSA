#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Character.h"
#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>

#include <memory>

//salut 2
// 3 alocari dinamice new, 2 delete
//ierarhii de clase (charater in 2 (player si enemy), boss mosteneste enemy
//supradefinire a multi supraoperatori, 2 pana acum cred ca sunt folositi
//polimorfism de mostenire avem, ad-hoc avem, parametric nu (poate pun)
//tablou polimorfm am, Character* characters[2]
//supradefinirea a cel putin o metoda am, attack def si heal
//metoda statica de resetare a jocului am, GameManager::resetGame, mai sunt optiuni de altele, sunet animatii etc.
//exceptii am, in player la takeTurn, in enemy la heal si cele de la sfml
//functii friend am una, care fusese 2.
// clasa abstracta am, Character e abstracta
//lucru cu fisiere am, salvarea nivelului, a caracterelor, a inamicilor
//clase template nu am

//de lucru la frontend

int main() {


    // Seed for random number generation (for enemy actions)
    srand(static_cast<unsigned int>(time(0)));

    bool gameOver = false;
    bool playerTurn = true;
    bool keyPressed = false;
	bool playerturn = true;
	bool playerdeath = true;

    int player_def = 0;
    int enemy_def = 0;

    int level = 1;

    Character* characters[2];

    characters[0] = new Player("Erou");
    characters[1] = new Enemy("Hot", level);
	char alegere = '0';
	char alegereinamic = '0';

    // generare fereastră
    sf::RenderWindow window(sf::VideoMode({ 1050, 700 }), "CSA");
    window.setFramerateLimit(60);

    // font
    sf::Font font("arial.ttf");

    // poza background
    sf::Texture texture;
    if (!texture.loadFromFile("arena.png")) {
        std::cerr << "Error loading background texture" << std::endl;
        return -1;
    }
    sf::Sprite background(texture);

    // a doua poza background
    sf::Texture downtexture;
    if (!downtexture.loadFromFile("down.png")) {
        std::cerr << "Error loading down texture" << std::endl;
        return -1;
    }
    sf::Sprite downbackground(downtexture);
    downbackground.setPosition({ 0, 541 });

    // animatie erou
    sf::Texture erouidle;
    if (!erouidle.loadFromFile("idle_erou.png")) {
        std::cerr << "Error loading erou texture" << std::endl;
        return -1;
    }
	sf::Texture erouattack;
	if (!erouattack.loadFromFile("attack_erou.png")) {
		std::cerr << "Error loading erou attack texture" << std::endl;
		return -1;
	}
    sf::Texture eroudeath;
	if (!eroudeath.loadFromFile("death_erou.png")) {
		std::cerr << "Error loading erou death texture" << std::endl;
		return -1;
	}
	sf::Texture erouhit;
	if (!erouhit.loadFromFile("hit_erou.png")) {
		std::cerr << "Error loading erou defend texture" << std::endl;
		return -1;
	}
    sf::Sprite erouSprite(erouidle);
    int currentFrame = 0;
    erouSprite.setPosition({ -50, -50 }); 
    sf::Clock clock;
    erouSprite.scale({4.0f, 4.0f}); 
    float frameDuration = 0.20f;
    float frameDurationDeath = 0.25f;
    int frameCount = 8;
	int framecountattack = 6; 

    // textele toate

    // nivel
    sf::Text nivel(font);
    nivel.setCharacterSize(24);
    nivel.setPosition({ 0, 0 });
    nivel.setFillColor(sf::Color::Black);
    nivel.setString("Nivel: " + std::to_string(level));

    // status erou
    sf::Text playerStatus(font);
    playerStatus.setCharacterSize(24);
    playerStatus.setPosition({ 40, 700 - 24 * 6 });
    playerStatus.setFillColor(sf::Color::Black);
    playerStatus.setString("Erou HP: " + std::to_string(characters[0]->getHp()) + "/" + std::to_string(characters[0]->getMaxHp()) +
        "Attack: " + std::to_string(characters[0]->getAttackPower()) + "Defence: " + std::to_string(characters[0]->getDefensePower()));

    // status inamic
    sf::Text enemyStatus(font);
    enemyStatus.setCharacterSize(24);
    enemyStatus.setPosition({ 40, 700 - 24 * 5 });
    enemyStatus.setFillColor(sf::Color::Black);
    enemyStatus.setString("Hot HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1]->getMaxHp()));

    // status actiune
    sf::Text actionText(font);
    actionText.setCharacterSize(24);
    actionText.setPosition({ 40, 700 - 24 * 2 });
    actionText.setFillColor(sf::Color::Black);
    actionText.setString("Press S to Save, L to Load, R to Restart");

    // status atac
    sf::Text attacktext(font);
    attacktext.setCharacterSize(24);
    attacktext.setPosition({ 600, 700 - 130 });
    attacktext.setFillColor(sf::Color::Black);
    attacktext.setString("A to Attack");

    // status defend
    sf::Text defendtext(font);
    defendtext.setCharacterSize(24);
    defendtext.setPosition({ 850, 700 - 130 });
    defendtext.setFillColor(sf::Color::Black);
    defendtext.setString("D to Defend");

    // status heal
    sf::Text healtext(font);
    healtext.setCharacterSize(24);
    healtext.setPosition({ 600, 700 - 60 });
    healtext.setFillColor(sf::Color::Black);
    healtext.setString("H to Heal");

    // status escape
    sf::Text escapetext(font);
    escapetext.setCharacterSize(24);
    escapetext.setPosition({ 850, 700 - 60 });
    escapetext.setFillColor(sf::Color::Black);
    escapetext.setString("ESC to Exit");

    // status save
    sf::Text actionText2(font);
    actionText2.setCharacterSize(24);
    actionText2.setPosition({ 40, 24 * 3 });
    actionText2.setFillColor(sf::Color::Black);
    actionText2.setString("");

    // status inamic actiune
    sf::Text enemyAction(font);
    enemyAction.setCharacterSize(24);
    enemyAction.setFillColor(sf::Color::Black);
    enemyAction.setPosition({ 40, 700 - 24 * 3 });


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
                            Player* player = dynamic_cast<Player*>(characters[0]);
                            if (player) {
                                player_def = player->getDefensePower();
                                player->takeTurn(*characters[1], action);
                            }
                            else {
                                std::cerr << "Error: characters[0] is not a Player object." << std::endl;
                                actionText.setString("Invalid player object.");
                            }
                            if ((characters[1]->getDefensePower() != enemy_def) && (enemy_def != 0))
                            {
                                characters[1]->setDefensePower(characters[1]->getDefensePower() / 2);
                            }
                            playerTurn = false;
							alegere = action;
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
                    if (level % 5 == 1)
                    {
                        characters[0]->setMaxHp(characters[0]->getMaxHp() + 30);
                        characters[0]->setHp(characters[0]->getMaxHp());
                        characters[0]->setAttackPower(characters[0]->getAttackPower() + 10);
                        characters[0]->setDefensePower(characters[0]->getDefensePower() + 5);
                    }
                    else
                    {
                        Player* player = dynamic_cast<Player*>(characters[0]);
                        if (player) {
                            ++(*player);
                        }
                    }
					enemy_def = characters[1]->getDefensePower();
                    playerTurn = true;
                    enemyAction.setString("");
                    actionText.setString("Press S to Save, L to Load, R to Restart");
                }
            }
        }

        // tura inamic
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
                    alegereinamic = 'D';
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

        //buton de restart
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            GameManager::resetGame(characters, level);
            gameOver = false; // Asigură-te că jocul nu este în starea de Game Over
            playerTurn = true; // Jucătorul începe din nou
            actionText.setString("Game Reset! Press S to Save, L to Load, R to Restart");
        }

        //buton de salvare
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            GameManager::saveToFile(*characters[0], "player_save.txt");
            GameManager::saveToFile(*characters[1], "enemy_save.txt");
            actionText2.setString("Game saved!");
        }

        //buton de incarcare
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
            GameManager::loadFromFile(*characters[0], "player_save.txt");
            GameManager::loadFromFile(*characters[1], "enemy_save.txt");
            actionText2.setString("Game loaded!");
        }

        // control sa nu apase pe mai multe butoane
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
            bool keyPressed = false;


            // Game over check
            if (!characters[0]->isAlive()) {
                actionText.setString("You Died! Press ESC to Exit");
                alegere = 'L';
                gameOver = true;
            }
            else if (!characters[1]->isAlive()) {
                actionText.setString("You defeated the " + characters[1]->getName() + "! Press C to continue.");
            }

            // Update UI
            playerStatus.setString("Erou HP: " + std::to_string(characters[0]->getHp()) + "/" + std::to_string(characters[0]->getMaxHp()) +
                "  ATK: " + std::to_string(characters[0]->getAttackPower()) + "  DEF: " + std::to_string(characters[0]->getDefensePower()));

            if (characters[1]->getName() == "Sef")
                enemyStatus.setString("Sef HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1]->getMaxHp()) +
                    "  ATK: " + std::to_string(characters[1]->getAttackPower()) + "  DEF: " + std::to_string(characters[1]->getDefensePower()));
            else
                enemyStatus.setString("Hot HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1]->getMaxHp()) +
                    "  ATK: " + std::to_string(characters[1]->getAttackPower()) + "  DEF: " + std::to_string(characters[1]->getDefensePower()));

            // Update action texts
            nivel.setString("Nivel: " + std::to_string(level));


            // Update animation
            if (alegere == 'A')
            {
                if (playerturn==true || alegereinamic=='0')
                {
                    erouSprite.setTexture(erouattack);
                    if (clock.getElapsedTime().asSeconds() > frameDuration) {
                        currentFrame = currentFrame % framecountattack;
                        erouSprite.setTextureRect(sf::IntRect({ currentFrame * 200,0 }, { 200,200 }));
                        currentFrame++;
                        clock.restart();
                        if (currentFrame >= framecountattack) {
                            alegere = '0';
                            currentFrame = 0;
                            erouSprite.setTexture(erouidle);
                            erouSprite.setTextureRect(sf::IntRect({ 0,0 }, { 200,200 }));
							playerturn = false; // Schimbă la tura inamicului
                        }
                    }
                }

            }
            else if (alegereinamic == 'D')
            {
                if (playerturn == false)
                {
                    erouSprite.setTexture(erouhit);
                    if (clock.getElapsedTime().asSeconds() > frameDuration) {
                        currentFrame = currentFrame % 4;
                        erouSprite.setTextureRect(sf::IntRect({ currentFrame * 200,0 }, { 200,200 }));
                        currentFrame++;
                        clock.restart();
                        if (currentFrame >= 4) {
                            alegereinamic = '0';
                            currentFrame = 0;
                            erouSprite.setTexture(erouidle);
                            erouSprite.setTextureRect(sf::IntRect({ 0,0 }, { 200,200 }));
							playerturn = true; // Schimbă la tura jucătorului
                        }
                    }
                }
            }
            else if(alegere == 'L' && playerdeath==true)
            {
                erouSprite.setTexture(eroudeath);
                if (clock.getElapsedTime().asSeconds() > frameDurationDeath) {
                    currentFrame = currentFrame % 6; 
                    erouSprite.setTextureRect(sf::IntRect({ currentFrame * 200,0 }, { 200,200 }));
                    currentFrame++;
                    clock.restart();
                    if (currentFrame >= 6) {
                        alegere = '0';
                        playerdeath = false;
                        gameOver = true;
                        // Stay on the last frame of the death animation
                        erouSprite.setTextureRect(sf::IntRect({ 5*200,0 }, { 200,200 }));
                    }
                }
            }
            else
            {
                if (gameOver == false)
                {
                erouSprite.setTexture(erouidle);
          
                if (clock.getElapsedTime().asSeconds() > frameDuration) {
                    currentFrame = currentFrame % frameCount;
                    erouSprite.setTextureRect(sf::IntRect({ currentFrame * 200,0 }, { 200,200 }));
                    currentFrame++;
                    clock.restart();
                }
            }
            }


            // Drawing
            window.clear();
            window.draw(background);
            window.draw(downbackground);
            window.draw(erouSprite);
            window.draw(nivel);
            window.draw(playerStatus);
            window.draw(enemyStatus);
            window.draw(actionText);
            window.draw(actionText2);
            window.draw(enemyAction);
            window.draw(attacktext);
            window.draw(defendtext);
            window.draw(healtext);
            window.draw(escapetext);
            window.display();
        }
    }
    delete characters[0];
    delete characters[1];
    return 0;
}