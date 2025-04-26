// Player.h
#pragma once
#include "Character.h"

class Player : public Character {
private:
    int level;
public:
    Player(std::string name);
    virtual ~Player();

    // Player-specific actions
    void attack(Character& target) override;
    void defend() override;
    void heal() override;

    void levelUp();

    void takeTurn(Character& target);

    // Getter
    int getLevel() const;
};