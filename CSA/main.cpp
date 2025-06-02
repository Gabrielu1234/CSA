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



enum class PlayerAnimState { Idle, Attack, Hit, Death};
enum class EnemyAnimState { Idle, Attack, Hit, Death };

int main() {

    srand(static_cast<unsigned int>(time(0)));

    bool gameOver = false;
    bool playerTurn = true;
    bool keyPressed = false;
	bool playerturn = true;
	bool playerdeath = true;
    bool inMenu = true;


    int player_def = 0;
    int enemy_def = 0;

    int level = 1;

    Character* characters[2];

    characters[0] = new Player("Erou");
    characters[1] = new Enemy("Hot", level);

	char alegere = '0';
	char alegereinamic = '0';
    PlayerAnimState playerAnimState = PlayerAnimState::Idle;
    EnemyAnimState  enemyAnimState = EnemyAnimState::Idle;
    int playerAnimStep = 0;
    int enemyAnimStep = 0;

    // generare fereastră
    sf::RenderWindow window(sf::VideoMode({ 1050, 700 }), "CSA");
    window.setFramerateLimit(60);

    // font
    sf::Font font("ToThePointRegular-n9y4.ttf");


    //patrat transparent gri de meniu
	sf::RectangleShape menuBackground(sf::Vector2f(1050, 700));
	menuBackground.setFillColor(sf::Color(128, 128, 128, 230)); 
    menuBackground.setPosition({ 0, 0 });

    //text cu buton play
    sf::Text playText(font);
	playText.setString("Play");
	playText.setCharacterSize(70);
	playText.setFillColor(sf::Color::Black);
	playText.setPosition({ 100, 100 });

	sf::Text playFromFileText(font);
	playFromFileText.setString("Play from file");
	playFromFileText.setCharacterSize(70);
	playFromFileText.setFillColor(sf::Color::Black);
	playFromFileText.setPosition({ 100, 200 });


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
    float frameDuration = 0.10f;
	float frameDurationAttack = 0.15f;
	float frameDurationHit = 0.20f;
    float frameDurationDeath = 0.25f;
    int frameCount = 8;
	int framecountattack = 6; 
	int framecountdeath = 6;
	int framecounthit = 4;

	sf::Sprite enemySprite(hotidle);
	enemySprite.setPosition({ 250, -200 });
	enemySprite.scale({ 4.0f, 4.0f }); 
	sf::Clock enemyClock;
	float enemyFrameDuration = 0.20f;
	float enemyFrameDurationAttack = 0.15f;
	float enemyFrameDurationHit = 0.30f;
	float enemyFrameDurationDeath = 0.25f;
	int enemyFrameCount = 4;
	int enemyFrameCountAttack = 4;
	int enemyFrameCountDeath = 7;
	int enemyFrameCountHit = 3;

    // textele toate
    int dimensiune = 40;

    // nivel
    sf::Text nivel(font);
    nivel.setCharacterSize(60);
    nivel.setPosition({ 0, -30 });
    nivel.setFillColor(sf::Color::Yellow);
    nivel.setString("Nivel: " + std::to_string(level));

    // status erou
    sf::Text playerStatus(font);
    playerStatus.setCharacterSize(dimensiune);
    playerStatus.setPosition({ 40, 700 - 31 * 5 });
    playerStatus.setFillColor(sf::Color::Black);
    playerStatus.setString("Erou HP: " + std::to_string(characters[0]->getHp()) + "/" + std::to_string(characters[0]->getMaxHp()) +
        "Attack: " + std::to_string(characters[0]->getAttackPower()) + "Defence: " + std::to_string(characters[0]->getDefensePower()));

    // status inamic
    sf::Text enemyStatus(font);
    enemyStatus.setCharacterSize(dimensiune);
    enemyStatus.setPosition({ 40, 700 - 32 * 4 });
    enemyStatus.setFillColor(sf::Color::Black);
    enemyStatus.setString("Hot HP: " + std::to_string(characters[1]->getHp()) + "/" + std::to_string(characters[1]->getMaxHp()));

    // status actiune
    sf::Text actionText(font);
    actionText.setCharacterSize(dimensiune);
    actionText.setPosition({ 40, 700 - 33 * 2 });
    actionText.setFillColor(sf::Color::Black);
    actionText.setString("Press S to Save, L to Load, R to Restart");

    // status atac
    sf::Text attacktext(font);
    attacktext.setCharacterSize(dimensiune+10);
    attacktext.setPosition({ 580, 700 - 155 });
    attacktext.setFillColor(sf::Color::Red);
	attacktext.setOutlineColor(sf::Color::Black);
	attacktext.setOutlineThickness(2);
    attacktext.setString("A to Attack");

    // status defend
    sf::Text defendtext(font);
    defendtext.setCharacterSize(dimensiune+10);
    defendtext.setPosition({ 850, 700 - 155 });
    //orange text
	defendtext.setFillColor(sf::Color::Yellow);
	defendtext.setOutlineColor(sf::Color::Black);
	defendtext.setOutlineThickness(2);
    defendtext.setString("D to Defend");

    // status heal
    sf::Text healtext(font);
    healtext.setCharacterSize(dimensiune+10);
    healtext.setPosition({ 600, 700 - 80 });
    healtext.setFillColor(sf::Color::Green);
	healtext.setOutlineColor(sf::Color::Black);
	healtext.setOutlineThickness(2);
    healtext.setString("H to Heal");

    // status escape
    sf::Text escapetext(font);
    escapetext.setCharacterSize(dimensiune+10);
    escapetext.setPosition({ 850, 700 - 80 });
    escapetext.setFillColor(sf::Color::Cyan);
	escapetext.setOutlineColor(sf::Color::Black);
	escapetext.setOutlineThickness(2);
    escapetext.setString("ESC to Exit");

    // status save
    sf::Text actionText2(font);
    actionText2.setCharacterSize(dimensiune);
    actionText2.setPosition({ 40, 40 * 3 });
    actionText2.setFillColor(sf::Color::Yellow);
    actionText2.setString("");

    // status inamic actiune
    sf::Text enemyAction(font);
    enemyAction.setCharacterSize(dimensiune);
    enemyAction.setFillColor(sf::Color::Black);
    enemyAction.setPosition({ 40, 700 - 31 * 3 });


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Buton Play (click pe text)
            if (inMenu) {
                if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::FloatRect playBounds = playText.getGlobalBounds();
                        sf::FloatRect playFromFileBounds = playFromFileText.getGlobalBounds();

                        if (playBounds.contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            inMenu = false; 
                        }
                        if (playFromFileBounds.contains({ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) })) {
                            
                            GameManager::loadFromFile(*characters[0], "player_save.txt");
                            GameManager::loadFromFile(*characters[1], "enemy_save.txt");
                            std::ifstream levelFile("level_save.txt");
                            if (levelFile.is_open()) {
                                levelFile >> level;
                                levelFile.close();
                            }
                            inMenu = false; 
                        }
                    }
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        window.close();
                }
                continue;
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }


            // Player input
            if(playerAnimState==PlayerAnimState::Idle && (enemyAnimState==EnemyAnimState::Idle || enemyAnimState==EnemyAnimState::Death))
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
                if (!characters[1]->isAlive() && keyPressed->scancode == sf::Keyboard::Scancode::C && playerAnimState==PlayerAnimState::Idle) {
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
                    enemyAnimState = EnemyAnimState::Idle;
                    enemyAnimStep = 0;
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && playerAnimState==PlayerAnimState::Idle) {
            GameManager::resetGame(characters, level);
            gameOver = false; 
            playerTurn = true; 
            actionText.setString("Game Reset! Press S to Save, L to Load, R to Restart");
        }

        //buton de salvare
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && playerAnimState==PlayerAnimState::Idle) {
            GameManager::saveToFile(*characters[0], "player_save.txt");
            GameManager::saveToFile(*characters[1], "enemy_save.txt");
            std::ofstream levelFile("level_save.txt");
            if (levelFile.is_open()) {
                levelFile << level;
                levelFile.close();
            }
            actionText2.setString("Game saved!");
        }

        //buton de incarcare
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) && playerAnimState==PlayerAnimState::Idle) {
            GameManager::loadFromFile(*characters[0], "player_save.txt");
            GameManager::loadFromFile(*characters[1], "enemy_save.txt");
            std::ifstream levelFile("level_save.txt");
            if (levelFile.is_open()) {
                levelFile >> level;
                levelFile.close();
            }
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
                if (playerAnimState != PlayerAnimState::Death) {
                    playerAnimState = PlayerAnimState::Death;
                    playerAnimStep = 0;
                }
            }
            else if (!characters[1]->isAlive()) {
                actionText.setString("You defeated the " + characters[1]->getName() + "! Press C to continue.");
                if (enemyAnimState != EnemyAnimState::Death) {
                    enemyAnimState = EnemyAnimState::Death;
                    enemyAnimStep = 0;
                }
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

            nivel.setString("Nivel: " + std::to_string(level));

            // logica animatii
            if (alegere == 'A' && alegereinamic != 'A') {
                // Player
                if (playerAnimState == PlayerAnimState::Idle) {
                    playerAnimState = PlayerAnimState::Attack;
                    playerAnimStep = 0;
                }
                if (playerAnimState == PlayerAnimState::Attack && playerAnimStep >= framecountattack) {
                    playerAnimState = PlayerAnimState::Idle;
                    playerAnimStep = 0;
                    alegere = '0';
                }
                // Enemy
                if (enemyAnimState == EnemyAnimState::Idle) {
                    enemyAnimState = EnemyAnimState::Hit;
                    enemyAnimStep = 0;
                }
            }

            if (enemyAnimState == EnemyAnimState::Hit && enemyAnimStep >= enemyFrameCountHit
                && !(alegere == 'A' && alegereinamic == 'A')) {
                enemyAnimState = EnemyAnimState::Idle;
                enemyAnimStep = 0;
                alegereinamic = '0';
            }

            if (alegere == 'A' && alegereinamic == 'A') {
                if (playerAnimState == PlayerAnimState::Idle && enemyAnimState == EnemyAnimState::Idle) {
                    playerAnimState = PlayerAnimState::Attack;
                    playerAnimStep = 0;
                    enemyAnimState = EnemyAnimState::Hit;
                    enemyAnimStep = 0;
                }
                else if (playerAnimState == PlayerAnimState::Attack && playerAnimStep >= framecountattack &&
                    enemyAnimState == EnemyAnimState::Hit && enemyAnimStep >= enemyFrameCountHit) {
                    playerAnimState = PlayerAnimState::Hit;
                    playerAnimStep = 0;
                    enemyAnimState = EnemyAnimState::Attack;
                    enemyAnimStep = 0;
                }
                else if (playerAnimState == PlayerAnimState::Hit && playerAnimStep >= framecounthit &&
                    enemyAnimState == EnemyAnimState::Attack && enemyAnimStep >= enemyFrameCountAttack) {
                    playerAnimState = PlayerAnimState::Idle;
                    playerAnimStep = 0;
                    enemyAnimState = EnemyAnimState::Idle;
                    enemyAnimStep = 0;
                    alegere = '0';
                    alegereinamic = '0';
                }

                if (playerAnimState == PlayerAnimState::Hit && playerAnimStep >= framecounthit &&
                    enemyAnimState == EnemyAnimState::Attack && enemyAnimStep >= enemyFrameCountAttack) {
                    playerAnimState = PlayerAnimState::Idle;
                    playerAnimStep = 0;
                    enemyAnimState = EnemyAnimState::Idle;
                    enemyAnimStep = 0;
                    alegere = '0';
                    alegereinamic = '0';
                }
            }

            if (alegere != 'A' && alegereinamic == 'A') {
                if (playerAnimState == PlayerAnimState::Idle) {
                    playerAnimState = PlayerAnimState::Hit;
                    playerAnimStep = 0;
                }
                if (enemyAnimState == EnemyAnimState::Idle) {
                    enemyAnimState = EnemyAnimState::Attack;
                    enemyAnimStep = 0;
                }
            }

            

            if (alegere != 'A' && alegereinamic == 'A' &&
                enemyAnimState == EnemyAnimState::Attack && enemyAnimStep >= enemyFrameCountAttack) {
                enemyAnimState = EnemyAnimState::Idle;
                enemyAnimStep = 0;
                alegereinamic = '0';
            }

            if (alegere == 'A' && !characters[1]->isAlive()) {
                if (playerAnimState == PlayerAnimState::Idle) {
                    playerAnimState = PlayerAnimState::Attack;
                    playerAnimStep = 0;
                }
                if (playerAnimState == PlayerAnimState::Attack && playerAnimStep >= framecountattack) {
                    playerAnimState = PlayerAnimState::Idle;
                    playerAnimStep = 0;
                    alegere = '0';
                }
                if (enemyAnimState == EnemyAnimState::Idle) {
                    enemyAnimState = EnemyAnimState::Hit;
                    enemyAnimStep = 0;
                }
                else if (enemyAnimState == EnemyAnimState::Hit && enemyAnimStep >= enemyFrameCountHit) {
                    enemyAnimState = EnemyAnimState::Death;
                    enemyAnimStep = 0;
                }
            }

            if (alegereinamic == 'A' && !characters[0]->isAlive()) {
                if (enemyAnimState == EnemyAnimState::Idle) {
                    enemyAnimState = EnemyAnimState::Attack;
                    enemyAnimStep = 0;
                }
                if (enemyAnimState == EnemyAnimState::Attack && enemyAnimStep >= enemyFrameCountAttack) {
                    enemyAnimState = EnemyAnimState::Idle;
                    enemyAnimStep = 0;
                    alegereinamic = '0';
                }
                if (playerAnimState == PlayerAnimState::Idle) {
                    playerAnimState = PlayerAnimState::Hit;
                    playerAnimStep = 0;
                }
                else if (playerAnimState == PlayerAnimState::Hit && playerAnimStep >= framecounthit) {
                    playerAnimState = PlayerAnimState::Death;
                    playerAnimStep = 0;
                }
            }

            if (playerAnimState == PlayerAnimState::Hit && playerAnimStep >= framecounthit) {
                playerAnimState = PlayerAnimState::Idle;
                playerAnimStep = 0;
            }

            switch (playerAnimState) {
                case PlayerAnimState::Attack:
                if (clock.getElapsedTime().asSeconds() > frameDurationAttack) {
                    erouSprite.setTexture(erouattack);
                    erouSprite.setTextureRect(sf::IntRect({ playerAnimStep * 200,0 }, { 200,200 }));
                    playerAnimStep++;
                    clock.restart();
                }
                break;
            case PlayerAnimState::Hit:
                if (clock.getElapsedTime().asSeconds() > frameDurationHit) {
                    erouSprite.setTexture(erouhit);
                    erouSprite.setTextureRect(sf::IntRect({ playerAnimStep * 200,0 }, { 200,200 }));
                    playerAnimStep++;
                    clock.restart();
                }
                break;
            case PlayerAnimState::Death:
                erouSprite.setTexture(eroudeath);
                {
                    int frame = std::min(playerAnimStep, framecountdeath - 1);
                    erouSprite.setTextureRect(sf::IntRect({ frame * 200,0 }, { 200,200 }));
                    if (playerAnimStep < framecountdeath) {
                        if (clock.getElapsedTime().asSeconds() > frameDurationDeath) {
                            playerAnimStep++;
                            clock.restart();
                        }
                    }
                }
                break;
            case PlayerAnimState::Idle:
            default:
                erouSprite.setTexture(erouidle);
                if (clock.getElapsedTime().asSeconds() > frameDuration) {
                    erouSprite.setTextureRect(sf::IntRect({ playerAnimStep * 200,0 }, { 200,200 }));
                    playerAnimStep = (playerAnimStep + 1) % frameCount;
                    clock.restart();
                }
                break;
			}

            switch (enemyAnimState)
            {
            case EnemyAnimState::Attack:
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDurationAttack) {
                    enemySprite.setTexture(hotattack);
                    int frame = std::min(enemyAnimStep, enemyFrameCountAttack - 1);
                    int reversedFrame = (enemyFrameCountAttack - 1) - frame;
                    enemySprite.setTextureRect(sf::IntRect({ reversedFrame * 200,0 }, { 200,200 }));
                    if (enemyAnimStep < enemyFrameCountAttack) enemyAnimStep++;
                    enemyClock.restart();
                }
                break;
            case EnemyAnimState::Hit:
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDurationHit) {
                    enemySprite.setTexture(hothit);
                    int frame = std::min(enemyAnimStep, enemyFrameCountHit - 1);
                    int reversedFrame = (enemyFrameCountHit - 1) - frame;
                    enemySprite.setTextureRect(sf::IntRect({ reversedFrame * 200,0 }, { 200,200 }));
                    if (enemyAnimStep < enemyFrameCountHit) enemyAnimStep++;
                    enemyClock.restart();
                }
                break;
            case EnemyAnimState::Death:
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDurationDeath) {
                    enemySprite.setTexture(hotdeath);
                    int frame = std::min(enemyAnimStep, enemyFrameCountDeath - 1);
                    int reversedFrame = (enemyFrameCountDeath - 1) - frame;
                    enemySprite.setTextureRect(sf::IntRect({ reversedFrame * 200,0 }, { 200,200 }));
                    if (enemyAnimStep < enemyFrameCountDeath) enemyAnimStep++;
                    enemyClock.restart();
                }
                break;
            case EnemyAnimState::Idle:
            default:
                enemySprite.setTexture(hotidle);
                if (enemyClock.getElapsedTime().asSeconds() > enemyFrameDuration) {
                    int frame = std::min(enemyAnimStep, enemyFrameCount - 1);
                    int reversedFrame = (enemyFrameCount - 1) - frame;
                    enemySprite.setTextureRect(sf::IntRect({ reversedFrame * 200,0 }, { 200,200 }));
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

        if (inMenu) {
            window.draw(menuBackground);
            window.draw(playText);
			window.draw(playFromFileText);
        }
        window.display();
    }
    delete characters[0];
    delete characters[1];
    return 0;
}