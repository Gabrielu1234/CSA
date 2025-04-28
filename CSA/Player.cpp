// Player.cpp
#include "Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>

Player::Player(std::string name)
    : Character(name, 100, 15, 5), level(1) {
}

Player::~Player() {
}

void Player::attack(Character& target) {
    target.takeDamage(attackPower);
}

void Player::defend() {

}
void Player::heal() {
    if (hp < maxHp) {
        //std::cout << name << " heals for 10 HP.\n";
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        //std::cout << name << " is already at full health!\n";
    }
}

void Player::levelUp() {  
   this->level++;  
   this->attackPower += 5;  
   this->maxHp += 20;  
   //std::cout << name << " leveled up to level " << level << "!\n";  
}


void Player::takeTurn(Character& target)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        attack(target);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        setDefensePower(getDefensePower() + 5);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H))
    {
        heal();
    }
    else
    {
        // Check if *any* key is pressed (but not A, D, H)
        for (int key = (int)sf::Keyboard::Key::A; key <= (int)sf::Keyboard::Key::Z; ++key)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key)))
            {
                throw std::invalid_argument("Invalid key pressed! Use A, D, or H.");
            }
        }
        // If no keys pressed at all: do nothing
    }
}


int Player::getLevel() const {
    return level;
}
