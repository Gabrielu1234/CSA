// Enemy.h
#pragma once
#include "Character.h"

class Enemy : public Character {
private:
    int level;
public:
    Enemy(std::string name, int level);
    virtual ~Enemy();

    // Enemy-specific actions
    void attack(Character& target) override;
    void defend() override;
    void heal() override;

    void takeTurn(Character& target);

    // Getter
    int getLevel() const;
};
