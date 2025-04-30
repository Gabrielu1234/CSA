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
    int getHp() const { return hp; }  
    int getMaxHp() const { return maxHp; }  
    int getAttackPower() const { return attackPower; }  
    int getDefensePower() const { return defensePower; }  

	//Setters
	void setHp(int newHp) { hp = newHp; }  
	void setMaxHp(int newMaxHp) { maxHp = newMaxHp; }  
	void setAttackPower(int newAttackPower) { attackPower = newAttackPower; }  
	void setDefensePower(int newDefensePower) { defensePower = newDefensePower; }  




    bool isAlive() const;

    // New methods
    std::string getName() const;
	void setName(std::string newName) { name = newName; }
    void takeDamage(int damage);  

	friend class GameManager; 

    // Operator overloads
    Character& operator-(int damage);
    Character& operator+(int heal);

    Character& operator=(const Character& other);
    bool operator==(const Character& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Character& c);

};