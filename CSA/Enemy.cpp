// Enemy.cpp
#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(std::string name, int level)
    : Character(name, 50 + (level * 10), 10 + (level * 3), 3), level(level) {
    std::cout << "Enemy " << name << " created with level " << level << ".\n";
}

Enemy::~Enemy() {
    std::cout << "Enemy destroyed.\n";
}

void Enemy::attack(Character& target) {
    std::cout << name << " attacks " << target.getName() << " for " << attackPower << " damage.\n";
    target.takeDamage(attackPower);
}

void Enemy::defend() {
    std::cout << name << " defends, reducing damage by " << defensePower << ".\n";
    // Add defense mechanic here if you want the enemy to reduce incoming damage
}

void Enemy::heal() {
    if (hp < maxHp) {
        std::cout << name << " heals for 10 HP.\n";
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        std::cout << name << " is already at full health!\n";
    }
}

void Enemy::takeTurn(Character& target)  {
    // Biased random selection for enemy actions
    int action = rand() % 100;  // Random number between 0 and 99

    if (action < 60) {  // 60% chance to attack
        attack(target);
    }
    else if (action < 80) {  // 20% chance to defend
        defend();
    }
    else {  // 20% chance to heal
        heal();
    }
}

int Enemy::getLevel() const {
    return level;
}
