#include "Boss.h"
#include <iostream>

Boss::Boss(std::string name, int level)
    : Enemy(name, level) {
    setMaxHp(getMaxHp() * 2);
    setHp(getMaxHp());
    setAttackPower(getAttackPower() * 2);
    setDefensePower(getDefensePower() * 2);
}

Boss::~Boss() {
}


void Boss::attack(Character& target) {
    target.takeDamage(getAttackPower());
}
