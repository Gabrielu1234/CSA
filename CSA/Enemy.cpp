// Enemy.cpp
#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(std::string name, int level)
    : Character(name, 70 + (level * 10), 10 + (level * 2), 3 + ((level-1)*2)-1), level(level) {
}

Enemy::~Enemy() {
    
}

void Enemy::attack(Character& target) {
    target.takeDamage(attackPower);
}

void Enemy::defend() { 
    setDefensePower(getDefensePower() * 2);
 }

void Enemy::heal() {
    if (hp < maxHp) {
        hp += 10;
        if (hp > maxHp) hp = maxHp;
    }
    else {
        std::cout << name << " is already at full health!\n";
    }
}

void Enemy::takeTurn(Character& target)  {
    int action = rand() % 100;  

    if (action < 60) { 

        attack(target);
    }
    else if (action < 80) { 
        defend();
    }
    else { 
        heal();
    }
}

int Enemy::getLevel() const {
    return level;
}
