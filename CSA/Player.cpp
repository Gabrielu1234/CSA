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
    target.takeDamage(attackPower*2);
}

void Player::defend() {
    setDefensePower(getDefensePower() * 2);
}
void Player::heal() {
    if (hp < maxHp) {
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        std::cout << name << " is already at full health!\n";
    }
}



void Player::takeTurn(Character& target,char action)
{
	int crit = rand() % 100;
    switch (action) {
    case 'A':
        if (crit < 10) { 
            attack(target);
        }
        else
        {
            target - attackPower;
        }

        break;
    case 'D':
        defend();
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

Player& Player::operator++() {
    this->level++;
	if (level % 5 == 0)
	{
		this->attackPower *= 1.3;
		this->defensePower  *= 1.4;
        this->maxHp += 30;
		this->hp += 30;
	}
	else
	{
		this->attackPower *= 1.15;
		this->defensePower *= 1.3;
        this->maxHp += 15;
		this->hp += 15;
	}
    if (this->hp > this->maxHp)
        this->hp = this->maxHp;
    return *this;
}

Player Player::operator++(int) {
    Player temp = *this;
    this->level++;
    if (level % 5 == 0)
    {
        this->attackPower *= 1.3;
        this->defensePower *= 1.3;
		this->maxHp += 30;
        this->hp += 20;
    }
    else
    {
        this->attackPower *= 1.1;
        this->defensePower *= 1.2;
		this->maxHp += 10;
        this->hp += 5;
    }
    if (this->hp > this->maxHp)
        this->hp = this->maxHp;
    return temp;
}
