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

enum class AnimationState {
    Idle,
    PlayerAttack,
    PlayerHit,
    PlayerDeath,
    EnemyDeath
};


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
    AnimationState animState = AnimationState::Idle;
    int animStep = 0;
	int enemyAnimStep = 0;

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
	sf::Texture hotidle;
	if (!hotidle.loadFromFile("idle_hot.png")) {
		std::cerr << "Error loading hot idle texture" << std::endl;
		return -1;
	}
	sf::Texture hotattack;
	if (!hotattack.loadFromFile("attack_hot.png")) {
		std::cerr << "Error loading hot attack texture" << std::endl;
		return -1;
	}
	sf::Texture hotdeath;
	if (!hotdeath.loadFromFile("death_hot.png")) {
		std::cerr << "Error loading hot death texture" << std::endl;
		return -1;
	}
	sf::Texture hothit;
	if (!hothit.loadFromFile("hit_hot.png")) {
		std::cerr << "Error loading hot defend texture" << std::endl;
		return -1;
	}
    int currentFrame = 0;

    sf::Sprite erouSprite(erouidle);
    erouSprite.setPosition({ -50, -75 }); 
    erouSprite.scale({4.0f, 4.0f}); 
    sf::Clock clock;
    float frameDuration = 0.20f;
    float frameDurationDeath = 0.25f;
    int frameCount = 8;
	int framecountattack = 6; 

	sf::Sprite enemySprite(hotidle);
	enemySprite.setPosition({ 250, -200 });
	enemySprite.scale({ 4.0f, 4.0f }); 
	sf::Clock enemyClock;
	float enemyFrameDuration = 0.20f;
	float enemyFrameDurationDeath = 0.25f;
	int enemyFrameCount = 4;
	int enemyFrameCountAttack = 6; 

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
                    alegereinamic = 'A';
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


           
            if (!characters[0]->isAlive() && playerdeath == true) {
                animState = AnimationState::PlayerDeath;
            }
			else if (!characters[1]->isAlive()) {
				animState = AnimationState::EnemyDeath;
			}
            else if (animState == AnimationState::Idle) {
                // Player attacks
                if (alegere == 'A') {
                    animState = AnimationState::PlayerAttack;
                    animStep = 0;
                }
                // Only enemy attacks
                else if (alegere != 'A' && alegereinamic == 'A') {
                    animState = AnimationState::PlayerHit;
                    animStep = 0;
                }
            }

            switch (animState) {
            case AnimationState::PlayerAttack:
                if (clock.getElapsedTime().asSeconds() > frameDuration) {
                    erouSprite.setTexture(erouattack);
                    erouSprite.setTextureRect(sf::IntRect({ animStep * 200,0 }, { 200,200 }));
                    animStep++;
                    clock.restart();
                    if (animStep >= framecountattack) {
                        if (alegereinamic == 'A') {
                            animState = AnimationState::PlayerHit;
                            animStep = 0;
                        }
                        else {
                            animState = AnimationState::Idle;
                            animStep = 0;
                            alegere = '0';
                        }
                    }
                }
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDuration)
                {
					enemySprite.setTexture(hotattack);
					enemySprite.setTextureRect(sf::IntRect({ enemyAnimStep * 200,0 }, { 200,200 }));
					enemyAnimStep++;
					enemyClock.restart();
					if (enemyAnimStep >= enemyFrameCountAttack) {
						if (alegere == 'A') {
							animState = AnimationState::PlayerHit;
							enemyAnimStep = 0;
						}
						else {
							animState = AnimationState::Idle;
							enemyAnimStep = 0;
							alegereinamic = '0';
						}
					}
                }
                break;
            case AnimationState::PlayerHit:
                if (clock.getElapsedTime().asSeconds() > frameDuration) {
                    erouSprite.setTexture(erouhit);
                    erouSprite.setTextureRect(sf::IntRect({ animStep * 200,0 }, { 200,200 }));
                    animStep++;
                    clock.restart();
                    if (animStep >= 4) { 
                        animState = AnimationState::Idle;
                        animStep = 0;
                        alegere = '0';
                        alegereinamic = '0';
                    }
                }
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDuration) {
					enemySprite.setTexture(hothit);
					enemySprite.setTextureRect(sf::IntRect({ enemyAnimStep * 200,0 }, { 200,200 }));
					enemyAnimStep++;
					enemyClock.restart();
                    if (enemyAnimStep >= 3) {
                        animState = AnimationState::Idle;
                        enemyAnimStep = 0;
                        alegereinamic = '0';
                    }
                }
                break;
            case AnimationState::PlayerDeath:
                erouSprite.setTexture(eroudeath);
                if (clock.getElapsedTime().asSeconds() > frameDurationDeath) {
                    erouSprite.setTextureRect(sf::IntRect({ animStep * 200,0 }, { 200,200 }));
                    animStep++;
                    clock.restart();
                    if (animStep >= 6) {
                        alegere = '0';
                        playerdeath = false;
                        gameOver = true;
                        erouSprite.setTextureRect(sf::IntRect({ 5 * 200,0 }, { 200,200 }));
                    }
                }
                break;
			case AnimationState::EnemyDeath:
				enemySprite.setTexture(hotdeath);
				if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDurationDeath) {
					enemySprite.setTextureRect(sf::IntRect({ enemyAnimStep * 200,0 }, { 200,200 }));
					enemyAnimStep++;
					enemyClock.restart();
					if (enemyAnimStep >= 7) {
						alegereinamic = '0';
						gameOver = true;
						enemySprite.setTextureRect(sf::IntRect({ 6 * 200,0 }, { 200,200 }));
					}
				}
				break;
            case AnimationState::Idle:
            default:
                erouSprite.setTexture(erouidle);
                if (clock.getElapsedTime().asSeconds() > frameDuration) {
                    erouSprite.setTextureRect(sf::IntRect({ animStep * 200,0 }, { 200,200 }));
                    animStep = (animStep + 1) % frameCount;
                    clock.restart();
                }
				if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDuration) {
					enemySprite.setTexture(hotidle);
					enemySprite.setTextureRect(sf::IntRect({ enemyAnimStep * 200,0 }, { 200,200 }));
					enemyAnimStep = (enemyAnimStep + 1) % enemyFrameCount;
					enemyClock.restart();
				}
                break;
            }
        }

            // Drawing
            window.clear();
            window.draw(background);
            window.draw(downbackground);
            window.draw(erouSprite);
			window.draw(enemySprite);
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
    delete characters[0];
    delete characters[1];
    return 0;
}