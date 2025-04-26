// Character.cpp
#include "Character.h"

Character::Character(std::string name, int maxHp, int attackPower, int defensePower)
    : name(name), maxHp(maxHp), hp(maxHp), attackPower(attackPower), defensePower(defensePower) {
}

Character::~Character() {
    std::cout << name << " destroyed.\n";
}

void Character::printStatus() const {
    std::cout << name << " HP: " << hp << "/" << maxHp << " | ATK: " << attackPower << " | DEF: " << defensePower << "\n";
}

bool Character::isAlive() const {
    return hp > 0;
}

std::string Character::getName() const {
    return name;  // Return the character's name
}

void Character::takeDamage(int damage) {
    int finalDamage = damage - defensePower;  // Apply defense to reduce damage
    if (finalDamage < 0) finalDamage = 0;  // Ensure no negative damage
    hp -= finalDamage;  // Reduce HP by damage amount
    std::cout << name << " takes " << finalDamage << " damage!\n";
}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        name = other.name;
        hp = other.hp;
        maxHp = other.maxHp;
        attackPower = other.attackPower;
        defensePower = other.defensePower;
    }
    return *this;
}

bool Character::operator==(const Character& other) const {
    return (name == other.name) && (hp == other.hp);
}

std::ostream& operator<<(std::ostream& out, const Character& c) {
    out << c.name << " HP: " << c.hp << "/" << c.maxHp;
    return out;
}


