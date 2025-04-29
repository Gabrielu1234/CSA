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
   this->defensePower += 2;
   this->hp += 15;
   if (this->hp > this->maxHp)
	   this->hp = this->maxHp;
   this->maxHp += 20;  
   //std::cout << name << " leveled up to level " << level << "!\n";  
}


void Player::takeTurn(Character& target,char action)
{
    switch (action) {
    case 'A':
        attack(target);
        break;
    case 'D':
        setDefensePower(getDefensePower() *2);
        break;
    case 'H':
        heal();
        break;
    default:
        throw std::invalid_argument("Invalid action passed to Player::takeTurn");
    }
}


int Player::getLevel() const {
    return level;
}
