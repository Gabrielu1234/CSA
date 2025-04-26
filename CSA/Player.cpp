// Player.cpp
#include "Player.h"
#include <iostream>

Player::Player(std::string name)
    : Character(name, 100, 15, 5), level(1) {
    std::cout << "Player " << name << " created with level " << level << ".\n";
}

Player::~Player() {
    std::cout << "Player destroyed.\n";
}

void Player::attack(Character& target) {
    std::cout << name << " attacks " << target.getName() << " for " << attackPower << " damage.\n";
    target.takeDamage(attackPower);
}

void Player::defend() {
    std::cout << name << " defends, reducing damage by " << defensePower << ".\n";
    // Add defense mechanic here (e.g., reduce incoming damage)
}

void Player::heal() {
    if (hp < maxHp) {
        std::cout << name << " heals for 10 HP.\n";
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        std::cout << name << " is already at full health!\n";
    }
}

void Player::levelUp() {
    level++;
    attackPower += 5;
    maxHp += 20;
    std::cout << name << " leveled up to level " << level << "!\n";
}

void Player::takeTurn(Character& target) {
    std::cout << "\nIt's your turn! Choose an action:\n";
    std::cout << "1. Attack\n2. Defend\n3. Heal\n";

    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        attack(target);
        break;
    case 2:
        defend();
        break;
    case 3:
        heal();
        break;
    default:
        std::cout << "Invalid choice, you do nothing.\n";
        break;
    }
}

int Player::getLevel() const {
    return level;
}
