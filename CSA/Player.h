// Player.h
#pragma once
#include "Character.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>

class Player : public Character {
private:
    int level;
	sf::Texture atackplayer;
	sf::Texture defendplayer;

public:


    Player(std::string name);
    virtual ~Player();

    // Player-specific actions
    void attack(Character& target) override;
    void defend() override;
    void heal() override;


    void takeTurn(Character& target,char action);

    // Getter
    int getLevel() const;

    //Operators
    Player& operator++(); // Prefix
    Player operator++(int); // Postfix
};