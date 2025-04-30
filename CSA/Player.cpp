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
        //std::cout << name << " heals for 10 HP.\n";
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        //std::cout << name << " is already at full health!\n";
    }
}



void Player::takeTurn(Character& target,char action)
{
	int crit = rand() % 100;
    switch (action) {
    case 'A':
        if (crit < 10) { // 10% chance for a critical hit
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
		this->attackPower *= 1.5;
		this->defensePower  *= 1.6;
		this->hp += 40;
	}
	else
	{
		this->attackPower *= 1.25;
		this->defensePower *= 1.4;
		this->hp += 15;
	}
    if (this->hp > this->maxHp)
        this->hp = this->maxHp;
    this->maxHp += 20;
    return *this;
}

Player Player::operator++(int) {
    Player temp = *this;
    this->level++;
    if (level % 5 == 0)
    {
        this->attackPower *= 1.5;
        this->defensePower *= 1.6;
        this->hp += 40;
    }
    else
    {
        this->attackPower *= 1.25;
        this->defensePower *= 1.4;
        this->hp += 15;
    }
    if (this->hp > this->maxHp)
        this->hp = this->maxHp;
    this->maxHp += 20;
    return temp;
}
