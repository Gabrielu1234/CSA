#pragma once
#include "Enemy.h"

class Boss : public Enemy {
public:
    Boss(std::string name, int level);
    ~Boss() override;

    void attack(Character& target) override;
};