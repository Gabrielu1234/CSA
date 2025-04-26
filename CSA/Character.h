// Character.h
#pragma once
#include <iostream>
#include <string>

class Character {
protected:
    std::string name;
    int hp;
    int maxHp;
    int attackPower;
    int defensePower;

public:
    Character(std::string name, int maxHp, int attackPower, int defensePower);
    virtual ~Character();

    virtual void attack(Character& target) = 0;
    virtual void defend() = 0;
    virtual void heal() = 0;
    virtual void printStatus() const;

    bool isAlive() const;

    // New methods
    std::string getName() const;         // Returns the character's name
    void takeDamage(int damage);         // Reduces the character's HP by damage amount

    // Operator overloads
    Character& operator=(const Character& other);
    bool operator==(const Character& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Character& c);
};
