// GameManager.cpp
#include "GameManager.h"
#include "Boss.h"
#include <iostream>

void GameManager::resetGame(Character* characters[], int& level) {
    // Resetează nivelul
    level = 1;

    // Resetează jucătorul
    characters[0]->setHp(characters[0]->getMaxHp());
    characters[0]->setAttackPower(10); // Valori implicite
    characters[0]->setDefensePower(5);

    // Creează un nou inamic
    delete characters[1];
    characters[1] = new Enemy("Hot", level);

    std::cout << "Jocul a fost resetat! Nivelul este acum " << level << ".\n";
}
