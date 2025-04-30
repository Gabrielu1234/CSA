#include "Boss.h"
#include <iostream>

Boss::Boss(std::string name, int level)
    : Enemy(name, level) {
    setMaxHp(getMaxHp() * 1.5);
    setHp(getMaxHp());
    setAttackPower(getAttackPower() * 1.2);
    setDefensePower(getDefensePower() * 1.4);
}

Boss::~Boss() {
}


void Boss::attack(Character& target) {
    target.takeDamage(getAttackPower());
}
