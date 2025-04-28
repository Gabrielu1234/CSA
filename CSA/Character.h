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

    // Getters
    int getHp() const { return hp; }  // Returns the current HP
    int getMaxHp() const { return maxHp; }  // Returns the maximum HP
    int getAttackPower() const { return attackPower; }  // Returns the attack power
    int getDefensePower() const { return defensePower; }  // Returns the defense power

	//Setters
	void setHp(int newHp) { hp = newHp; }  // Sets the current HP
	void setMaxHp(int newMaxHp) { maxHp = newMaxHp; }  // Sets the maximum HP
	void setAttackPower(int newAttackPower) { attackPower = newAttackPower; }  // Sets the attack power
	void setDefensePower(int newDefensePower) { defensePower = newDefensePower; }  // Sets the defense power




    bool isAlive() const;

    // New methods
    std::string getName() const;         // Returns the character's name
    void takeDamage(int damage);         // Reduces the character's HP by damage amount

    // Operator overloads
    Character& operator=(const Character& other);
    bool operator==(const Character& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Character& c);

};